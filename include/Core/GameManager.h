#pragma once

#include <string>
#include <memory>
#include <SFML/System/Vector2.hpp>
#include <Gameplay/World.h>

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

		void createTurret(sf::Vector2f tileCoordinates, GameObjectType turretType);
		void upgradeTurret(sf::Vector2f tileCoordinates);
		void sellTurret(sf::Vector2f tileCoordinates);
		bool isTurretCreated(sf::Vector2f tileCoordinates) const;

	private:

		std::unique_ptr<sf::RenderWindow> m_window {nullptr};
		std::unique_ptr<MouseManager> m_mouseManager {nullptr};
		std::unique_ptr<World> m_world {nullptr};
		std::unique_ptr<Player> m_player {nullptr};
};