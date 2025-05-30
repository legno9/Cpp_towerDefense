#pragma once

#include <memory>
#include <string>
#include <vector>
#include <Gameplay/Level.h>

class Level;

class World
{
public:
    World();
    ~World();

    bool loadLevel(const std::string& levelId);
    bool loadFirstLevel();

    void unloadCurrentLevel();

    void update(uint32_t deltaMilliseconds);

    void nextLevel();
    void resetLevel();
    bool isLevelCompleted() const;
    void startCurrentLevelWaves();

    Level* getCurrentLevel() const { return m_currentLevel.get(); }

private:
    std::unique_ptr<Level> m_currentLevel {nullptr};

    std::map<std::string, std::string> m_levelPaths {};
    std::vector<std::string> m_levelOrder {}; 
    int m_currentLevelIndex {0};
    
    void loadLevelIndex(const std::string& indexFilePath);
};

