#pragma once

#include <vector>
#include <memory>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <Gameplay/GameObject.h>
#include <Utils/Common.h>

class RenderManager;

class GameObjectManager
{
	
	public:
		GameObjectManager(RenderManager& renderManager);
    	~GameObjectManager() = default;

		void updateGameObjects(uint32_t deltaMilliseconds);

		GameObject* spawnTurret(GameObjectType type, const sf::Vector2f& position);
		GameObject* spawnEnemy(GameObjectType type, const sf::Vector2f& spawnPosition, 
			const std::string& configPath, const std::vector<sf::Vector2f>& pathPoints);
    
		void checkGameObjectsMarkedForRemoval();

	private:

		RenderManager& m_renderManager;

		std::vector<std::unique_ptr<GameObject>> m_gameObjects {};
};