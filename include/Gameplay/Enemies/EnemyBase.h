#pragma once

#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <SFML/Graphics/Sprite.hpp>
#include <Animation/AnimationComponent.h>
#include <Gameplay/GameObject.h>

class EnemyBase : public GameObject
{
public:
    
    EnemyBase(const sf::Vector2f& initialPosition, const std::string& configPath,const std::vector<sf::Vector2f>& pathPoints);
    ~EnemyBase() override;

    void update(uint32_t deltaTime) override;

    virtual void receiveDamage(float damage);
    virtual void die();

protected:
    float m_maxHealth { 0.0f };
    float m_currentHealth { 0.0f };
    float m_speed { 0.0f };
    int m_goldValue { 0 };

    std::unique_ptr<sf::Sprite> m_sprite { nullptr };
    std::unique_ptr<AnimationComponent> m_animationComponent { nullptr };

    std::vector<sf::Vector2f> m_pathPoints { };
    int m_currentPathIndex { 0 };               

    void applyConfig(const nlohmann::json& configData);

    virtual void move(uint32_t deltaTime);
};