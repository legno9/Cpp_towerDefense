#pragma once

#include <functional> 
#include <SFML/Graphics/Sprite.hpp>
#include <Core/AnimationDataManager.h>

enum class Direction : uint8_t {
    South,
    North,          
    East,         
    West,         
    None
};

class AnimationComponent
{
public:
    AnimationComponent(sf::Sprite& targetSprite, const std::string& animationConfigPath);

    void update(uint32_t deltaMilliseconds);

    void play(const std::string& animationName, bool forceRestart = false);
    void stop();

    void setDirection(Direction direction);

private:
    sf::Sprite& m_targetSprite; 
    sf::Texture* m_spriteSheetTexture; 

    const SpriteSheetAnimationData* m_animationData;
    const Animation* m_currentAnimation;

    std::string m_currentAnimationName;
    bool m_isPlaying;
    float m_currentTime;
    int m_currentFrameIndex;

    Direction m_currentDirection;
    std::string getAnimationNameForDirection(const std::string& baseAnimationName, Direction direction) const;
};