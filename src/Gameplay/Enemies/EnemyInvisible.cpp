#include <Gameplay/Enemies/EnemyInvisible.h>  
#include <Core/JsonManager.h>                 

EnemyInvisible::EnemyInvisible(const sf::Vector2f& initialPosition, const std::string& configPath,
                    const std::vector<sf::Vector2f>& pathPoints)

    : EnemyBase(initialPosition, configPath, pathPoints)
{
    nlohmann::json enemyJson = JsonManager::getInstance().loadConfigFile(configPath);

    if (enemyJson.is_null() || enemyJson.empty()) {
        std::cerr << "ERROR: EnemyBase failed to load or parse config: " << configPath << std::endl;
        throw std::runtime_error("Invalid enemy config file: " + configPath);
    }

    m_invisibilityRevealedPath = JsonManager::getInstance().getString(enemyJson, "invisibilityRevealedPath");
}

void EnemyInvisible::receiveDamage(float damage)
{
    if (m_isInvisible) 
    {
        m_isInvisible = false;
        m_animationComponent->changeConfigPath(m_invisibilityRevealedPath);
    }

    EnemyBase::receiveDamage(damage);
}