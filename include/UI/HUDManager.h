#pragma once

#include <memory>            
#include <string>            
#include <SFML/Graphics.hpp>

class TextLabel;
class TowerSelectorUI;
class GameManager;

class HUDManager
{
public:
    static HUDManager& getInstance();

    static void initialize(GameManager& m_gameManager);
    static void removeInstance();

    void update(uint32_t deltaMiliseconds, const sf::Vector2f mousePos);
    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void updateGoldDisplay(int gold);
    void updateHealthDisplay(int health);


private:
    HUDManager(GameManager& gameManager); 
    ~HUDManager();
    
    HUDManager(const HUDManager&) = delete;
    HUDManager& operator=(const HUDManager&) = delete;

    sf::RectangleShape m_healthBackground {};
    sf::RectangleShape m_goldBackground {};
    std::unique_ptr<TextLabel> m_goldLabel { nullptr };
    std::unique_ptr<TextLabel> m_healthLabel { nullptr };
    std::unique_ptr<TowerSelectorUI> m_towerSelectorUI{ nullptr};

    GameManager& m_gameManager;

    static HUDManager* s_instance;
};