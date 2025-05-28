#pragma once

#include <Memory>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf
{
    class RenderWindow;
    class RectangleShape;
}

class MapLayer;
class RenderManager;
class GameManager;
class MouseManager
{
    public:

        MouseManager(sf::RenderWindow& window, RenderManager& renderManager, GameManager& gameManager);
        ~MouseManager();

        void update();

        void setTowerLayer(MapLayer* towersLayer);

        bool isMouseOnWindow() const;

    private:
        static constexpr unsigned int m_nullTileID{ 0 };

        RenderManager& m_renderManager;
        GameManager& m_gameManager;
        sf::RenderWindow& m_window;
        
        MapLayer* m_towersLayer;

        std::unique_ptr<sf::RectangleShape> m_tileIndicator{ nullptr };

        sf::Vector2f m_worldPos { 0.f, 0.f };
        sf::Vector2i m_pos{ 0, 0 };
        sf::Vector2u m_currentTileSize{ 0u, 0u };

        sf::Color m_fillIndicatorColor{ 215, 215, 215,  200};
        sf::Color m_outlineIndicatorColor{ 44, 44, 44,  200};
};