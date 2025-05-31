#include <Gameplay/Turrets/Projectiles/ProjectileSlowness.h>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <Core/JsonManager.h>
#include <Core/GameObjectManager.h>
#include <Gameplay/Enemies/EnemyBase.h>


ProjectileSlowness::ProjectileSlowness(const sf::Vector2f& spawnPosition, const std::string& configPath,
                               unsigned int targetEnemyId, float damageFromTurret)
    : ProjectileBase(spawnPosition, configPath, damageFromTurret),
    m_targetEnemyId(targetEnemyId)
{
    nlohmann::json projectileJson = JsonManager::getInstance().loadConfigFile(configPath);

    if (projectileJson.is_null() || projectileJson.empty()) 
    {
        std::cerr << "ERROR: AreaProjectile failed to load or parse config: " << configPath << std::endl;
        throw std::runtime_error("Invalid AreaProjectile config file: " + configPath);
    }

    m_areaOfEffectRadius = JsonManager::getInstance().getFloat(projectileJson, "areaOfEffectRadius");
    m_slownessFactor = JsonManager::getInstance().getFloat(projectileJson, "slownessFactor");
    m_slownessDuration = JsonManager::getInstance().getFloat(projectileJson, "slownessDuration");
    m_projectileDuration = JsonManager::getInstance().getFloat(projectileJson, "projectileDuration");

    GameObject* rawTarget = GameObjectManager::getInstance().getGameObjectById(m_targetEnemyId);
    EnemyBase* targetEnemy = dynamic_cast<EnemyBase*>(rawTarget);

    if (targetEnemy == nullptr || targetEnemy->isMarkedForRemoval())
    {
        m_markedForRemoval = true;
        return;
    }

    m_fixedTargetPosition = targetEnemy->getPosition();
}

void ProjectileSlowness::update(uint32_t deltaMilliseconds)
{
    ProjectileBase::update(deltaMilliseconds);

    if (!m_reachedTarget) {return;}

    float deltaSeconds = static_cast<float>(deltaMilliseconds) / 1000.0f;

    m_elapsedTime += deltaSeconds;

    if (m_elapsedTime >= m_projectileDuration) 
    {
        m_markedForRemoval = true;
        return;
    }

    hit();
}

void ProjectileSlowness::move(uint32_t deltaMilliseconds)
{
    if (m_markedForRemoval || m_reachedTarget) return;

    float deltaSeconds = static_cast<float>(deltaMilliseconds) / 1000.0f;

    sf::Vector2f vectorToTarget = m_fixedTargetPosition - m_position;
    float distanceToTarget = std::hypot(vectorToTarget.x, vectorToTarget.y);

    if (distanceToTarget < m_speed * deltaSeconds)
    {
        m_position = m_fixedTargetPosition;
        m_reachedTarget = true;
        return;
    }

    sf::Vector2f normalizedDirection = vectorToTarget / distanceToTarget;
    m_position += normalizedDirection * m_speed * deltaSeconds;
}

void ProjectileSlowness::hit()
{
    std::vector<unsigned int> enemiesAroundTarget = GameObjectManager::getInstance().getEnemiesInRadius(m_fixedTargetPosition, m_areaOfEffectRadius);

    for (unsigned int affectedEnemyId : enemiesAroundTarget) 
    {
        GameObject* affectedRawTarget = GameObjectManager::getInstance().getGameObjectById(affectedEnemyId);
        EnemyBase* affectedTargetEnemy = dynamic_cast<EnemyBase*>(affectedRawTarget);

        if (!affectedTargetEnemy || affectedTargetEnemy->isMarkedForRemoval()) {continue;}
        affectedTargetEnemy->reduceSpeed(m_slownessFactor, m_slownessDuration);
    }
}