#include <Core/GameManager.h>
#include <cassert>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/World.h>
#include <Core/MouseManager.h>
#include <Core/RenderManager.h>
#include <Core/GameObjectManager.h>


GameManager::~GameManager() = default;
GameManager::GameManager() = default;

bool GameManager::init(GameCreateInfo& createInfo)
{
	assert((m_window == nullptr || m_world == nullptr) && "Game is already initialized, we are about to leak memory");

	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(createInfo.screenWidth, createInfo.screenHeight), createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);
	
	m_renderManager = std::make_unique<RenderManager>(*m_window);
	m_world = std::make_unique<World>(*m_renderManager);
	m_mouseManager = std::make_unique<MouseManager>(*m_window, *m_renderManager, *this);
	m_gameObjectManager = std::make_unique<GameObjectManager>(*m_renderManager);

	const bool loadOk = m_world->load();
	m_mouseManager->setTowerLayer(m_world->getTowersLayer());//Should be on a level initializer
	return loadOk;
}

bool GameManager::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void GameManager::update(uint32_t deltaMilliseconds)
{
	assert(m_window != nullptr && m_world != nullptr && m_mouseManager != nullptr && "Game::update() window, world or mouseManager is nullptr");

	m_mouseManager->update();
	m_world->update(deltaMilliseconds);
	m_gameObjectManager->updateGameObjects(deltaMilliseconds);
}

void GameManager::render()
{
	m_window->clear();

	m_renderManager->render();

	m_window->display();
}

void GameManager::onTileClicked(sf::Vector2f position)
{
	assert(m_world != nullptr && "GameManager::onTileClicked() world is nullptr");
	m_gameObjectManager->newGameObject(GameObjectType::ShooterTurret, static_cast<sf::Vector2f>(position));
}