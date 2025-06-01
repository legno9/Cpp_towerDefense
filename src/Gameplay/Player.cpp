#include <Gameplay/Player.h>
#include <UI/HUDManager.h>


Player::Player(int startingHealth, int startingGold)
    : m_maxHealth(startingHealth), m_currentHealth(startingHealth), m_gold(startingGold) 
{}


void Player::addGold(int amount) 
{
    if (amount > 0) 
    {
        m_gold += amount;
        HUDManager::getInstance().updateGoldDisplay(m_gold);
    }
}

void Player::reduceGold(int amount) 
{
    if (amount > 0) 
    {
        m_gold -= amount;
        if (m_gold < 0) 
        {
            m_gold = 0;
        }
        HUDManager::getInstance().updateGoldDisplay(m_gold);
    }

}

void Player::receiveDamage(int amount) 
{
    if (amount > 0) 
    {
        m_currentHealth -= amount;
        if (m_currentHealth < 0) 
        {
            m_currentHealth = 0;
            //Lose
        }
        HUDManager::getInstance().updateHealthDisplay(m_currentHealth);
    }
}

void Player::resetStats(int startingHealth, int startingGold) 
{
    m_maxHealth = startingHealth;
    m_currentHealth = startingHealth;
    m_gold = startingGold;
    HUDManager::getInstance().updateGoldDisplay(m_gold);
    HUDManager::getInstance().updateHealthDisplay(m_currentHealth);
}