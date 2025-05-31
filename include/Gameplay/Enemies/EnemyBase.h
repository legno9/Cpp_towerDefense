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
    virtual void reduceSpeed(float reductionAmount, float speedReductionDuration);
    virtual void die();
    virtual bool isTargetable() const { return true; }

    void predictDamage(float damageAmount);
    bool isPredictedDead() const { return m_predictedHealth <= 0.0f; }
    float getPredictedHealth() const { return m_predictedHealth; }

    float getMaxHealth() const { return m_maxHealth; }
    float getCurrentHealth() const { return m_currentHealth; }

    float getDistanceAlongPath() const {return m_distanceCoveredTotal; }

protected:
    float m_maxHealth { 0.0f };
    float m_currentHealth { 0.0f };
    float m_maxSpeed { 0.0f };
    float m_currentSpeed { 0.0f };
    float m_speedReductionDuration { 0.0f };
    int m_goldValue { 0 };
    int m_playerDamage {1};
    
    float m_distanceCoveredTotal{0.0f};
    float m_totalPathLength{0.0f}; 

    float m_predictedHealth { 0.0f };

    std::unique_ptr<sf::Sprite> m_sprite { nullptr };
    std::unique_ptr<AnimationComponent> m_animationComponent { nullptr };

    std::vector<sf::Vector2f> m_pathPoints { };
    int m_currentPathIndex { 0 };               

    void applyConfig(const nlohmann::json& configData);

    virtual void move(uint32_t deltaTime);
};