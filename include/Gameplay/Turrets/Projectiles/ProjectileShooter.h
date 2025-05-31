#pragma once

#include <Gameplay/Turrets/Projectiles/ProjectileBase.h>
#include <SFML/System/Vector2.hpp>

class ProjectileShooter : public ProjectileBase
{
public:
    ProjectileShooter(const sf::Vector2f& spawnPosition, const std::string& configPath,
    unsigned int targetEnemyId, float damageFromTurret);

    ~ProjectileShooter() override = default;

    void move(uint32_t deltaTime) override;
    void hit() override;                 

protected:
    float m_rotationSpeed {0.0f}; 
    unsigned int m_targetEnemyId {0};
};