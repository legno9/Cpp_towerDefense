#pragma once

#include <string>
#include <iostream>

class GameManager;

class Player
{
public:
    Player(int startingHealth, int startingGold, GameManager& gameManager);
    ~Player() = default;

    int getCurrentHealth() const { return m_currentHealth; }
    int getMaxHealth() const { return m_maxHealth; }
    int getGold() const { return m_gold; }

    void addGold(int amount);
    void reduceGold(int amount); 

    void receiveDamage(int amount);
    
    void resetStats();

private:

    int m_maxHealth {0};
    int m_currentHealth {0};
    int m_gold {0};
    int m_startingGold {0};

    GameManager& m_gameManager;
};