#include <UI/TowerSelectorUI.h>
#include <iostream>
#include <Core/GameManager.h>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <Core/GameObjectManager.h>
#include <UI/HUDManager.h>
#include <UI/TextLabel.h>
#include <UI/Button.h>

TowerSelectorUI::TowerSelectorUI(GameManager& gameManager, const sf::Vector2f& position, const sf::Vector2f& size)
    : m_gameManager(gameManager)
    , m_position(position)
    , m_size(size)
{
    m_backgroundPanel.setSize(size);
    m_backgroundPanel.setPosition(position);
    m_backgroundPanel.setFillColor(sf::Color(50, 50, 50, 200));
    m_backgroundPanel.setOutlineColor(sf::Color::Black);
    m_backgroundPanel.setOutlineThickness(1.0f);

    sf::Texture* sellTexture = nullptr;
    sf::Texture* upgradeTexture = nullptr;

    try 
    {
        sellTexture = &AssetManager::getInstance().loadTexture("./data/User Interface/texturebutton_other_trash_normal.png");
        upgradeTexture = &AssetManager::getInstance().loadTexture("./data/User Interface/upgrade.png");
    } 
    catch (const std::runtime_error& e) 
    {
        std::cerr << "WARNING: TowerSelectorUI: Failed to load sell or upgrade texture" << std::endl;
    }

    RenderManager::getInstance().addToRenderQueue(*this, ZOrder::UI);

    m_upgradeButton = std::make_unique<Button>(sf::Vector2f(position.x + 130, position.y + 12.5), sf::Vector2f(30, 30), "", 12, sf::Color(125,125,125), sf::Color(150,150,150), sf::Color(50,50,50), upgradeTexture);
    m_upgradeButton->setCallback([this]() { onUpgradeButtonClicked(); });
    
    m_sellButton = std::make_unique<Button>(sf::Vector2f(position.x + 170, position.y + 12.5 ), sf::Vector2f(30, 30), "", 12, sf::Color(125,125,125), sf::Color(150,150,150), sf::Color(50,50,50), sellTexture); 
    m_sellButton->setCallback([this]() { onSellButtonClicked(); });
}

TowerSelectorUI::~TowerSelectorUI() 
{
    RenderManager::getInstance().removeFromRenderQueue(*this, ZOrder::UI);
}

void TowerSelectorUI::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(m_backgroundPanel, states); 
}

void TowerSelectorUI::update(uint32_t deltaMiliseconds, const sf::Vector2f mousePos) 
{
    for (auto& button : m_towerButtons) 
    {
        button->update(deltaMiliseconds, mousePos);
    }
    m_sellButton->update(deltaMiliseconds, mousePos);
    m_upgradeButton->update(deltaMiliseconds, mousePos);
}

bool TowerSelectorUI::handleEvent(const sf::Event& event, const sf::RenderWindow& window) 
{
    bool interacted = false;

    for (auto& button : m_towerButtons) 
    {
        interacted = interacted || button->handleEvent(event, window);
    }

    interacted = interacted || m_sellButton->handleEvent(event, window);
    interacted = interacted || m_upgradeButton->handleEvent(event, window);

    return interacted;
}


void TowerSelectorUI::initializeSelector(const std::unordered_map<GameObjectType, std::string>& buildableTowers) 
{
    float currentX = m_position.x + 10.0f;
    float buttonSpacing = 10.0f;
    float buttonSize = 30.0f;

    for (const auto& [type, iconPath] : buildableTowers) 
    {        
        sf::Texture* iconTexture = nullptr;

        try 
        {
            iconTexture = &AssetManager::getInstance().loadTexture(iconPath);
        } 
        catch (const std::runtime_error& e) 
        {
            std::cerr << "WARNING: TowerSelectorUI: Failed to load icon for turret type " << static_cast<int>(type) << " from " << iconPath << ": " << e.what() << std::endl;
        }

        setupTowerButton(type, sf::Vector2f(currentX, m_position.y + 12.5), buttonSize, *iconTexture);
        
        currentX += buttonSize + buttonSpacing;
    }
}

void TowerSelectorUI::setupTowerButton(GameObjectType type, const sf::Vector2f& position, float size, const sf::Texture& iconTexture) 
{
    auto button = std::make_unique<Button>
    (
        position, sf::Vector2f(size, size), "", 12,
        sf::Color(125,125,125), sf::Color(150,150,150), sf::Color(50,50,50),
        &iconTexture
    );

    button->setCallback([this, type]() 
    {
        if (m_selectedTowerType == type) 
        {
            deselectTower();
        } 
        else 
        {
            m_selectedTowerType = type;
            m_gameManager.setPlayerActionState(PlayerActionState::PlacingTower, type);
        }

    });
    m_towerButtons.push_back(std::move(button));
}

void TowerSelectorUI::deselectTower() 
{
    m_selectedTowerType = GameObjectType::None;
    m_gameManager.setPlayerActionState(PlayerActionState::None);
}

void TowerSelectorUI::onUpgradeButtonClicked() 
{
    if (m_upgradeButtonEnabled) 
    {
        m_upgradeButtonEnabled = false;
        m_gameManager.setPlayerActionState(PlayerActionState::None);
        m_selectedTowerType = GameObjectType::None;
    } 
    else 
    {
        m_upgradeButtonEnabled = true;
        m_gameManager.setPlayerActionState(PlayerActionState::UpgradingTower);
        m_selectedTowerType = GameObjectType::None;
        m_sellButtonEnabled = false;
    }
}

void TowerSelectorUI::onSellButtonClicked() 
{
    if (m_sellButtonEnabled) 
    {
        m_sellButtonEnabled = false;
        m_gameManager.setPlayerActionState(PlayerActionState::None);
        m_selectedTowerType = GameObjectType::None;
    } 
    else 
    {
        m_sellButtonEnabled = true;
        m_gameManager.setPlayerActionState(PlayerActionState::SellingTower);
        m_selectedTowerType = GameObjectType::None;
        m_upgradeButtonEnabled = false;
        
    }
}