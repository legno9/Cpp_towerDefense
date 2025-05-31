#include <Gameplay/Turrets/Projectiles/ProjectileBase.h>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <Core/JsonManager.h>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>

ProjectileBase::ProjectileBase(const sf::Vector2f& spawnPosition, const std::string& configPath, float damageFromTurret)
    :GameObject(spawnPosition.x, spawnPosition.y), 
    m_damage(damageFromTurret)
{
    nlohmann::json projectileJson = JsonManager::getInstance().loadConfigFile(configPath);

    if (projectileJson.is_null() || projectileJson.empty()) 
    {
        std::cerr << "ERROR: ProjectileBase failed to load or parse config: " << configPath << std::endl;
        throw std::runtime_error("Invalid projectile config file: " + configPath);
    }

    m_speed = JsonManager::getInstance().getFloat(projectileJson, "speed");
    std::string texturePath = JsonManager::getInstance().getString(projectileJson, "texturePath");

    m_sprite = std::make_unique<sf::Sprite>();

    m_currentTexture = &AssetManager::getInstance().loadTexture(texturePath);

    m_sprite->setTexture(*m_currentTexture);
    m_sprite->setOrigin(m_sprite->getLocalBounds().width / 2, m_sprite->getLocalBounds().height / 2);
    m_sprite->setPosition(m_position);

    float scale = JsonManager::getInstance().getFloat(projectileJson, "spriteScale");
    m_sprite->setScale(scale, scale);

    RenderManager::getInstance().addToRenderQueue(*m_sprite, ZOrder::Foreground);
}

ProjectileBase::~ProjectileBase()
{
    if (m_sprite) 
    {
        RenderManager::getInstance().removeFromRenderQueue(*m_sprite, ZOrder::Foreground);
    }
}

void ProjectileBase::update(uint32_t deltaMilliseconds)
{
    if (m_markedForRemoval) return;

    move(deltaMilliseconds);

    if (m_sprite) 
    {
        m_sprite->setPosition(m_position);
    }
}

