#pragma once

#include <cstdint>
#include <string>

namespace sf
{
	class RenderWindow;
}

class World;
class MouseManager;
class RenderManager;

class Game
{
	public:

		struct GameCreateInfo
		{
			std::string gameTitle;
			uint32_t screenWidth;
			uint32_t screenHeight;
			uint32_t frameRateLimit;
		};

		~Game();

		bool init(GameCreateInfo& createInfo);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

	private:

		sf::RenderWindow* m_window{ nullptr };
		MouseManager* m_mouseManager{ nullptr };
		RenderManager* m_renderManager{ nullptr };
		World* m_world{ nullptr };
};