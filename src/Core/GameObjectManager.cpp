#include "Core/GameObjectManager.h"
#include <cassert>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/RenderManager.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Turrets/TurretShooter.h"
#include "Gameplay/Enemies/EnemyBasic.h"
// #include "Gameplay/Turrets/Projectiles/ProjectilShooter.h"

GameObjectManager::GameObjectManager()
{}

void GameObjectManager::updateGameObjects(uint32_t deltaMiliseconds)
{
    for (const auto& obj : m_gameObjects)
    {
        obj->update(deltaMiliseconds);
    }

    checkGameObjectsMarkedForRemoval();
}

GameObject* GameObjectManager::spawnEnemy(GameObjectType type, const sf::Vector2f& spawnPosition, 
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
            default:
                std::cerr << "WARNING: GameObjectManager::newGameObject: Attempted to create unknown GameObjectType: " << static_cast<int>(type) << std::endl;
                break;
        }

        if (newObject != nullptr)
        {
            m_gameObjects.push_back(std::move(newObject));
            return m_gameObjects.back().get();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating new game object: " << e.what() << std::endl;
        return nullptr;
    }

    return nullptr;
}

GameObject* GameObjectManager::spawnTurret(GameObjectType type, const sf::Vector2f& mousePosition)
{
    std::unique_ptr<GameObject> newObject = nullptr;

    try
    {
        switch (type)
        {
            case GameObjectType::ShooterTurret:
                newObject = std::make_unique<TurretShooter>(mousePosition, "./data/Config/Turrets/turretShooterConfig.json");
                break;
            default:
                std::cerr << "WARNING: GameObjectManager::spawnTurret: Attempted to create unknown GameObjectType: " << static_cast<int>(type) << std::endl;
                break;
        }

        if (newObject != nullptr)
        {
            m_gameObjects.push_back(std::move(newObject));
            return m_gameObjects.back().get();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating new game object: " << e.what() << std::endl;
        return nullptr;
    }

    return nullptr;
}

GameObject* GameObjectManager::spawnProjectile(GameObjectType projectileType, const sf::Vector2f& spawnPosition,
                                              const std::string& configPath, EnemyBase* targetEnemy, float damage)
{
    std::unique_ptr<GameObject> newObject = nullptr;
    try {
        switch (projectileType) 
        {
            case GameObjectType::ShooterProjectile:
                // newObject = std::make_unique<ProjectilShooter>(spawnPosition, configPath, m_renderManager, targetEnemy, damage);
                break;
            default:
                std::cerr << "WARNING: GameObjectManager::buildProjectile: Unknown or invalid projectileType: " << static_cast<int>(projectileType) << std::endl;
                break;
        }
        if (newObject != nullptr)
        {
            m_gameObjects.push_back(std::move(newObject));
            return m_gameObjects.back().get();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating new game object: " << e.what() << std::endl;
        return nullptr;
    }

    return nullptr;
}

void GameObjectManager::checkGameObjectsMarkedForRemoval()
{
    m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
        [](const std::unique_ptr<GameObject>& obj)
        {
            return obj && obj->isMarkedForRemoval();
        }),
        m_gameObjects.end());
}