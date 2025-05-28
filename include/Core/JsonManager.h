#pragma once

#include <string>
#include <Utils/json.hpp>
#include <iostream>

class JsonManager
{
public:

    static JsonManager& getInstance();

    nlohmann::json loadConfigFile(const std::string& filename);

    std::string getString(const nlohmann::json& jsonData, const std::string& key, const std::string& defaultValue = "");
    float getFloat(const nlohmann::json& jsonData, const std::string& key, float defaultValue = 0.0f);
    int getInt(const nlohmann::json& jsonData, const std::string& key, int defaultValue = 0);
    bool getBool(const nlohmann::json& jsonData, const std::string& key, bool defaultValue = false);

private:

    JsonManager() = default;
    ~JsonManager() = default;

    JsonManager(const JsonManager&) = delete;
    JsonManager& operator=(const JsonManager&) = delete;

    std::map<std::string, nlohmann::json> m_loadedConfigs;
};