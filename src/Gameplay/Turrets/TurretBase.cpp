#include <Gameplay/Turrets/TurretBase.h>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <Core/JsonManager.h>

TurretBase::TurretBase(const sf::Vector2f& position, const std::string& configPath, RenderManager& renderManager)
    : GameObject(position.x, position.y)
    , m_renderManager(renderManager)
{
    nlohmann::json turretJson = JsonManager::getInstance().loadConfigFile(configPath);

    if (turretJson.is_null() || turretJson.empty()) 
    {
        std::cerr << "ERROR: Failed to load or parse turret config: " << configPath << std::endl;
        throw std::runtime_error("Invalid turret config file.");
    }

    applyConfig(turretJson);

    m_sprite = std::make_unique<sf::Sprite>();

    std::string animationPath = JsonManager::getInstance().getString(turretJson, "animationDataPath");
    float scale = JsonManager::getInstance().getFloat(turretJson, "spriteScale");
    
    m_animationComponent = std::make_unique<AnimationComponent>(*m_sprite, animationPath);
    m_animationComponent->play("idle");
    m_animationComponent->setDirection(Direction::North);

    m_sprite->setOrigin(m_sprite->getLocalBounds().width / 2, m_sprite->getLocalBounds().height / 2);
    m_sprite->setPosition(m_position);
    m_sprite->setScale(scale,scale);

    m_actionTimer = 0.0f;
    m_level = 1;

    m_renderManager.addToRenderQueue(*m_sprite, ZOrder::Foreground);
}

TurretBase::~TurretBase()
{
    if (m_sprite)
    {
        m_renderManager.removeFromRenderQueue(*m_sprite, ZOrder::Foreground);
    }
}

void TurretBase::applyConfig(const nlohmann::json& configData)
{
    m_damage = JsonManager::getInstance().getFloat(configData, "damage");
    m_areaDamage = JsonManager::getInstance().getFloat(configData, "areaDamage");
    m_actionRange = JsonManager::getInstance().getFloat(configData, "actionRange");
    m_actionRate = JsonManager::getInstance().getFloat(configData, "actionRate");

    m_buyPrice = JsonManager::getInstance().getInt(configData, "buyPrice");
    m_sellPrice = JsonManager::getInstance().getInt(configData, "sellPrice");
    m_upgradePrice = JsonManager::getInstance().getInt(configData, "upgradePrice");
    m_maxLevel = JsonManager::getInstance().getInt(configData, "maxLevel");

    m_flags = TurretFlags::None;
    if (configData.contains("flags") && configData["flags"].is_array()) 
    {
        for (const auto& flagStrJson : configData["flags"]) 
        {
            if (flagStrJson.is_string()) 
            {
                std::string currentFlagStr = flagStrJson.get<std::string>();

                auto it = flagMap.find(currentFlagStr);
               
                if (it == flagMap.end()) 
                {
                    std::cerr << "WARNING: Unknown TurretFlag '" << currentFlagStr << "' in config for " << configData.dump() << std::endl;
                    continue;
                } 
                m_flags |= it->second;
            }
        }
    }
}

void TurretBase::update(uint32_t deltaMilliseconds)
{
    m_animationComponent->update(deltaMilliseconds);

    m_actionTimer += deltaMilliseconds;
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