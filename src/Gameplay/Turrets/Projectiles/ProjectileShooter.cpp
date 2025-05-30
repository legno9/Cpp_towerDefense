#include <Gameplay/Turrets/Projectiles/ProjectileShooter.h>
#include <iostream>
#include <cmath>
#include <corecrt_math_defines.h>
#include <Core/JsonManager.h>
#include <Gameplay/Enemies/EnemyBase.h>


ProjectileShooter::ProjectileShooter(const sf::Vector2f& spawnPosition, const std::string& configPath,
    float damageFromTurret, EnemyBase* targetEnemy)
    : ProjectileBase(spawnPosition, configPath, targetEnemy, damageFromTurret),
      m_actualTargetEnemy(targetEnemy)
{
    nlohmann::json projectileJson = JsonManager::getInstance().loadConfigFile(configPath);
    m_rotationSpeed = JsonManager::getInstance().getFloat(projectileJson, "rotationSpeed");
    
    if (m_actualTargetEnemy) 
    {
        sf::Vector2f initialDirection = m_actualTargetEnemy->getPosition() - m_position;
        float angle = std::atan2(initialDirection.y, initialDirection.x) * 180.0f / M_PI;
        if (m_sprite) 
        {
            m_sprite->setRotation(angle);
        }
    }
}

void ProjectileShooter::move(uint32_t deltaTime)
{
    float deltaSeconds = static_cast<float>(deltaTime) / 1000.0f;

    if (m_actualTargetEnemy == nullptr || m_actualTargetEnemy->isMarkedForRemoval())
    {
        m_markedForRemoval = true;
        return;
    }

    sf::Vector2f targetPosition = m_actualTargetEnemy->getPosition();
    sf::Vector2f vectorToTarget = targetPosition - m_position;
    float distanceToTarget = std::hypot(vectorToTarget.x, vectorToTarget.y);

    if (distanceToTarget < m_speed * deltaSeconds)
    {
        m_position = targetPosition;
        hit();
        return;
    }

    sf::Vector2f normalizedDirection = vectorToTarget / distanceToTarget;
    m_position += normalizedDirection * m_speed * deltaSeconds;

    float currentAngle = std::atan2(normalizedDirection.y, normalizedDirection.x) * 180.0f / M_PI;
    if (m_sprite) 
    {
        m_sprite->setRotation(currentAngle);
    }
}

void ProjectileShooter::hit()
{
    if (m_actualTargetEnemy && !m_actualTargetEnemy->isMarkedForRemoval())
    {
        m_actualTargetEnemy->receiveDamage(m_damage);
        std::cout << "HomingProjectile hit enemy! Dealt " << m_damage << " damage." << std::endl;
    }
    m_markedForRemoval = true;
}