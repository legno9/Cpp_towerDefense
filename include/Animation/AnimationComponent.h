#pragma once

#include <functional> 
#include <SFML/Graphics/Sprite.hpp>
#include <Core/AnimationDataManager.h>
#include <Utils/Common.h>

class AnimationComponent
{
public:
    AnimationComponent(sf::Sprite& targetSprite, const std::string& animationConfigPath);

    void update(uint32_t deltaMilliseconds);

    void play(const std::string& animationName, bool forceRestart = false);
    void stop();

    void setDirection(Direction direction);
    void SetDirectionFromVector(const sf::Vector2f& vector);

private:
    sf::Sprite& m_targetSprite; 
    sf::Texture* m_spriteSheetTexture { nullptr }; 

    const SpriteSheetAnimationData* m_animationData { nullptr };
    const Animation* m_currentAnimation { nullptr };

    std::string m_currentAnimationName { };
    bool m_isPlaying { false };
    float m_currentTime { 0.0f };
    int m_currentFrameIndex { 0 };

    Direction m_currentDirection { Direction::None };
    std::string getAnimationNameForDirection(const std::string& baseAnimationName, Direction direction) const;
};