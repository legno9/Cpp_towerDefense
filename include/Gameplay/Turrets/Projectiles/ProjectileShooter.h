#pragma once

#include <Gameplay/Turrets/Projectiles/ProjectileBase.h>
#include <SFML/System/Vector2.hpp>

class EnemyBase;

class ProjectileShooter : public ProjectileBase
{
public:
    ProjectileShooter(const sf::Vector2f& spawnPosition, const std::string& configPath,
    float damageFromTurret, EnemyBase* targetEnemy);

    ~ProjectileShooter() override = default;

    void move(uint32_t deltaTime) override;
    void hit() override;                 

protected:
    EnemyBase* m_actualTargetEnemy;
    float m_rotationSpeed; 
};