#include <Gameplay/Turrets/Projectiles/ProjectileAreaDamage.h>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <Core/JsonManager.h>
#include <Core/GameObjectManager.h>
#include <Gameplay/Enemies/EnemyBase.h>


ProjectileAreaDamage::ProjectileAreaDamage(const sf::Vector2f& spawnPosition, const std::string& configPath,
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

    GameObject* rawTarget = GameObjectManager::getInstance().getGameObjectById(m_targetEnemyId);
    EnemyBase* targetEnemy = dynamic_cast<EnemyBase*>(rawTarget);

    if (targetEnemy == nullptr || targetEnemy->isMarkedForRemoval())
    {
        m_markedForRemoval = true;
        return;
    }

    m_fixedTargetPosition = targetEnemy->getPosition();
}

void ProjectileAreaDamage::move(uint32_t deltaMilliseconds)
{
    if (m_markedForRemoval) return;

    float deltaSeconds = static_cast<float>(deltaMilliseconds) / 1000.0f;

    sf::Vector2f vectorToTarget = m_fixedTargetPosition - m_position;
    float distanceToTarget = std::hypot(vectorToTarget.x, vectorToTarget.y);

    if (distanceToTarget < m_speed * deltaSeconds)
    {
        m_position = m_fixedTargetPosition;
        hit();
        return;
    }

    sf::Vector2f normalizedDirection = vectorToTarget / distanceToTarget;
    m_position += normalizedDirection * m_speed * deltaSeconds;
}

void ProjectileAreaDamage::hit()
{
    std::vector<unsigned int> enemiesAroundTarget = GameObjectManager::getInstance().getEnemiesInRadius(m_fixedTargetPosition, m_areaOfEffectRadius);

    for (unsigned int affectedEnemyId : enemiesAroundTarget) 
    {
        GameObject* affectedRawTarget = GameObjectManager::getInstance().getGameObjectById(affectedEnemyId);
        EnemyBase* affectedTargetEnemy = dynamic_cast<EnemyBase*>(affectedRawTarget);

        if (!affectedTargetEnemy || affectedTargetEnemy->isMarkedForRemoval()) {continue;}
        affectedTargetEnemy->receiveDamage(m_damage);
    }

    m_markedForRemoval = true;
}