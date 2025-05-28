#pragma once

#include <string>
#include <memory>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
}

class World;
class MouseManager;
class RenderManager;
class GameObjectManager;

class GameManager
{
	public:

		struct GameCreateInfo //Other file
		{
			std::string gameTitle;
			uint32_t screenWidth;
			uint32_t screenHeight;
			uint32_t frameRateLimit;
		};
		GameManager();
		~GameManager();

		bool init(GameCreateInfo& createInfo);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

		void onTileClicked(sf::Vector2f tileCoordinates);

	private:

		std::unique_ptr<sf::RenderWindow> m_window;
		std::unique_ptr<MouseManager> m_mouseManager;
		std::unique_ptr<RenderManager> m_renderManager;
		std::unique_ptr<World> m_world;
		std::unique_ptr<GameObjectManager> m_gameObjectManager;
};