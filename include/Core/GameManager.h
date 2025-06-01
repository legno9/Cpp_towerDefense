#pragma once

#include <string>
#include <memory>
#include <SFML/System/Vector2.hpp>
#include <Gameplay/World.h>
#include <Utils/Common.h>

namespace sf
{
	class RenderWindow;
}

class World;
class MouseManager;
class Player;

class GameManager
{
	public:

		struct GameCreateInfo
		{
			std::string gameTitle {};
			uint32_t screenWidth {0};
			uint32_t screenHeight {0};
			uint32_t frameRateLimit {0};
		};
		GameManager();
		~GameManager();

		bool init(const std::string& configPath);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

		void startWaves();

		void setPlayerActionState(PlayerActionState state, GameObjectType turretType = GameObjectType::None);
		
		void createTurret(GameObjectType turretType, const sf::Vector2f& tileCoordinates);
		void upgradeTurret(const sf::Vector2f& tileCoordinates);
		void sellTurret(const sf::Vector2f& tileCoordinates);

		bool isTurretCreated(const sf::Vector2f& tileCoordinates) const;
		void onTileClicked(const sf::Vector2f& tileCoordinates);

		void resetLevel();

	private:

		std::unique_ptr<sf::RenderWindow> m_window {nullptr};
		std::unique_ptr<Player> m_player {nullptr};
		std::unique_ptr<MouseManager> m_mouseManager {nullptr};
		std::unique_ptr<World> m_world {nullptr};

		PlayerActionState m_playerActionState {PlayerActionState::None};
		GameObjectType m_selectedTowerType {GameObjectType::None};
};