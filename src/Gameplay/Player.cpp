#include <Gameplay/Player.h>
#include <UI/HUDManager.h>
#include <Core/GameManager.h>


Player::Player(int startingHealth, int startingGold, GameManager& gameManager)
    : m_maxHealth(startingHealth),
    m_currentHealth(startingHealth),
    m_gold(startingGold),
    m_startingGold(startingGold),
    m_gameManager(gameManager) 
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
        if (m_currentHealth <= 0) 
        {
            m_currentHealth = 0;
            m_gameManager.resetLevel();
        }
        HUDManager::getInstance().updateHealthDisplay(m_currentHealth);
    }
}

void Player::resetStats() 
{
    m_currentHealth = m_maxHealth;
    m_gold = m_startingGold;
    HUDManager::getInstance().updateGoldDisplay(m_gold);
    HUDManager::getInstance().updateHealthDisplay(m_currentHealth);
}