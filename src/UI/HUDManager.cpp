#include <UI/HUDManager.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <Core/RenderManager.h> 
#include <Core/AssetManager.h> 
#include <Core/GameObjectManager.h>  
#include <Core/GameManager.h>
#include <UI/TextLabel.h>
#include <UI/TowerSelectorUI.h>

HUDManager* HUDManager::s_instance = nullptr;

void HUDManager::initialize(GameManager& gameManager) 
{
    if (s_instance == nullptr) 
    {
        s_instance = new HUDManager(gameManager);
    } 
    else 
    {
        std::cerr << "WARNING: RenderManager::initialize() called more than once." << std::endl;
    }
}

HUDManager::HUDManager(GameManager& gameManager)
    : m_gameManager(gameManager)
{
    sf::Font& gameFont = AssetManager::getInstance().getDefaultFont();

    m_healthBackground.setSize(sf::Vector2f(85.0f, 25.0f));
    m_healthBackground.setFillColor(sf::Color(50, 50, 50, 200));
    m_healthBackground.setOutlineColor(sf::Color::Black);
    m_healthBackground.setOutlineThickness(1.0f);
    m_healthBackground.setPosition(10.0f, 10.0f);
    RenderManager::getInstance().addToRenderQueue(m_healthBackground, ZOrder::UI);

    m_goldBackground.setSize(sf::Vector2f(85.0f, 25.0f));
    m_goldBackground.setFillColor(sf::Color(50, 50, 50, 200));
    m_goldBackground.setOutlineColor(sf::Color::Black);
    m_goldBackground.setOutlineThickness(1.0f);
    m_goldBackground.setPosition(10.0f, 40.0f);
    RenderManager::getInstance().addToRenderQueue(m_goldBackground, ZOrder::UI);


    if (gameFont.getInfo().family.empty()) 
    {
        std::cerr << "ERROR: HUDManager failed to load default font." << std::endl;
        throw std::runtime_error("Failed to load default font for HUDManager.");
    }

    m_healthLabel = std::make_unique<TextLabel>(sf::Vector2f(15.0f, 15.0f), "Health: 0", sf::Color::Red, 15, gameFont);
    m_goldLabel = std::make_unique<TextLabel>(sf::Vector2f(15.0f, 45.0f), "Gold: 0", sf::Color::Yellow, 15, gameFont);

    m_towerSelectorUI = std::make_unique<TowerSelectorUI>(m_gameManager, sf::Vector2f(100.0f, 10.0f), sf::Vector2f(210.0f, 55.0f));
    m_towerSelectorUI->initializeSelector(GameObjectManager::getInstance().getBuildableTurrets());
}

HUDManager& HUDManager::getInstance() 
{
    if (s_instance == nullptr) 
    {
        std::cerr << "ERROR: RenderManager::getInstance() called before initialize()." << std::endl;
        throw std::runtime_error("RenderManager not initialized!");
    }
    return *s_instance;
}

HUDManager::~HUDManager()
{
    RenderManager::getInstance().removeFromRenderQueue(m_goldBackground, ZOrder::UI);
    RenderManager::getInstance().removeFromRenderQueue(m_healthBackground, ZOrder::UI);
    if (m_goldLabel) 
    {
        m_goldLabel.reset();
    }
    if (m_healthLabel) 
    {
        m_healthLabel.reset();
    } 
    if (m_towerSelectorUI) 
    {
        m_towerSelectorUI.reset();
    }
}

void HUDManager::removeInstance()
{
    if (s_instance != nullptr) 
    {
        delete s_instance;
        s_instance = nullptr;
    } 
    else 
    {
        std::cerr << "WARNING: RenderManager::shutdown() called without instance initialized." << std::endl;
    }
}

void HUDManager::update(uint32_t deltaMiliseconds, const sf::Vector2f mousePos) 
{
    if (m_towerSelectorUI) 
    {
        m_towerSelectorUI->update(deltaMiliseconds, mousePos);
    } 
    else 
    {
        std::cerr << "ERROR: TowerSelectorUI is not initialized." << std::endl;
    }
}

void HUDManager::handleEvent(const sf::Event& event, const sf::RenderWindow& window) 
{
    if (m_towerSelectorUI) 
    {
        m_towerSelectorUI->handleEvent(event, window);
    } 
    else 
    {
        std::cerr << "ERROR: TowerSelectorUI is not initialized." << std::endl;
    }
}

void HUDManager::updateGoldDisplay(int gold) 
{
    if (m_goldLabel) 
    {
        m_goldLabel->setText("Gold: " + std::to_string(gold));
    }
    else 
    {
        std::cerr << "ERROR: Gold label is not initialized." << std::endl;
    } 
}

void HUDManager::updateHealthDisplay(int health) 
{
    if (m_healthLabel) 
    {
        m_healthLabel->setText("Health: " + std::to_string(health));
    } 
    else 
    {
        std::cerr << "ERROR: Health label is not initialized." << std::endl;
    }
}