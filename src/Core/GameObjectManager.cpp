#include "Core/GameObjectManager.h"
#include <cassert>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/RenderManager.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Turrets/TurretShooter.h"

GameObjectManager::GameObjectManager(RenderManager& renderManager)
    : m_renderManager(renderManager)
{}

void GameObjectManager::updateGameObjects(uint32_t deltaMiliseconds)
{
    for (const auto& obj : m_gameObjects)
    {
        obj->update(deltaMiliseconds);
    }
}

void GameObjectManager::newGameObject(GameObjectType type, const sf::Vector2f& position)
{
    std::unique_ptr<GameObject> newObject = nullptr;

    try
    {
        switch (type)
        {
            case GameObjectType::Enemy:
                break;
            case GameObjectType::ShooterTurret:
                newObject = std::make_unique<TurretShooter>(position, "./data/Config/Turrets/turretShooterConfig.json", m_renderManager);
                break;
            case GameObjectType::None:
            default:
                std::cerr << "WARNING: GameObjectManager::newGameObject: Attempted to create unknown GameObjectType: " << static_cast<int>(type) << std::endl;
                break;
        }

        if (newObject != nullptr)
        {
            m_gameObjects.push_back(std::move(newObject));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating new game object: " << e.what() << std::endl;
        return;
    }
}

void GameObjectManager::destroyGameObject()
{

}