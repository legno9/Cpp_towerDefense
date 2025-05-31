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

    applyConfig(turretJson);

    m_sprite = std::make_unique<sf::Sprite>();

    std::string animationPath = JsonManager::getInstance().getString(turretJson, "animationDataPath");
    float scale = JsonManager::getInstance().getFloat(turretJson, "spriteScale");
    
    m_animationComponent = std::make_unique<AnimationComponent>(*m_sprite, animationPath);
    m_animationComponent->play("idle");
    m_animationComponent->setDirection(Direction::South);

    m_sprite->setOrigin(m_sprite->getLocalBounds().width / 2, m_sprite->getLocalBounds().height / 2);
    m_sprite->setPosition(m_position);
    m_sprite->setScale(scale,scale);

    m_actionTimer = 0.0f;
    m_level = 1;

    RenderManager::getInstance().addToRenderQueue(*m_sprite, ZOrder::Foreground);
}

TurretBase::~TurretBase()
{
    if (m_sprite)
    {
        RenderManager::getInstance().removeFromRenderQueue(*m_sprite, ZOrder::Foreground);
    }
}

void TurretBase::applyConfig(const nlohmann::json& configData)
{
    m_damage = JsonManager::getInstance().getFloat(configData, "damage");
    m_actionRange = JsonManager::getInstance().getFloat(configData, "actionRange");
    m_actionRate = JsonManager::getInstance().getFloat(configData, "actionRate");

    m_buyPrice = JsonManager::getInstance().getInt(configData, "buyPrice");
    m_sellPrice = JsonManager::getInstance().getInt(configData, "sellPrice");
    m_upgradePrice = JsonManager::getInstance().getInt(configData, "upgradePrice");
    m_maxLevel = JsonManager::getInstance().getInt(configData, "maxLevel");

    m_projectileType = JsonManager::getInstance().getString(configData, "projectileType");
    m_projectileConfigPath = JsonManager::getInstance().getString(configData, "projectileConfigPath");
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
        else
        {
            m_animationComponent->setDirection(Direction::South);
        }
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

void TurretBase::upgrade(const nlohmann::json& json)
{

}

void TurretBase::sell()
{
    m_markedForRemoval = true;
    std::cout << "Turret sold for: " << m_sellPrice << " gold." << std::endl;
}