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

    bool isPlaying() const { return m_isPlaying; }

    void changeConfigPath(const std::string& newConfigPath);
    void changeTexture(const std::string& texturePath);

    std::function<void(const std::string& animationName, int frameIndex)> onFrameEvent;
    std::function<void()> onAnimationEnd;

private:
    sf::Sprite& m_targetSprite; 
    sf::Texture* m_spriteSheetTexture { nullptr }; 

    const SpriteSheetAnimationData* m_animationData { nullptr };
    const Animation* m_currentAnimation { nullptr };

    std::string m_completeAnimationName { };
    std::string m_baseAnimationName { };
    
    bool m_isPlaying { false };
    float m_currentTime { 0.0f };
    int m_currentFrameIndex { 0 };
    int m_lastEventFrameIndex{-1};

    Direction m_currentDirection { Direction::South };
    std::string getAnimationNameForDirection(const std::string& baseAnimationName, Direction direction) const;
};