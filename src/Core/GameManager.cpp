#include <Core/GameManager.h>
#include <cassert>
#include <windows.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/MouseManager.h>
#include <Core/RenderManager.h>
#include <Core/GameObjectManager.h>
#include <Core/JsonManager.h>
#include <Gameplay/World.h>


GameManager::~GameManager() = default;
GameManager::GameManager() = default;

bool GameManager::init(const std::string& configPath)
{
	assert((m_window == nullptr || m_world == nullptr) && "Game is already initialized, we are about to leak memory");

	nlohmann::json configJson = JsonManager::getInstance().loadConfigFile(configPath);

	GameCreateInfo createInfo;

	if (configJson.is_null() || configJson.empty())
    {
        std::cerr << "ERROR: GameCreateInfo failed to load or parse config: " << configPath << std::endl;
        return false;
    }

	try
    {
        createInfo.gameTitle = JsonManager::getInstance().getString(configJson, "gameTitle");
        createInfo.screenWidth = JsonManager::getInstance().getInt(configJson, "screenWidth");
        createInfo.screenHeight = JsonManager::getInstance().getInt(configJson, "screenHeight");
        createInfo.frameRateLimit = JsonManager::getInstance().getInt(configJson, "frameRateLimit");
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "ERROR: GameCreateInfo failed to parse JSON data from " << configPath << ": " << e.what() << std::endl;
        return false;
    }

	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(createInfo.screenWidth, createInfo.screenHeight), createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);
	
	HWND hWnd = m_window->getSystemHandle();
	if (hWnd)
    {
        ShowWindow(hWnd, SW_MAXIMIZE);
    }
	
	RenderManager::initialize(*m_window);
	m_mouseManager = std::make_unique<MouseManager>(*m_window, *this);
	m_gameObjectManager = std::make_unique<GameObjectManager>();
	m_world = std::make_unique<World>(*m_gameObjectManager);

	bool loadOk = false;
    if (m_world) 
	{
        loadOk = m_world->loadFirstLevel();
        if (!loadOk) 
		{
            std::cerr << "FATAL ERROR: Failed to load initial level. Exiting." << std::endl;
            return false;
        }
    } 
	else 
	{
        std::cerr << "FATAL ERROR: World manager not initialized." << std::endl;
        return false;
    }

    if (m_mouseManager && m_world->getCurrentLevel()) 
	{
        MapLayer* towerLayer = m_world->getCurrentLevel()->getTowerLayer();
        if (towerLayer) 
		{
            m_mouseManager->setTowerLayer(towerLayer);
        } 
		else 
		{
            std::cerr << "ERROR: Failed to get tower layer from current level." << std::endl;
            return false;
        }
    } 
	else 
	{
        std::cerr << "ERROR: MouseManager or current Level not initialized to set tower layer." << std::endl;
        return false;
    }
    
    return true;
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

	RenderManager::getInstance().render();

	m_window->display();
}

void GameManager::onTileClicked(sf::Vector2f mousePosition)
{
	assert(m_world != nullptr && "GameManager::onTileClicked() world is nullptr");
	m_gameObjectManager->spawnTurret(GameObjectType::ShooterTurret, static_cast<sf::Vector2f>(mousePosition));
	m_world->startCurrentLevelWaves();
}