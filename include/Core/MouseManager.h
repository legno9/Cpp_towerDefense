#pragma once

#include <Memory>
#include <map>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <Utils/Common.h>

namespace sf
{
    class RenderWindow;
    class RectangleShape;
    class color;
}

class MapLayer;
class GameManager;
class Player;

class MouseManager
{
    public:

        MouseManager(sf::RenderWindow& window, GameManager& gameManager, Player& player);
        ~MouseManager();

        void update();

        void setTowerLayer(MapLayer* towersLayer);

        bool isMouseOnWindow() const;

        sf::Vector2f getMousePosition() const { return m_worldPos; }
        sf::Vector2f getMouseTilePosition() const { return m_mouseTilePos; }

    private:
        static constexpr unsigned int m_nullTileID{ 0 };

        GameManager& m_gameManager;
        sf::RenderWindow& m_window;
        Player& m_player;
        
        MapLayer* m_towersLayer { nullptr };
        
        std::unique_ptr<sf::RectangleShape> m_tileIndicator{ nullptr };

        sf::Vector2f m_worldPos { 0.f, 0.f };
        sf::Vector2i m_pos{ 0, 0 };
        sf::Vector2u m_currentTileSize{ 0u, 0u };
        sf::Vector2f m_mouseTilePos { 0.f, 0.f };

        sf::Color m_fillIndicatorColor{ 215, 215, 215,  200};
        sf::Color m_outlineIndicatorColor{ 44, 44, 44,  200};
};