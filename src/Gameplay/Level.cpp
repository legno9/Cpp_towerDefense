#include <Gameplay/Level.h>
#include <iostream>
#include <stdexcept>
#include <tmxlite/Map.hpp>
#include <Render/SFMLOrthogonalLayer.h>
#include <Core/GameObjectManager.h>
#include <Core/RenderManager.h>
#include <Core/JsonManager.h>

void from_json(const nlohmann::json& j, GameObjectType& p) 
{
    std::string typeStr = j.get<std::string>();
    if (typeStr == "BasicEnemy") p = GameObjectType::BasicEnemy;
    else if (typeStr == "InvisibleEnemy") p = GameObjectType::InvisibleEnemy;
    else if (typeStr == "FastEnemy") p = GameObjectType::FastEnemy;
    else 
    {
        std::cerr << "WARNING: Unknown GameObjectType string in JSON: " << typeStr << std::endl;
        p = GameObjectType::None;
    }
}

void from_json(const nlohmann::json& j, EnemySpawn& p) 
{
    p.type = j.at("type").get<GameObjectType>();
    p.configPath = j.at("configPath").get<std::string>();
    p.spawnInterval = j.at("spawnInterval").get<float>();
    p.count = j.at("count").get<int>();

    p.spawnedCount = 0;
    p.currentSpawnTimer = 0.0f;
}

void from_json(const nlohmann::json& j, Wave& p) 
{
    p.waveNumber = j.at("waveNumber").get<int>();
    p.delayBeforeWave = j.at("delayBeforeWave").get<float>();
    p.spawns = j.at("spawns").get<std::vector<EnemySpawn>>();

    p.isStarted = false;
    p.isCompleted = false;
    p.currentWaveTimer = 0.0f;
    p.currentSpawnGroupIndex = 0;
}

Level::Level()
{}

Level::~Level()
{
    GameObjectManager::getInstance().removeAllGameObjects();
    if (m_layerZero) RenderManager::getInstance().removeFromRenderQueue(*m_layerZero, ZOrder::Background);
    if (m_layerOne) RenderManager::getInstance().removeFromRenderQueue(*m_layerOne, ZOrder::Background);
    if (m_layerTwo) RenderManager::getInstance().removeFromRenderQueue(*m_layerTwo, ZOrder::Background);
}

std::vector<sf::Vector2f> Level::extractPointsFromMapLayer(const tmx::Map& map, const std::string& layerName) 
{
    std::vector<sf::Vector2f> path;
    for (const auto& layer : map.getLayers()) 
    {
        if (layer->getType() == tmx::Layer::Type::Object && layer->getName() == layerName) 
        {
            const auto& objectGroup = layer->getLayerAs<tmx::ObjectGroup>();
            for (const auto& object : objectGroup.getObjects()) 
            {
                path.push_back({ object.getPosition().x, object.getPosition().y });
            }
            break;
        }
    }
    if (path.empty()) 
    {
        throw std::runtime_error("No path points found in layer: " + layerName);
    }
    return path;
}

void Level::spawnEnemy(GameObjectType type, const std::string& configPath) 
{
    if (m_enemyPathPoints.empty()) 
    {
        std::cerr << "WARNING: Cannot spawn enemy, enemy path points are not loaded." << std::endl;
        return;
    }
    if (m_enemyPathPoints.size() < 2) 
    {
        std::cerr << "WARNING: Enemy path needs at least two points for off-screen spawn calculation." << std::endl;
        return;
    }

    sf::Vector2f firstPathPoint = m_enemyPathPoints[0];
    sf::Vector2f secondPathPoint = m_enemyPathPoints[1];
    sf::Vector2f initialSegmentDirection = secondPathPoint - firstPathPoint;

    float segmentLength = std::hypot(initialSegmentDirection.x, initialSegmentDirection.y);

    if (segmentLength > 0) 
    {
        initialSegmentDirection /= segmentLength;
    } 
    else 
    {
        initialSegmentDirection = sf::Vector2f(0.0f, -1.0f);
    }

    
    sf::Vector2f offScreenSpawnPos = firstPathPoint - initialSegmentDirection * m_spawnDistanceOffScreen;

    GameObjectManager::getInstance().spawnEnemy(type, offScreenSpawnPos, configPath, m_enemyPathPoints);
}

bool Level::load(const std::string& levelFilePath)
{
    nlohmann::json levelJson = JsonManager::getInstance().loadConfigFile(levelFilePath);

    if (levelJson.is_null() || levelJson.empty()) 
    {
        std::cerr << "ERROR: Level failed to load or parse config: " << levelFilePath << std::endl;
        throw std::runtime_error("Invalid level config file: " + levelFilePath);
    }

    std::string mapFilePath = JsonManager::getInstance().getString(levelJson, "mapFilePath");
    m_enemyPathLayerName = JsonManager::getInstance().getString(levelJson, "enemyPathLayerName");
    m_towerLayerIndex = JsonManager::getInstance().getInt(levelJson, "towerLayerIndex");

    m_map = std::make_unique<tmx::Map>();

    if (!m_map->load(mapFilePath)) 
    {
        std::cerr << "ERROR: Level failed to load map: " << mapFilePath << std::endl;
        throw std::runtime_error("Failed to load map: " + mapFilePath);
    }

    try 
    {
        m_enemyPathPoints = extractPointsFromMapLayer(*m_map, m_enemyPathLayerName);
    } 
    catch (const std::runtime_error& e) 
    {
        std::cerr << "ERROR: " << e.what() << ". Enemy spawning might fail." << std::endl;
        throw std::runtime_error("Failed to load enemy path for level: " + levelFilePath + " - " + e.what());
    }

    try 
    {
        m_waves = levelJson.at("waves").get<std::vector<Wave>>();
    } 
    catch (const nlohmann::json::exception& e) 
    {
        std::cerr << "ERROR: Level failed to parse 'waves' array from " << levelFilePath << ": " << e.what() << std::endl;
        throw std::runtime_error("Failed to parse waves from config: " + levelFilePath + " - " + e.what());
    }

    m_layerZero = std::make_unique<MapLayer>(*m_map, 0);
    m_layerOne = std::make_unique<MapLayer>(*m_map, 1);
    m_layerTwo = std::make_unique<MapLayer>(*m_map, 2);

    if (m_layerZero) RenderManager::getInstance().addToRenderQueue(*m_layerZero, ZOrder::Background);
    if (m_layerOne) RenderManager::getInstance().addToRenderQueue(*m_layerOne, ZOrder::Background);
    if (m_layerTwo) RenderManager::getInstance().addToRenderQueue(*m_layerTwo, ZOrder::Background);
    
    m_currentWaveIndex = 0;
    m_waveDelayTimer = 0.0f;
    m_levelWavesStart = false;
    m_allWavesCompleted = false;

    return true;
}


void Level::update(uint32_t deltaMilliseconds)
{
    if (!m_levelWavesStart || m_allWavesCompleted) return;

    float deltaSeconds = static_cast<float>(deltaMilliseconds) / 1000.0f;

    if (m_currentWaveIndex >= m_waves.size()) 
    {
        m_allWavesCompleted = true;
        return;
    }

    Wave& currentWave = m_waves[m_currentWaveIndex];

    if (!currentWave.isStarted) 
    {
        m_waveDelayTimer += deltaSeconds;
        if (m_waveDelayTimer >= currentWave.delayBeforeWave) 
        {
            currentWave.isStarted = true;
            m_waveDelayTimer = 0.0f;
        }
        return;
    }

    if (currentWave.currentSpawnGroupIndex < currentWave.spawns.size()) 
    {
        EnemySpawn& currentSpawnGroup = currentWave.spawns[currentWave.currentSpawnGroupIndex];

        if (currentSpawnGroup.spawnedCount >= currentSpawnGroup.count) 
        {
            currentWave.currentSpawnGroupIndex++;
            currentSpawnGroup.currentSpawnTimer = 0.0f;
            
            return;
        }

        currentSpawnGroup.currentSpawnTimer += deltaSeconds;
        if (currentSpawnGroup.currentSpawnTimer >= currentSpawnGroup.spawnInterval) 
        {
            spawnEnemy(currentSpawnGroup.type, currentSpawnGroup.configPath);
            currentSpawnGroup.spawnedCount++;
            currentSpawnGroup.currentSpawnTimer = 0.0f;
        }
    } 
    else 
    {
        currentWave.isCompleted = true;
        m_currentWaveIndex++;
        m_waveDelayTimer = 0.0f;
    }
}

MapLayer* Level::getTowerLayer() const
{
    switch (m_towerLayerIndex)
    {
        case 0: return m_layerZero.get();
        case 1: return m_layerOne.get();
        case 2: return m_layerTwo.get();
        default:
            std::cerr << "WARNING: Level::getTowerLayer() called with invalid towerLayerIndex: " << m_towerLayerIndex << std::endl;
            return nullptr;
    }
}