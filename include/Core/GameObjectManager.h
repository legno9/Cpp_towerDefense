#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include <Gameplay/GameObject.h>
#include <Utils/Common.h>

class EnemyBase;

class GameObjectManager
{
	public:
		
		static GameObjectManager& getInstance();

		void updateGameObjects(uint32_t deltaMilliseconds);

		GameObject* spawnTurret(GameObjectType type, const sf::Vector2f& position);
		GameObject* spawnEnemy(GameObjectType type, const sf::Vector2f& spawnPosition, 
			const std::string& configPath, const std::vector<sf::Vector2f>& pathPoints);
		GameObject* spawnProjectile(GameObjectType projectileType, const sf::Vector2f& spawnPosition,
            const std::string& configPath, unsigned int targetEnemy, float damage);
    
		void checkGameObjectsMarkedForRemoval();

		void removeAllGameObjects();

		std::vector<unsigned int> getEnemiesInRadius(const sf::Vector2f& center, float radius) const;
		GameObject* getGameObjectById(unsigned int id) const;

	private:
		
		GameObjectManager() = default;
    	~GameObjectManager() = default;

		GameObjectManager(const GameObjectManager&) = delete;
    	GameObjectManager& operator=(const GameObjectManager&) = delete;

		std::vector<std::unique_ptr<GameObject>> m_gameObjects {};
		std::vector<std::unique_ptr<GameObject>> m_pendingGameObjects {};

		std::unordered_map<unsigned int, GameObject*> m_idToObjectMap{};
		unsigned int m_nextId{0};

		GameObject* addGameObjectToList(std::unique_ptr<GameObject> object);
};