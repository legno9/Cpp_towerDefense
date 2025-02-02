#include <cassert>
#include <Core/MouseManager.h>
#include <Core/RenderManager.h>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

MouseManager::MouseManager(sf::RenderWindow* window, RenderManager* renderManager)
{
    assert(window != nullptr && renderManager != nullptr && "MouseManager::MouseManager() window is nullptr");
    m_window = window;
    m_renderManager = renderManager;
}

MouseManager::~MouseManager()
{
    delete m_tileIndicator;
    m_tileIndicator = nullptr;
}

void MouseManager::setTowerLayer(MapLayer* towersLayer)
{
    assert(towersLayer != nullptr && "MouseManager::setTowerLayer() towersLayer or window is nullptr"); 

    m_towersLayer = towersLayer;
    m_currentTileSize = static_cast<sf::Vector2u>(m_towersLayer->getMapTileSize());
    
    if (m_tileIndicator == nullptr )
    {
        m_tileIndicator = new sf::RectangleShape(sf::Vector2f(m_currentTileSize.x, m_currentTileSize.y));
        m_tileIndicator->setFillColor(m_fillIndicatorColor);
        m_tileIndicator->setOutlineColor(m_outlineIndicatorColor);
        m_tileIndicator->setOutlineThickness(3.f);
        m_renderManager->addToRenderQueue(m_tileIndicator, ZOrder::SpatialUI);
        return;
    }

    if (m_tileIndicator->getSize() != static_cast<sf::Vector2f>(m_currentTileSize))
    {
        m_tileIndicator->setSize(sf::Vector2f(m_currentTileSize.x, m_currentTileSize.y));
    }
    
}

bool MouseManager::isMouseOnWindow() const
{
    assert(m_window != nullptr && "MouseManager::MouseManager() window is nullptr");

    return (m_pos.x >= 0 && m_pos.x <= static_cast<int>(m_window->getSize().x) &&
            m_pos.y >= 0 && m_pos.y <= static_cast<int>(m_window->getSize().y));
}

void MouseManager::update()
{
    assert(m_towersLayer != nullptr && m_window != nullptr &&"MouseManager::update() towersLayer or window is nullptr"); 
    
    m_pos = sf::Mouse::getPosition(*m_window);

    if (!isMouseOnWindow()){return;}

    m_worldPos = m_window->mapPixelToCoords(m_pos);

    sf::Vector2u tileCoordinates(m_worldPos.x / m_currentTileSize.x, m_worldPos.y / m_currentTileSize.y);
    tmx::TileLayer::Tile currentTile = m_towersLayer->getTile(tileCoordinates.x, tileCoordinates.y);        

    if (currentTile.ID == m_nullTileID)
    {
        m_tileIndicator->setPosition(tileCoordinates.x * m_currentTileSize.x , tileCoordinates.y * m_currentTileSize.y );
    }
    else
    {
        m_tileIndicator-> setPosition(-100.f, -100.f);
    }
}

