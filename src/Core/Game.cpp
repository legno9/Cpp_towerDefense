#include <cassert>
#include <Core/Game.h>
#include <Core/World.h>
#include <Core/MouseManager.h>
#include <Core/RenderManager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

bool Game::init(GameCreateInfo& createInfo)
{
	assert((m_window == nullptr || m_world == nullptr) && "Game is already initialized, we are about to leak memory");

	m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);
	m_renderManager = new RenderManager(m_window);
	m_world = new World(m_renderManager);
	m_mouseManager = new MouseManager(m_window, m_renderManager);
	const bool loadOk = m_world->load();
	m_mouseManager->setTowerLayer(m_world->getTowersLayer());//Should be on a level initializer
	return loadOk;
}

Game::~Game()
{
	// To-Do: make sure m_world is unloaded()

	delete m_world;
	delete m_window;
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	// Check if user closed the window
	for (auto event = sf::Event(); m_window->pollEvent(event);)
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
		}
	}
	
	m_world->update(deltaMilliseconds);
	m_mouseManager->update();
}

void Game::render()
{
	m_window->clear();

	m_renderManager->render();

	m_window->display();
}