#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <SFML/System/Vector2.hpp>
#include <Utils/json.hpp>
#include <Utils/Common.h>


struct EnemySpawn
{
    GameObjectType type;       
    std::string configPath;
    float spawnInterval; 
    int count;                  

    int spawnedCount = 0;
    float currentSpawnTimer = 0.0f;
};

struct Wave
{
    int waveNumber;
    float delayBeforeWave;      
    std::vector<EnemySpawn> spawns;

    bool isStarted = false;
    bool isCompleted = false;
    float currentWaveTimer = 0.0f;
    int currentSpawnGroupIndex = 0;
};

void from_json(const nlohmann::json& j, GameObjectType& p);
void from_json(const nlohmann::json& j, EnemySpawn& p);
void from_json(const nlohmann::json& j, Wave& p);

namespace tmx
{
	class Map;
}

class MapLayer;

class Level
{
public:
    
    Level();
    ~Level();

    bool load(const std::string& levelFilePath);

    void update(uint32_t deltaMilliseconds);
    
    MapLayer* getTowerLayer() const;

    bool isLevelCompleted() const { return m_allWavesCompleted; }
    void startLevelWaves() { m_levelWavesStart = true; }

private:

    std::unique_ptr<tmx::Map> m_map {nullptr};
    std::unique_ptr<MapLayer> m_layerZero {nullptr};
    std::unique_ptr<MapLayer> m_layerOne {nullptr};
    std::unique_ptr<MapLayer> m_layerTwo {nullptr};

    std::string m_enemyPathLayerName {};
    std::vector<sf::Vector2f> m_enemyPathPoints {};

    std::vector<Wave> m_waves {};
    int m_currentWaveIndex {0};
    float m_waveDelayTimer {0.0f};
    bool m_allWavesCompleted {false};
    bool m_levelWavesStart {false};
    int m_towerLayerIndex{0};
    float m_spawnDistanceOffScreen {50.0f};

    std::vector<sf::Vector2f> extractPointsFromMapLayer(const tmx::Map& map, const std::string& layerName);
    void spawnEnemy(GameObjectType type, const std::string& configPath);
};