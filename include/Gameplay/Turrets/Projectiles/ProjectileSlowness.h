#pragma once

#include <Gameplay/Turrets/Projectiles/ProjectileBase.h>
#include <string>

namespace sf 
{
    class CircleShape;
}

class ProjectileSlowness : public ProjectileBase
{
public:

    ProjectileSlowness(const sf::Vector2f& spawnPosition, const std::string& configPath, 
        unsigned int targetEnemyId, float damageFromTurret);

    ~ProjectileSlowness() override;

    void move(uint32_t deltaTime) override; 
    void hit() override;    
     
    void update(uint32_t deltaMilliseconds) override;

protected:

    float m_areaOfEffectRadius {0.0f};     
    float m_slownessFactor {0.0f};
    float m_slownessDuration {0.0f};
    float m_elapsedTime {0.0f};
    float m_projectileDuration {0.0f};
    
    bool m_reachedTarget {false};

    unsigned int m_targetEnemyId {0};
    sf::Vector2f m_fixedTargetPosition {0.0f, 0.0f};

    std::unique_ptr<sf::CircleShape> m_areaOfEffectShape {nullptr}; //Temporal visualization of the area of effect
};