#include <Gameplay/Enemies/EnemyBase.h>
#include <iostream>
#include <stdexcept>
#include <Core/JsonManager.h>
#include <Core/AssetManager.h>
#include <Core/AnimationDataManager.h>
#include <Core/RenderManager.h>


EnemyBase::EnemyBase(const sf::Vector2f& initialPosition, const std::string& configPath, const std::vector<sf::Vector2f>& pathPoints)
    : GameObject(initialPosition.x, initialPosition.y),
    m_pathPoints(pathPoints)
{

    nlohmann::json enemyJson = JsonManager::getInstance().loadConfigFile(configPath);

    if (enemyJson.is_null() || enemyJson.empty()) {
        std::cerr << "ERROR: EnemyBase failed to load or parse config: " << configPath << std::endl;
        throw std::runtime_error("Invalid enemy config file: " + configPath);
    }

    applyConfig(enemyJson);

    if (m_pathPoints.size() > 1) {
        for (size_t i = 0; i < m_pathPoints.size() - 1; ++i) 
        {
            sf::Vector2f segmentVector = m_pathPoints[i + 1] - m_pathPoints[i];
            m_totalPathLength += std::hypot(segmentVector.x, segmentVector.y);
        }
    }
    else 
    {
        m_totalPathLength = 0.0f;
    }

    m_sprite = std::make_unique<sf::Sprite>();

    std::string animationPath =  JsonManager::getInstance().getString(enemyJson, "animationDataPath");
    float scale = JsonManager::getInstance().getFloat(enemyJson, "spriteScale");

    m_animationComponent = std::make_unique<AnimationComponent>(*m_sprite, animationPath);
    m_animationComponent->play("walk");

    m_sprite->setOrigin(m_sprite->getLocalBounds().width / 2, m_sprite->getLocalBounds().height / 2);
    m_sprite->setPosition(m_position);
    m_sprite->setScale(scale, scale);

    RenderManager::getInstance().addToRenderQueue(*m_sprite, ZOrder::Foreground);
}

EnemyBase::~EnemyBase()
{
    if (m_sprite) {
        RenderManager::getInstance().removeFromRenderQueue(*m_sprite, ZOrder::Foreground);
    }
}

void EnemyBase::applyConfig(const nlohmann::json& configData)
{
    m_maxHealth = JsonManager::getInstance().getFloat(configData, "health");
    m_currentHealth = m_maxHealth;
    m_predictedHealth = m_maxHealth;
    m_maxSpeed = JsonManager::getInstance().getFloat(configData, "speed");
    m_currentSpeed = m_maxSpeed;
    m_goldValue = JsonManager::getInstance().getInt(configData, "goldValue");
}

void EnemyBase::update(uint32_t deltaMilliseconds)
{
    if (m_markedForRemoval) return;
    
    move(deltaMilliseconds);

    if (m_animationComponent)
    {
        m_animationComponent->update(deltaMilliseconds);
    }

    if (m_sprite) 
    { 
        m_sprite->setPosition(m_position);
    }

    if (m_currentHealth <= 0) 
    {
        m_markedForRemoval = true;
        die();
    }

    if (m_speedReductionDuration > 0.0f) 
    {
        m_speedReductionDuration -= static_cast<float>(deltaMilliseconds) / 1000.0f;
        if (m_speedReductionDuration <= 0.0f) 
        {
            m_currentSpeed = m_maxSpeed;
        }
    }
}

void EnemyBase::move(uint32_t deltaMilliseconds)
{
    if (m_pathPoints.empty()) return;

    float deltaSeconds = static_cast<float>(deltaMilliseconds) / 1000.0f;
    float distanceToMove = m_currentSpeed * deltaSeconds;
    sf::Vector2f lastMovementDirection = sf::Vector2f(0.0f, 0.0f);

    while (distanceToMove > 0.0f && m_currentPathIndex < m_pathPoints.size())
    {
        if (m_currentPathIndex >= m_pathPoints.size() - 1)
        {
            m_position = m_pathPoints.back();
            m_distanceCoveredTotal = m_totalPathLength; 
            m_markedForRemoval = true;
            return;
        }

        sf::Vector2f startPoint = m_pathPoints[m_currentPathIndex];
        sf::Vector2f endPoint = m_pathPoints[m_currentPathIndex + 1];

        sf::Vector2f vectorEndPoint = endPoint - m_position;
        float distanceToEnd = std::hypot(vectorEndPoint.x, vectorEndPoint.y);

        if (distanceToEnd < 0.001f)
        {
            m_currentPathIndex++;
            m_position = endPoint;
            continue;
        }

        sf::Vector2f normalizedDirection = vectorEndPoint / distanceToEnd;

        if (distanceToMove >= distanceToEnd)
        {
            m_position = endPoint;
            distanceToMove -= distanceToEnd;
            m_distanceCoveredTotal += distanceToEnd;
            m_currentPathIndex++;
            lastMovementDirection = normalizedDirection;
        }
        else
        {
            m_position += normalizedDirection * distanceToMove;
            m_distanceCoveredTotal += distanceToMove;
            lastMovementDirection = normalizedDirection;
            distanceToMove = 0.0f;
            break;
        }
    }

    if (m_animationComponent) 
    {
        if (std::hypot(lastMovementDirection.x, lastMovementDirection.y) > 0.001f)
        {
            m_animationComponent->SetDirectionFromVector(lastMovementDirection);
            m_animationComponent->play("walk");
        }
        else 
        {
            m_animationComponent->play("idle");
        }
    }
}

void EnemyBase::receiveDamage(float damage)
{
    m_currentHealth -= damage;
    if (m_currentHealth <= 0) 
    {
        m_currentHealth = 0;
        m_markedForRemoval = true;
        die();

    }
    //Hit effect
}

void EnemyBase::reduceSpeed(float reductionFactor, float speedReductionDuration)
{
    if (m_maxSpeed * (1.0f - reductionFactor) <= m_currentSpeed)
    {
        m_currentSpeed *= (1.0f - reductionFactor);
        m_speedReductionDuration = speedReductionDuration;
    }  
}

void EnemyBase::die()
{
    std::cout << "Enemy died! Awarded " << m_goldValue << " gold." << std::endl;
    
}

void EnemyBase::predictDamage(float damageAmount)
{
    m_predictedHealth -= damageAmount;
}

