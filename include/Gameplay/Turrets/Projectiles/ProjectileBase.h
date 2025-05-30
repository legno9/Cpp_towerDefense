#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <Gameplay/GameObject.h>

class EnemyBase;

class ProjectileBase : public GameObject
{
public:
    ProjectileBase(const sf::Vector2f& spawnPosition, const std::string& configPath,
                EnemyBase* targetEnemy, float turretDamage);
    ~ProjectileBase() override;

    void update(uint32_t deltaMilliseconds) override;

    virtual void move(uint32_t deltaTime) = 0;
    virtual void hit() = 0;

protected:
    
    float m_speed {0.0f};
    float m_damage {0.0f};
    EnemyBase* m_targetEnemy {nullptr};

    std::unique_ptr<sf::Sprite> m_sprite {nullptr};
    sf::Texture* m_currentTexture {nullptr};

    bool m_markedForRemoval = false;
    bool isMarkedForRemoval() const override { return m_markedForRemoval; }

    sf::Vector2f getTargetPosition() const;
};