#include <Animation/AnimationComponent.h>
#include <iostream>
#include <cmath>
#include <corecrt_math_defines.h>

AnimationComponent::AnimationComponent(sf::Sprite& targetSprite, const std::string& animationConfigPath)
    : m_targetSprite(targetSprite)
{
    m_animationData = &AnimationDataManager::getInstance().getSpriteSheetData(animationConfigPath);
    
    m_spriteSheetTexture = &AnimationDataManager::getInstance().getSpriteSheetTexture(m_animationData->texturePaths[0]);
    m_targetSprite.setTexture(*m_spriteSheetTexture);
}

void AnimationComponent::update(uint32_t deltaMilliseconds)
{
    if (!m_isPlaying || !m_currentAnimation || m_currentAnimation->frames.empty()) return;

    float deltaSeconds = static_cast<float>(deltaMilliseconds) / 1000.0f;
    m_currentTime += deltaSeconds;

    if (m_currentTime >= m_currentAnimation->totalDuration)
    {
        if (m_currentAnimation->loop)
        {
            m_currentTime = fmod(m_currentTime, m_currentAnimation->totalDuration);
            m_lastEventFrameIndex = -1; 
        }
        else
        {
            m_isPlaying = false;
            m_currentTime = m_currentAnimation->totalDuration;
            m_currentFrameIndex = m_currentAnimation->frames.size() - 1;
            
            m_targetSprite.setTextureRect(m_currentAnimation->frames[m_currentFrameIndex].textureRect);

            if (onAnimationEnd)
            {
                onAnimationEnd();
            }

            return;
        }
    }

    float runningTime = 0.0f;
    for (size_t i = 0; i < m_currentAnimation->frames.size(); ++i)
    {
        runningTime += m_currentAnimation->frames[i].duration;
        if (m_currentTime < runningTime)
        {
            m_currentFrameIndex = i;
            break;
        }
        if (i == m_currentAnimation->frames.size() - 1) {
            m_currentFrameIndex = i;
        }
    }

    if (m_currentFrameIndex != m_lastEventFrameIndex && 
        m_currentAnimation->frames[m_currentFrameIndex].eventTrigger) 
    {
        if (onFrameEvent)
        {
            onFrameEvent(m_baseAnimationName, m_currentFrameIndex); 
        }
        m_lastEventFrameIndex = m_currentFrameIndex; 
    }

    m_targetSprite.setTextureRect(m_currentAnimation->frames[m_currentFrameIndex].textureRect);
}

void AnimationComponent::play(const std::string& animationName, bool forceRestart)
{
    std::string actualAnimationName = getAnimationNameForDirection(animationName, m_currentDirection);

    if (m_completeAnimationName == actualAnimationName && m_isPlaying && !forceRestart)
    {
        return;
    }

    auto it = m_animationData->animations.find(actualAnimationName);
    if (it == m_animationData->animations.end())
    {
        std::cerr << "ERROR: Animation '" << actualAnimationName << "' not found for spritesheet " << m_animationData->texturePaths[0] << std::endl;
        m_isPlaying = false;
        m_currentAnimation = nullptr;
        throw std::runtime_error("Failed to find animation: " + actualAnimationName);
    }

    m_currentAnimation = &it->second;
    m_completeAnimationName = actualAnimationName;
    m_baseAnimationName = animationName;
    m_currentTime = 0.0f;
    m_currentFrameIndex = 0;
    m_isPlaying = true;
    m_lastEventFrameIndex = -1;

    if (!m_currentAnimation->frames.empty())
    {
        m_targetSprite.setTextureRect(m_currentAnimation->frames[m_currentFrameIndex].textureRect);
        if (m_currentAnimation->frames[0].eventTrigger) 
        {
            if (onFrameEvent)
            {
                onFrameEvent(m_baseAnimationName, m_currentFrameIndex); 
            }
        }        
    }
    else
    {
        std::cerr << "WARNING: Animation '" << actualAnimationName << "' has no frames." << std::endl;
        m_isPlaying = false;
        throw std::runtime_error("Animation: " + actualAnimationName + " has no frames.");
    }
}

void AnimationComponent::stop()
{
    m_isPlaying = false;
    m_currentTime = 0.0f;
    m_currentFrameIndex = 0;
}

void AnimationComponent::setDirection(Direction direction)
{
    if (m_currentDirection == direction) return;

    m_currentDirection = direction;
    
    if (m_isPlaying && m_currentAnimation) 
    {
        play(m_baseAnimationName, true);
    }
}

void AnimationComponent::SetDirectionFromVector(const sf::Vector2f& vector)
{
    if (vector.x == 0.0f && vector.y == 0.0f) 
    {
        setDirection(Direction::South);
        return;
    }

    float angle = std::atan2(vector.y, vector.x) * 180.0f / M_PI;
    Direction direction;

    if (angle >= -45.0f && angle < 45.0f) 
    {
        direction =  Direction::East;
    } 
    else if (angle >= 45.0f && angle < 135.0f) 
    {
        direction = Direction::South;
    } 
    else if (angle >= 135.0f || angle < -135.0f) 
    { 
        direction = Direction::West;
    } 
    else 
    { 
        direction = Direction::North;
    }

    setDirection(direction);
}

std::string AnimationComponent::getAnimationNameForDirection(const std::string& baseAnimationName, Direction direction) const
{
    std::string directionSuffix;
    switch (direction) {
        case Direction::South: directionSuffix = "south"; break;
        case Direction::North: directionSuffix = "north"; break;
        case Direction::East: directionSuffix = "east"; break;
        case Direction::West: directionSuffix = "west"; break;
        case Direction::None:
        default: return baseAnimationName;
    }
    return baseAnimationName + "_" + directionSuffix;
}

void AnimationComponent::changeConfigPath(const std::string& newConfigPath)
{
    m_animationData = &AnimationDataManager::getInstance().getSpriteSheetData(newConfigPath);
    m_spriteSheetTexture = &AnimationDataManager::getInstance().getSpriteSheetTexture(m_animationData->texturePaths[0]);
    m_targetSprite.setTexture(*m_spriteSheetTexture);

    if (m_isPlaying && m_currentAnimation) 
    {
        play(m_baseAnimationName, true);
    }
}

void AnimationComponent::changeTexture(const std::string& texturePath)
{
    m_spriteSheetTexture = &AnimationDataManager::getInstance().getSpriteSheetTexture(texturePath);
    m_targetSprite.setTexture(*m_spriteSheetTexture);

    if (m_isPlaying && m_currentAnimation) 
    {
        play(m_baseAnimationName, true);
    }
}

