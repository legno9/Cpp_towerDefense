#include <Core/GameManager.h>
#include <cassert>
#include <windows.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/MouseManager.h>
#include <Core/RenderManager.h>
#include <Core/GameObjectManager.h>
#include <Core/JsonManager.h>
#include <Gameplay/World.h>
#include <Gameplay/Player.h>
#include <Gameplay/Turrets/TurretBase.h>
#include <UI/HUDManager.h>

GameManager::GameManager() = default;

GameManager::~GameManager()
{
    GameObjectManager::getInstance().removeAllGameObjects();

    if (m_world)
    {
        m_world->unloadCurrentLevel();
        m_world.reset();
    }

    if (m_mouseManager)
    {
        m_mouseManager->setTowerLayer(nullptr);
        m_mouseManager.reset();
    }

    HUDManager::removeInstance();
    
    RenderManager::removeInstance();
};


bool GameManager::init(const std::string& configPath)
{
	assert((m_window == nullptr || m_world == nullptr) && "Game is already initialized, we are about to leak memory");

	nlohmann::json configJson = JsonManager::getInstance().loadConfigFile(configPath);

	if (configJson.is_null() || configJson.empty())
    {
        std::cerr << "ERROR: GameCreateInfo failed to load or parse config: " << configPath << std::endl;
        return false;
    }

    GameCreateInfo createInfo;

    createInfo.gameTitle = JsonManager::getInstance().getString(configJson, "gameTitle");
    createInfo.screenWidth = JsonManager::getInstance().getInt(configJson, "screenWidth");
    createInfo.screenHeight = JsonManager::getInstance().getInt(configJson, "screenHeight");
    createInfo.frameRateLimit = JsonManager::getInstance().getInt(configJson, "frameRateLimit");
    int startingGold = JsonManager::getInstance().getInt(configJson, "startingGold");
    int startingHealth = JsonManager::getInstance().getInt(configJson, "startingHealth");

	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(createInfo.screenWidth, createInfo.screenHeight), createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);
	
	HWND hWnd = m_window->getSystemHandle();
	if (hWnd)
    {
        ShowWindow(hWnd, SW_MAXIMIZE);
    }
	
	RenderManager::initialize(*m_window);
    m_player = std::make_unique<Player>(startingHealth, startingGold); 
	m_mouseManager = std::make_unique<MouseManager>(*m_window, *this , *m_player);
	m_world = std::make_unique<World>();
    GameObjectManager::getInstance().setPlayer(m_player.get());

    if (configJson.contains("turrets") && configJson["turrets"].is_array()) 
    {
        for (const auto& turretEntry : configJson["turrets"]) 
        {
            std::string typeStr = JsonManager::getInstance().getString(turretEntry, "type");
            std::string turretConfigPath = JsonManager::getInstance().getString(turretEntry, "configPath");

            GameObjectType turretType = stringToGameObjectType(typeStr);

            if (turretType != GameObjectType::None && !turretConfigPath.empty()) 
            {
                GameObjectManager::getInstance().registerTurretsPrices(turretType, turretConfigPath);
            } else 
            {
                std::cerr << "WARNING: Invalid buildable turret entry in config: " << turretEntry.dump() << std::endl;
            }
        }
    } else 
    {
        std::cerr << "ERROR: 'buildableTurrets' array not found in buildable_turrets.json." << std::endl;
    }

	if (!m_world->loadFirstLevel()) 
    {
        std::cerr << "FATAL ERROR: Failed to load initial level. Exiting." << std::endl;
        return false;
    }

    if (m_mouseManager && m_world->getCurrentLevel()) 
    {
        MapLayer* towerLayer = m_world->getCurrentLevel()->getTowerLayer();
        if (towerLayer) { m_mouseManager->setTowerLayer(towerLayer); }
        else { std::cerr << "ERROR: Failed to get tower layer from current level." << std::endl; return false; }
    } 
    else 
    { 
        std::cerr << "ERROR: MouseManager or current Level not initialized to set tower layer." << std::endl; return false; 
    }

    HUDManager::initialize(*this);
    HUDManager::getInstance().updateGoldDisplay(m_player->getGold());
    HUDManager::getInstance().updateHealthDisplay(m_player->getCurrentHealth());

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
    HUDManager::getInstance().update(deltaMilliseconds, m_mouseManager->getMousePosition());
	m_world->update(deltaMilliseconds);
	GameObjectManager::getInstance().updateGameObjects(deltaMilliseconds);
    
}

void GameManager::render()
{
	m_window->clear();

	RenderManager::getInstance().render();

	m_window->display();
}

void GameManager::startWaves()
{
	assert(m_world != nullptr && "GameManager::onTileClicked() world is nullptr");
	m_world->startCurrentLevelWaves();
}


void GameManager::createTurret(GameObjectType turretType, const sf::Vector2f& tileCoordinates)
{
    if (GameObjectManager::getInstance().getTurretBuyPrice(turretType) > m_player->getGold())
    {
        std::cerr << "ERROR: Not enough gold to create turret."<< std::endl;
        return;
    }

    m_player->reduceGold(GameObjectManager::getInstance().getTurretBuyPrice(turretType));
	GameObjectManager::getInstance().spawnTurret(turretType, m_mouseManager->getMouseTilePosition());
    
	startWaves();
}

void GameManager::upgradeTurret(const sf::Vector2f& tileCoordinates)
{
    TurretBase* turret = GameObjectManager::getInstance().getTurretByPos(m_mouseManager->getMouseTilePosition());

    if (!turret)
    {
        std::cerr << "ERROR: No turret found " << std::endl;
        return;
    }

    if (turret->isMarkedForRemoval())
    {
        std::cerr << "ERROR: Turret is marked for removal." << std::endl;
        return;
    }

    if (turret->isMaxLevel())
    {
        std::cerr << "ERROR: Turret at position is already at max level." << std::endl;
        return;
    }

    GameObjectType turretType = turret->getType();
    int upgradePrice = GameObjectManager::getInstance().getTurretUpgradePrice(turretType);

    if (upgradePrice > m_player->getGold())
    {
        std::cerr << "ERROR: Not enough gold to upgrade turret "<< std::endl;
        return;
    }

    turret->upgrade();
    m_player->reduceGold(upgradePrice);
}

void GameManager::sellTurret(const sf::Vector2f& tileCoordinates)
{
    TurretBase* turret = GameObjectManager::getInstance().getTurretByPos(m_mouseManager->getMouseTilePosition());
    
    if (turret)
    {
        GameObjectType turretType = turret->getType();
        int sellPrice = GameObjectManager::getInstance().getTurretSellPrice(turretType);
        
        m_player->addGold(sellPrice);
        turret->sell();
    }
    else
    {
        std::cerr << "ERROR: No turret found at mouse position"<< std::endl;
    }
}

void GameManager::setPlayerActionState(PlayerActionState state, GameObjectType turretType)
{
    m_playerActionState = state;

    if (state == PlayerActionState::PlacingTower)
    {
        if (turretType == GameObjectType::None)
        {
            std::cerr << "ERROR: Cannot create turret without specifying type." << std::endl;
            return;
        }

        m_selectedTowerType = turretType;
    }
}

bool GameManager::isTurretCreated(const sf::Vector2f& tileCoordinates) const
{
    TurretBase* turret = GameObjectManager::getInstance().getTurretByPos(tileCoordinates);
    return turret != nullptr;
}

void GameManager::onTileClicked(const sf::Vector2f& tileCoordinates)
{
    if (m_playerActionState == PlayerActionState::PlacingTower)
    {
        if (m_selectedTowerType == GameObjectType::None)
        {
            std::cerr << "ERROR: No turret type selected for placement." << std::endl;
            return;
        }

        if( isTurretCreated(tileCoordinates))
        {
            std::cerr << "ERROR: Turret already exists at tile coordinates." << std::endl;
            return;
        }

        createTurret(m_selectedTowerType, tileCoordinates);
    }
    else if (m_playerActionState == PlayerActionState::UpgradingTower)
    {
        if (!isTurretCreated(tileCoordinates))
        {
            std::cerr << "ERROR: No turret found at tile coordinates for upgrade." << std::endl;
            return;
        }
        upgradeTurret(tileCoordinates);
    }
    else if (m_playerActionState == PlayerActionState::SellingTower)
    {
        if (!isTurretCreated(tileCoordinates))
        {
            std::cerr << "ERROR: No turret found at tile coordinates for selling." << std::endl;
            return;
        }
        sellTurret(tileCoordinates);
    }
}