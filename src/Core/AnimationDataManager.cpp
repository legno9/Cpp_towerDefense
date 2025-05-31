#include <Core/AnimationDataManager.h>
#include <fstream>      
#include <iostream>     
#include <stdexcept>    
#include <Core/JsonManager.h>

void from_json(const nlohmann::json& j, AnimationFrame& p) {
    int row = j.at("row").get<int>();
    int col = j.at("col").get<int>();
    p.duration = j.at("duration").get<float>();
    p.eventTrigger = j.value("eventTrigger", false);
    p.textureRect = sf::IntRect(col, row, 1, 1);
}

void from_json(const nlohmann::json& j, Animation& p) {
    p.name = j.at("name").get<std::string>();
    p.loop = j.at("loop").get<bool>();
    p.frames = j.at("frames").get<std::vector<AnimationFrame>>();

    p.totalDuration = 0.0f;
    for (const auto& frame : p.frames) {
        p.totalDuration += frame.duration;
    }
}

void from_json(const nlohmann::json& j, SpriteSheetAnimationData& p) {
    p.texturePaths = j.at("texturePaths").get<std::vector<std::string>>();
    p.defaultFrameWidth = j.at("defaultFrameWidth").get<int>();
    p.defaultFrameHeight = j.at("defaultFrameHeight").get<int>();
    p.animations = j.at("animations").get<std::map<std::string, Animation>>();

    for (auto& pair : p.animations) {
        for (auto& frame : pair.second.frames) {
            frame.textureRect = sf::IntRect(
                frame.textureRect.left * p.defaultFrameWidth,  
                frame.textureRect.top * p.defaultFrameHeight, 
                p.defaultFrameWidth,
                p.defaultFrameHeight
            );
        }
    }
}

AnimationDataManager& AnimationDataManager::getInstance()
{
    static AnimationDataManager instance;
    return instance;
}

const SpriteSheetAnimationData& AnimationDataManager::getSpriteSheetData(const std::string& configPath)
{
    auto it = m_loadedAnimationData.find(configPath);
    if (it != m_loadedAnimationData.end())
    {
        return it->second;
    }

    nlohmann::json jsonData = JsonManager::getInstance().loadConfigFile(configPath);

    if (jsonData.is_null() || jsonData.empty()) 
    {
        std::cerr << "ERROR: AnimationDataManager failed to load or parse JSON for spritesheet: " << configPath << std::endl;
        throw std::runtime_error("Invalid animation config file: " + configPath);
    }

    SpriteSheetAnimationData data;
    try 
    { 
        data = jsonData.get<SpriteSheetAnimationData>();
    }
    catch (const nlohmann::json::exception& e) 
    {
        std::cerr << "ERROR: AnimationDataManager failed to convert JSON to SpriteSheetAnimationData for " << configPath << ": " << e.what() << std::endl;
        throw std::runtime_error("JSON conversion failed for animation config: " + configPath + " - " + e.what());
    }

    for (const std::string& texturePath : data.texturePaths)
    {
        try 
        {
            sf::Texture& texture = AssetManager::getInstance().loadTexture(texturePath);
            m_loadedSpriteSheetTextures[texturePath] = &texture;
        } 
        catch (const std::runtime_error& e) 
        {
            std::cerr << "ERROR: AnimationDataManager failed to load spritesheet texture '" << texturePath << "' for config " << configPath << ": " << e.what() << std::endl;
            throw std::runtime_error("Failed to load spritesheet texture: " + texturePath + " for config " + configPath + " - " + e.what());
        }
    }

    m_loadedAnimationData[configPath] = std::move(data);
    return m_loadedAnimationData[configPath];
}

sf::Texture& AnimationDataManager::getSpriteSheetTexture(const std::string& texturePath)
{
    auto it = m_loadedSpriteSheetTextures.find(texturePath);
    if (it == m_loadedSpriteSheetTextures.end() || it->second == nullptr)
    {
        std::cerr << "ERROR: Spritesheet texture '" << texturePath << "' not found in AnimationDataManager" << std::endl;
        throw std::runtime_error("Spritesheet texture not cached: " + texturePath);
    }
    return *it->second;
}