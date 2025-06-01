#pragma once

#include <Gameplay/Turrets/Projectiles/ProjectileBase.h>
#include <string>

namespace sf 
{
    class CircleShape;
}

class ProjectileAreaDamage : public ProjectileBase
{
public:

    ProjectileAreaDamage(const sf::Vector2f& spawnPosition, const std::string& configPath, 
        unsigned int targetEnemyId, float damageFromTurret);

    ~ProjectileAreaDamage() override;

    void move(uint32_t deltaTime) override; 
    void hit() override;                 

protected:
    float m_areaOfEffectRadius {0.0f};   
    float m_slownessFactor {0.0f};    
    unsigned int m_targetEnemyId {0};
    sf::Vector2f m_fixedTargetPosition {0.0f, 0.0f};

    std::unique_ptr<sf::CircleShape> m_areaOfEffectShape {nullptr}; //Temporal visualization of the area of effect
};