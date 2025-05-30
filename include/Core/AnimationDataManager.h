#pragma once

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics/Rect.hpp>
#include <Utils/json.hpp>
#include <Core/AssetManager.h>

struct AnimationFrame
{
    sf::IntRect textureRect {0, 0, 0, 0}; 
    float duration {0.0f};         
};

struct Animation
{
    std::string name {};
    std::vector<AnimationFrame> frames {};
    bool loop {false};               
    float totalDuration {0.0f};     
};


struct SpriteSheetAnimationData
{
    std::string texturePath {};
    int defaultFrameWidth {0};   
    int defaultFrameHeight {0};  
    std::map<std::string, Animation> animations {};
};


void from_json(const nlohmann::json& j, AnimationFrame& p);
void from_json(const nlohmann::json& j, Animation& p);
void from_json(const nlohmann::json& j, SpriteSheetAnimationData& p);


class AnimationDataManager
{
public:
    static AnimationDataManager& getInstance();

    const SpriteSheetAnimationData& getSpriteSheetData(const std::string& configPath);
    sf::Texture& getSpriteSheetTexture(const std::string& texturePath);

private:
    AnimationDataManager() = default;
    AnimationDataManager(const AnimationDataManager&) = delete;
    AnimationDataManager& operator=(const AnimationDataManager&) = delete;

    std::map<std::string, SpriteSheetAnimationData> m_loadedAnimationData {};
    std::map<std::string, sf::Texture*> m_loadedSpriteSheetTextures {};
};