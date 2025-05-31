#pragma once

#include <string>
#include <iostream>

class Player
{
public:
    Player(int startingHealth, int startingGold);
    ~Player() = default;

    int getCurrentHealth() const { return m_currentHealth; }
    int getMaxHealth() const { return m_maxHealth; }
    int getGold() const { return m_gold; }

    void addGold(int amount);
    void reduceGold(int amount); 

    void receiveDamage(int amount);
    
    void resetStats(int startingHealth, int startingGold);

private:

    int m_maxHealth {0};
    int m_currentHealth {0};
    int m_gold {0};
};