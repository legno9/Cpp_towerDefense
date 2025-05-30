#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <SFML/System/Vector2.hpp>
#include <Gameplay/GameObject.h>
#include <Utils/Common.h>

class EnemyBase;
class TurretBase;
class Player;

class GameObjectManager
{
	public:
		
		static GameObjectManager& getInstance();

		void registerTurretsPrices(GameObjectType type, const std::string& configPath);

		void updateGameObjects(uint32_t deltaMilliseconds);

		unsigned int spawnTurret(GameObjectType type, const sf::Vector2f& position);
		unsigned int spawnEnemy(GameObjectType type, const sf::Vector2f& spawnPosition, 
			const std::string& configPath, const std::vector<sf::Vector2f>& pathPoints);
		unsigned int spawnProjectile(GameObjectType projectileType, const sf::Vector2f& spawnPosition,
            const std::string& configPath, unsigned int targetEnemy, float damage);
    
		void checkGameObjectsMarkedForRemoval();
		void removeAllGameObjects();

		std::vector<unsigned int> getEnemiesInRadius(const sf::Vector2f& center, float radius) const;
		TurretBase* getTurretByPos(sf::Vector2f) const;
		GameObject* getGameObjectById(unsigned int id) const;
		
		int getTurretBuyPrice(GameObjectType type) const;
		int getTurretSellPrice(GameObjectType type) const;
		int getTurretUpgradePrice(GameObjectType type) const;

		void setPlayer(Player* player);
		Player* getPlayer() const { return m_player; }

	private:
		
		GameObjectManager() = default;
    	~GameObjectManager() = default;

		GameObjectManager(const GameObjectManager&) = delete;
    	GameObjectManager& operator=(const GameObjectManager&) = delete;

		std::vector<std::unique_ptr<GameObject>> m_gameObjects {};
		std::vector<std::unique_ptr<GameObject>> m_pendingGameObjects {};

		std::unordered_map<unsigned int, GameObject*> m_idToObjectMap{};
		std::map<GameObjectType, int> m_turretBuyPrices {};
		std::map<GameObjectType, int> m_turretSellPrices {};
		std::map<GameObjectType, int> m_turretUpgradePrices {};

		unsigned int m_nextId{0};
			
		unsigned int addGameObjectToList(std::unique_ptr<GameObject> object);

		Player* m_player {nullptr};
};