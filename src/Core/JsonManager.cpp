#include "Core/JsonManager.h" 
#include <fstream>

JsonManager& JsonManager::getInstance()
{
    static JsonManager instance;
    return instance;
}


nlohmann::json JsonManager::loadConfigFile(const std::string& filePath)
{
    auto it = m_loadedConfigs.find(filePath);

    if (it != m_loadedConfigs.end())
    {
        return it->second;
    }

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "ERROR: DataManager failed to load config file: " << filePath << std::endl;
        throw std::runtime_error("Failed to load config file: " + filePath);
    }

    nlohmann::json jsonData;

    try { file >> jsonData; } 

    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "ERROR: DataManager failed to parse JSON file " << filePath << std::endl;
        throw std::runtime_error("Failed to parse JSON file: " + filePath + " - " + e.what());
    }

    m_loadedConfigs[filePath] = jsonData;
    return m_loadedConfigs[filePath];
}

std::string JsonManager::getString(const nlohmann::json& jsonData, const std::string& key, const std::string& defaultValue)
{
    if (jsonData.contains(key) && jsonData[key].is_string())
    {
        return jsonData[key].get<std::string>();
    }
    return defaultValue;
}

float JsonManager::getFloat(const nlohmann::json& jsonData, const std::string& key, float defaultValue)
{
    if (jsonData.contains(key) && jsonData[key].is_number())
    {
        return jsonData[key].get<float>();
    }
    return defaultValue;
}

int JsonManager::getInt(const nlohmann::json& jsonData, const std::string& key, int defaultValue)
{
    if (jsonData.contains(key) && jsonData[key].is_number_integer())
    {
        return jsonData[key].get<int>();
    }
    return defaultValue;
}

bool JsonManager::getBool(const nlohmann::json& jsonData, const std::string& key, bool defaultValue)
{
    if (jsonData.contains(key) && jsonData[key].is_boolean())
    {
        return jsonData[key].get<bool>();
    }
    // También puedes considerar strings "true"/"false" si lo deseas
    if (jsonData.contains(key) && jsonData[key].is_string()) {
        std::string val = jsonData[key].get<std::string>();
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);
        return (val == "true" || val == "1");
    }
    return defaultValue;
}