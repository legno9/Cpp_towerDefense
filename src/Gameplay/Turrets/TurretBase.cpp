#include <Gameplay/Turrets/TurretBase.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <iostream>

TurretBase::TurretBase(const sf::Vector2f& position, const std::string& texturePath, const TurretConfig& config, RenderManager& renderManager)
    : GameObject(position.x, position.y)
    , m_renderManager(renderManager)
{
    try
    {
        m_currentTexture = &AssetManager::getInstance().loadTexture(texturePath);
        m_sprite.setTexture(*m_currentTexture);
        m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
        m_sprite.setPosition(m_position);
    }
    catch (const std::runtime_error& e)
    {
        m_currentTexture = nullptr;
        std::cerr << "Error loading texture for turret: " << e.what() << std::endl;
    }

    applyConfig(config);

    m_actionTimer = 0.0f;
    m_level = 1;

    m_renderManager.addToRenderQueue(m_sprite, ZOrder::Foreground);
}

void TurretBase::applyConfig(const TurretConfig& config)
{
    m_damage = config.damage;
    m_areaDamage = config.areaDamage;
    m_actionRange = config.actionRange;
    m_actionRate = config.actionRate;
    m_buyPrice = config.buyPrice;
    m_sellPrice = config.sellPrice;
    m_upgradePrice = config.upgradePrice;
    m_flags = config.flags;
    m_maxLevel = config.maxLevel;
}

void TurretBase::update(float deltaTime)
{
    m_actionTimer += deltaTime;
    if (m_actionTimer >= m_actionRate)
    {
        action();
        m_actionTimer = 0.0f;
    }
}

void TurretBase::upgrade(const TurretConfig& newLevelConfig)
{

}

void TurretBase::action()
{
    
}