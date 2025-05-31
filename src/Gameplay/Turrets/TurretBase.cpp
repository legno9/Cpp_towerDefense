#include <Gameplay/Turrets/TurretBase.h>
#include <iostream>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <Core/GameObjectManager.h>
#include <Core/JsonManager.h>
#include <Gameplay/Enemies/EnemyBase.h>
#include <Gameplay/Enemies/EnemyInvisible.h>
#include <Utils/Common.h>

TurretBase::TurretBase(const sf::Vector2f& position, const std::string& configPath)
    : GameObject(position.x, position.y)
{
    nlohmann::json turretJson = JsonManager::getInstance().loadConfigFile(configPath);

    if (turretJson.is_null() || turretJson.empty()) 
    {
        std::cerr << "ERROR: Failed to load or parse turret config: " << configPath << std::endl;
        throw std::runtime_error("Invalid turret config file.");
    }

    if (turretJson.contains("levels") && turretJson["levels"].is_array()) 
    {
        m_allLevelsData = turretJson["levels"];
        m_maxLevel = static_cast<int>(m_allLevelsData.size());
    } 
    else 
    {
        std::cerr << "ERROR: Turret config '" << configPath << "' missing 'levels' array." << std::endl;
        throw std::runtime_error("Turret config missing 'levels' array.");
    }

    m_projectileType = JsonManager::getInstance().getString(turretJson, "projectileType");
    m_projectileConfigPath = JsonManager::getInstance().getString(turretJson, "projectileConfigPath");

    m_sprite = std::make_unique<sf::Sprite>();

    std::string animationPath = JsonManager::getInstance().getString(turretJson, "animationDataPath");
    float scale = JsonManager::getInstance().getFloat(turretJson, "spriteScale");
    
    m_animationComponent = std::make_unique<AnimationComponent>(*m_sprite, animationPath);
    m_animationComponent->play("idle");
    m_animationComponent->setDirection(Direction::South);

    m_sprite->setOrigin(m_sprite->getLocalBounds().width / 2, m_sprite->getLocalBounds().height / 2);
    m_sprite->setPosition(m_position);
    m_sprite->setScale(scale,scale);

    if (m_level > 0 && m_level <= m_allLevelsData.size()) 
    {
        applyConfig(m_allLevelsData[m_level - 1]);
    } 
    else 
    {
        std::cerr << "ERROR: Turret config '" << configPath << "' has no level 1 data." << std::endl;
        throw std::runtime_error("Turret config missing level 1 data.");
    }

    getAnimLevelsData(turretJson);

    RenderManager::getInstance().addToRenderQueue(*m_sprite, ZOrder::Foreground);
}

TurretBase::~TurretBase()
{
    if (m_sprite)
    {
        RenderManager::getInstance().removeFromRenderQueue(*m_sprite, ZOrder::Foreground);
    }
}

void TurretBase::getAnimLevelsData(const nlohmann::json& json)
{
    std::string animationDataPath = JsonManager::getInstance().getString(json, "animationDataPath");
    if (animationDataPath.empty()) 
    {
        std::cerr << "ERROR: Turret config missing 'animationDataPath'." << std::endl;
        throw std::runtime_error("Turret config missing 'animationDataPath'.");
    }

    nlohmann::json animationData = JsonManager::getInstance().loadConfigFile(animationDataPath);
    if (animationData.is_null() || animationData.empty()) 
    {
        std::cerr << "ERROR: Failed to load or parse animation data: " << animationDataPath << std::endl;
        throw std::runtime_error("Invalid animation data file.");
    }
    if (animationData.contains("texturePaths") && animationData["texturePaths"].is_array()) 
    {
        m_allAnimLevelsData = animationData["texturePaths"];
    } 
    else 
    {
        std::cerr << "ERROR: Animation data '" << animationDataPath << "' missing 'levels' array." << std::endl;
        throw std::runtime_error("Animation data missing 'levels' array.");
    }
}

void TurretBase::applyConfig(const nlohmann::json& configData)
{
    m_damage = JsonManager::getInstance().getFloat(configData, "damage");
    m_actionRange = JsonManager::getInstance().getFloat(configData, "actionRange");
    m_actionRate = JsonManager::getInstance().getFloat(configData, "actionRate");
}

void TurretBase::update(uint32_t deltaMilliseconds)
{
    float deltaSeconds = static_cast<float>(deltaMilliseconds) / 1000.0f;

    if (m_markedForRemoval) return;
    
    if (m_animationComponent)
    {
        m_animationComponent->update(deltaMilliseconds);
    }

    m_actionTimer += deltaSeconds;

    unsigned int targetEnemyId = getLeadEnemy();
    updateFacingDirection(targetEnemyId);
    
    if (targetEnemyId != 0 && m_actionTimer >= m_actionRate)
    {
        action(targetEnemyId);
    }
}

unsigned int TurretBase::getLeadEnemy()
{
    std::vector<unsigned int> enemiesInRangeIds = GameObjectManager::getInstance().getEnemiesInRadius(m_position, m_actionRange);
    
    unsigned int targetEnemyId = 0;
    float furthestDistanceAlongPath = -1.0f;

    for (unsigned int enemyId : enemiesInRangeIds)
    {
        GameObject* rawEnemy = GameObjectManager::getInstance().getGameObjectById(enemyId);
        EnemyBase* enemy = dynamic_cast<EnemyBase*>(rawEnemy);

        if (enemy && !enemy->isMarkedForRemoval() && !enemy->isPredictedDead() && enemy->isTargetable())
        {
            try 
            {
                float enemyProgress = enemy->getDistanceAlongPath();

                if (enemyProgress > furthestDistanceAlongPath)
                {
                    furthestDistanceAlongPath = enemyProgress;
                    targetEnemyId = enemyId;
                }
            } 
            catch (const std::exception& e) 
            {
                std::cerr << "WARNING: getLeadEnemy: Exception calling getDistanceAlongPath() for enemy ID " << enemyId << ": " << e.what() << ". Skipping this enemy." << std::endl;
            }
        }
    }
    
    return targetEnemyId;
}

void TurretBase::updateFacingDirection(unsigned int targetEnemyId)
{
    if (m_isAttacking) {return;}

    if (m_animationComponent)
    {
        if (targetEnemyId != 0) 
        {
            GameObject* rawTarget = GameObjectManager::getInstance().getGameObjectById(targetEnemyId);
            EnemyBase* targetEnemy = dynamic_cast<EnemyBase*>(rawTarget);
            
            if (targetEnemy != nullptr && !targetEnemy->isMarkedForRemoval())
            {
                sf::Vector2f directionToTarget = targetEnemy->getPosition() - m_position;
                m_animationComponent->SetDirectionFromVector(directionToTarget);
            }
        }
        // else
        // {
        //     m_animationComponent->setDirection(Direction::South);
        // }
    }
}

void TurretBase::action(unsigned int currentTargetEnemyId)
{
    if (currentTargetEnemyId == 0)
    {
        return;
    }

    GameObject* rawTarget = GameObjectManager::getInstance().getGameObjectById(currentTargetEnemyId);
    EnemyBase* targetEnemy = dynamic_cast<EnemyBase*>(rawTarget);

    if (!targetEnemy || targetEnemy->isMarkedForRemoval()) 
    {
        std::cerr << "WARNING: TurretBase::action() target ID " << currentTargetEnemyId << " became invalid during action execution." << std::endl;
        return;
    }

    targetEnemy->predictDamage(m_damage);
    m_actionTimer = 0.0f;
    performAttack(currentTargetEnemyId);
}

void TurretBase::performAttack(unsigned int targetEnemyId)
{
    m_isAttacking = true;

    if (m_animationComponent) 
    {
        m_animationComponent->play("shoot", true);
        m_animationComponent->onFrameEvent = [this, targetEnemyId](const std::string& animName, int frameIdx) 
        {
            if (animName == "shoot") 
            { 
                GameObjectManager::getInstance().spawnProjectile
                ( 
                    stringToGameObjectType(m_projectileType),
                    m_position,                      
                    m_projectileConfigPath,                          
                    targetEnemyId,
                    m_damage                       
                );
                m_animationComponent->onFrameEvent = nullptr;
            }
        };
        m_animationComponent->onAnimationEnd = [this]() 
        {
            m_isAttacking = false;
            m_animationComponent->play("idle");
        };
        
    }
}

void TurretBase::upgrade()
{
    if (isMaxLevel()) 
    {
        std::cerr << "WARNING: Attempted to upgrade Turret ID " << getId() << " but it is already at max level (" << m_maxLevel << ")." << std::endl;
        return;
    }

    m_level++;

    if (m_level > 0 && m_level <= m_allLevelsData.size()) 
    {
        applyConfig(m_allLevelsData[m_level - 1]);
        m_animationComponent->changeTexture(m_allAnimLevelsData[m_level - 1]);
    }
}

void TurretBase::sell()
{
    m_markedForRemoval = true;
}