#include <Gameplay/World.h>
#include <iostream>
#include <stdexcept>
#include <Core/JsonManager.h>

World::World(GameObjectManager& gameObjectManager)
    :m_gameObjectManager(gameObjectManager)
{
    try 
	{
        loadLevelIndex("data/Config/Levels/levels_index.json");
    } 
	catch (const std::runtime_error& e) 
	{
        std::cerr << "FATAL ERROR: Failed to load level index: " << e.what() << std::endl;
    }
}

World::~World()
{
}

void World::loadLevelIndex(const std::string& indexFilePath) 
{
	nlohmann::json indexJson = JsonManager::getInstance().loadConfigFile(indexFilePath);

    if (indexJson.is_null() || indexJson.empty()) 
	{
        throw std::runtime_error("Invalid level index file: " + indexFilePath);
    }

    if (indexJson.contains("levelSequence") && indexJson["levelSequence"].is_array()) 
	{
        for (const auto& levelEntry : indexJson["levelSequence"]) 
		{
            std::string id = JsonManager::getInstance().getString(levelEntry, "id");
            std::string configPath = JsonManager::getInstance().getString(levelEntry, "configPath");

            if (!id.empty() && !configPath.empty()) 
			{
                m_levelPaths[id] = configPath;
                m_levelOrder.push_back(id); 
            } 
			else 
			{
                std::cerr << "WARNING: Invalid level entry in index: " << levelEntry.dump() << std::endl;
            }
        }
    } 
	else 
	{
        throw std::runtime_error("Level index file missing 'levelSequence' array: " + indexFilePath);
    }
}

bool World::loadLevel(const std::string& levelId)
{
    m_currentLevel.reset();

    auto it = m_levelPaths.find(levelId);
    if (it == m_levelPaths.end()) 
	{
        std::cerr << "ERROR: Level ID '" << levelId << "' not found in level index." << std::endl;
        return false;
    }
    std::string levelConfigPath = it->second;

    m_currentLevel = std::make_unique<Level>(m_gameObjectManager);
    try 
	{
        m_currentLevel->load(levelConfigPath);

		for (int i = 0; i < m_levelOrder.size(); ++i) 
		{
            if (m_levelOrder[i] == levelId) 
			{
                m_currentLevelIndex = i;
                break;
            }
        }
        return true;
    } 
	catch (const std::runtime_error& e) 
	{
        std::cerr << "ERROR: World failed to load level '" << levelId << "': " << e.what() << std::endl;
        m_currentLevel.reset();
        return false;
    }
}

bool World::loadFirstLevel() 
{
	if (m_levelOrder.empty()) 
	{
		std::cerr << "ERROR: No levels available to load." << std::endl;
		return false;
	}
	return loadLevel(m_levelOrder[0]);
}

void World::update(uint32_t deltaMilliseconds)
{
    if (m_currentLevel) 
	{
        m_currentLevel->update(deltaMilliseconds);

    }
}

bool World::isLevelCompleted() const///////////////
{
    if (m_currentLevel) 
	{
        return m_currentLevel->isLevelCompleted();
    }
    return false;
}

void World::nextLevel() 
{
	if (m_currentLevelIndex + 1 < m_levelOrder.size()) 
	{
        m_currentLevelIndex++;
        loadLevel(m_levelOrder[m_currentLevelIndex]);
    } 
	else
	{
		//End screen
	}
}

void World::resetLevel() 
{
	if (m_currentLevelIndex >= 0 && m_currentLevelIndex < m_levelOrder.size()) 
	{
        loadLevel(m_levelOrder[m_currentLevelIndex]);
    } 
	else 
	{
        std::cerr << "ERROR: Cannot reset level, no valid current level." << std::endl;
    }
}

void World::startCurrentLevelWaves() 
{
	if (m_currentLevel) 
	{
		m_currentLevel->startLevelWaves();
	} 
	else 
	{
		std::cerr << "ERROR: No current level to start waves." << std::endl;
	}
}