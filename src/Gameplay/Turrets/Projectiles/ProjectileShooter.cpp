#include <Gameplay/Turrets/Projectiles/ProjectileShooter.h>
#include <iostream>
#include <cmath>
#include <corecrt_math_defines.h>
#include <Core/JsonManager.h>
#include <Gameplay/Enemies/EnemyBase.h>
#include <Core/GameObjectManager.h>


ProjectileShooter::ProjectileShooter(const sf::Vector2f& spawnPosition, const std::string& configPath,
    unsigned int targetEnemyID, float damageFromTurret)
    : ProjectileBase(spawnPosition, configPath, damageFromTurret),
    m_targetEnemyId(targetEnemyID)
{
    nlohmann::json projectileJson = JsonManager::getInstance().loadConfigFile(configPath);

    if (projectileJson.is_null() || projectileJson.empty())
    {
        std::cerr << "ERROR: ProjectileShooter failed to load or parse config: " << configPath << std::endl;
        throw std::runtime_error("Invalid ProjectileShooter config file: " + configPath);
    }
    
    m_rotationSpeed = JsonManager::getInstance().getFloat(projectileJson, "rotationSpeed");
}

void ProjectileShooter::move(uint32_t deltaMiliseconds)
{
    if (m_markedForRemoval) return;

    float deltaSeconds = static_cast<float>(deltaMiliseconds) / 1000.0f;

    GameObject* rawTarget = GameObjectManager::getInstance().getGameObjectById(m_targetEnemyId);
    EnemyBase* targetEnemy = dynamic_cast<EnemyBase*>(rawTarget);

    if (targetEnemy == nullptr || targetEnemy->isMarkedForRemoval())
    {
        m_markedForRemoval = true;
        return;
    }

    sf::Vector2f targetPosition = targetEnemy->getPosition();
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
    GameObject* rawTarget = GameObjectManager::getInstance().getGameObjectById(m_targetEnemyId);
    EnemyBase* targetEnemy = dynamic_cast<EnemyBase*>(rawTarget);

    if (targetEnemy && !targetEnemy->isMarkedForRemoval())
    {
        targetEnemy->receiveDamage(m_damage);
    }
    m_markedForRemoval = true;
}