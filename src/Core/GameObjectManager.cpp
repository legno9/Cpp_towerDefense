#include "Core/GameObjectManager.h"
#include <cassert>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/RenderManager.h"
#include "Core/JsonManager.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Turrets/TurretShooter.h"
#include "Gameplay/Turrets/TurretSlowness.h"
#include "Gameplay/Turrets/TurretAreaDamage.h"
#include "Gameplay/Enemies/EnemyFast.h"
#include "Gameplay/Enemies/EnemyInvisible.h"
#include "Gameplay/Enemies/EnemyBasic.h"
#include "Gameplay/Turrets/Projectiles/ProjectileShooter.h"
#include "Gameplay/Turrets/Projectiles/ProjectileAreaDamage.h"
#include "Gameplay/Turrets/Projectiles/ProjectileSlowness.h"

GameObjectManager& GameObjectManager::getInstance()
{
    static GameObjectManager instance;
    return instance;
}

unsigned int GameObjectManager::addGameObjectToList(std::unique_ptr<GameObject> object)
{
    if (object != nullptr)
    {
        object->setId(m_nextId++);
        GameObject* rawPtr = object.get();
        m_pendingGameObjects.push_back(std::move(object)); 
        m_idToObjectMap[rawPtr->getId()] = rawPtr;
        return m_nextId;
    }
    return 0;
}

void GameObjectManager::registerTurretsPrices(GameObjectType type, const std::string& configPath)
{
    nlohmann::json turretJson = JsonManager::getInstance().loadConfigFile(configPath);
    if (turretJson.is_null() || turretJson.empty()) 
    {
        std::cerr << "ERROR: Failed to load or parse turret config: " << configPath << std::endl;
        return;
    }

    int buyPrice = JsonManager::getInstance().getInt(turretJson, "buyPrice");
    int sellPrice = JsonManager::getInstance().getInt(turretJson, "sellPrice");
    int upgradePrice = JsonManager::getInstance().getInt(turretJson, "upgradePrice");

    m_turretBuyPrices[type] = buyPrice;
    m_turretSellPrices[type] = sellPrice;
    m_turretUpgradePrices[type] = upgradePrice;
}

void GameObjectManager::updateGameObjects(uint32_t deltaMiliseconds)
{
    for (const auto& obj : m_gameObjects)
    {
        if (!obj) 
        {
            std::cerr << "CRITICAL WARNING: GameObjectManager::update: Found a nullptr unique_ptr in m_gameObjects. Skipping update." << std::endl;
            continue;
        }
        obj->update(deltaMiliseconds);
    }

    if (!m_pendingGameObjects.empty()) 
    {
        m_gameObjects.insert(m_gameObjects.end(),
                             std::make_move_iterator(m_pendingGameObjects.begin()),
                             std::make_move_iterator(m_pendingGameObjects.end()));
        m_pendingGameObjects.clear();
    }

    checkGameObjectsMarkedForRemoval();
}

unsigned int GameObjectManager::spawnEnemy(GameObjectType type, const sf::Vector2f& spawnPosition, 
			const std::string& configPath, const std::vector<sf::Vector2f>& pathPoints)
{
    std::unique_ptr<GameObject> newObject = nullptr;

    try
    {
        switch (type)
        {
            case GameObjectType::BasicEnemy:
                newObject = std::make_unique<EnemyBasic>(spawnPosition, configPath, pathPoints);
                break;
            case GameObjectType::FastEnemy:
                newObject = std::make_unique<EnemyFast>(spawnPosition, configPath, pathPoints);
                break;
            case GameObjectType::InvisibleEnemy:
                newObject = std::make_unique<EnemyInvisible>(spawnPosition, configPath, pathPoints);
                break;
            default:
                std::cerr << "WARNING: GameObjectManager::newGameObject: Attempted to create unknown GameObjectType: " << static_cast<int>(type) << std::endl;
                break;
        }

        if (newObject != nullptr)
        {
            return addGameObjectToList(std::move(newObject));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating new game object: " << e.what() << std::endl;
        return 0;
    }

    return 0;
}

unsigned int GameObjectManager::spawnTurret(GameObjectType type, const sf::Vector2f& mousePosition)
{
    std::unique_ptr<GameObject> newObject = nullptr;

    try
    {
        switch (type)
        {
            case GameObjectType::ShooterTurret:
                newObject = std::make_unique<TurretShooter>(mousePosition, "./data/Config/Turrets/turretShooterConfig.json");
                break;
            case GameObjectType::AreaDamageTurret:
                newObject = std::make_unique<TurretAreaDamage>(mousePosition, "./data/Config/Turrets/turretAreaDamageConfig.json");
                break;
            case GameObjectType::SlownessTurret:
                newObject = std::make_unique<TurretSlowness>(mousePosition, "./data/Config/Turrets/turretSlownessConfig.json");
                break;
            default:
                std::cerr << "WARNING: GameObjectManager::spawnTurret: Attempted to create unknown GameObjectType: " << static_cast<int>(type) << std::endl;
                break;
        }

        if (newObject != nullptr)
        {
            return addGameObjectToList(std::move(newObject));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating new game object: " << e.what() << std::endl;
        return 0;
    }

    return 0;
}

unsigned int GameObjectManager::spawnProjectile(GameObjectType projectileType, const sf::Vector2f& spawnPosition,
                                              const std::string& configPath, unsigned int targetEnemyId, float damage)
{
    std::unique_ptr<GameObject> newObject = nullptr;
    try {
        switch (projectileType) 
        {
            case GameObjectType::ShooterProjectile:
                newObject = std::make_unique<ProjectileShooter>(spawnPosition, configPath, targetEnemyId, damage);
                break;
            case GameObjectType::AreaDamageProjectile:
                newObject = std::make_unique<ProjectileAreaDamage>(spawnPosition, configPath, targetEnemyId, damage);
                break;
            case GameObjectType::SlownessProjectile:
                newObject = std::make_unique<ProjectileSlowness>(spawnPosition, configPath, targetEnemyId, damage);
                break;
            default:
                std::cerr << "WARNING: GameObjectManager::buildProjectile: Unknown or invalid projectileType: " << static_cast<int>(projectileType) << std::endl;
                break;
        }
        if (newObject != nullptr)
        {
            return addGameObjectToList(std::move(newObject));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating new game object: " << e.what() << std::endl;
        return 0;
    }

    

    return 0;
}

void GameObjectManager::checkGameObjectsMarkedForRemoval()
{
    m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
        [this](const std::unique_ptr<GameObject>& obj)
        {
            if (obj && obj->isMarkedForRemoval())
            {
                m_idToObjectMap.erase(obj->getId()); 
                return true;
            }
            return false;
        }),
        m_gameObjects.end());
}

void GameObjectManager::removeAllGameObjects()
{
    m_gameObjects.clear();
    m_pendingGameObjects.clear();
    m_idToObjectMap.clear();
}

std::vector<unsigned int> GameObjectManager::getEnemiesInRadius(const sf::Vector2f& center, float radius) const
{
    std::vector<unsigned int> enemiesInRadius;
    float radiusSq = radius * radius;
    
    for (const auto& obj : m_gameObjects) 
    {
        if (obj->isMarkedForRemoval()) continue;

        EnemyBase* enemy = dynamic_cast<EnemyBase*>(obj.get());
        if (enemy) 
        {
            sf::Vector2f enemyPos = enemy->getPosition();
            float distSq = std::hypot(enemyPos.x - center.x, enemyPos.y - center.y);
            distSq *= distSq;

            if (distSq <= radiusSq) 
            { 
                enemiesInRadius.push_back(enemy->getId());
            }
        }
    }
    
    return enemiesInRadius;
}

GameObject* GameObjectManager::getGameObjectById(unsigned int id) const
{
    auto it = m_idToObjectMap.find(id);
    if (it != m_idToObjectMap.end())
    {
        return it->second;
    }
    return nullptr;
}

TurretBase* GameObjectManager::getTurretByPos(sf::Vector2f position) const
{
    for (const auto& obj : m_gameObjects)
    {
        if (obj->isMarkedForRemoval()) continue;

        TurretBase* turret = dynamic_cast<TurretBase*>(obj.get());
        if (turret && turret->getPosition() == position)
        {
            return turret;
        }
    }
    return nullptr;
}

void GameObjectManager::setPlayer(Player* player)
{
    if (player && m_player == nullptr)
    {
        m_player = player;
    }
    else
    {
        std::cerr << "WARNING: GameObjectManager::setPlayer called with errors." << std::endl;
    }
}

int GameObjectManager::getTurretBuyPrice(GameObjectType type) const
{
    auto it = m_turretBuyPrices.find(type);
    if (it != m_turretBuyPrices.end())
    {
        return it->second;
    }
    return 0; 
}

int GameObjectManager::getTurretSellPrice(GameObjectType type) const
{
    auto it = m_turretSellPrices.find(type);
    if (it != m_turretSellPrices.end())
    {
        return it->second;
    }
    return 0; 
}

int GameObjectManager::getTurretUpgradePrice(GameObjectType type) const
{
    auto it = m_turretUpgradePrices.find(type);
    if (it != m_turretUpgradePrices.end())
    {
        return it->second;
    }
    return 0; 
}