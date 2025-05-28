#pragma once

#include <vector>
#include <memory>
#include <SFML/System/Vector2.hpp>
#include <Gameplay/GameObject.h>

class RenderManager;

enum class GameObjectType //Other file
{
    None,
    Enemy,
    ShooterTurret,
};

class GameObjectManager
{
	
	public:
		GameObjectManager(RenderManager& renderManager);
    	~GameObjectManager() = default;

		void updateGameObjects(uint32_t deltaMilliseconds);

		void newGameObject(GameObjectType type, const sf::Vector2f& position);
		void destroyGameObject();

	private:

		RenderManager& m_renderManager;

		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};