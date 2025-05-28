#include <Core/MouseManager.h>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Render/SFMLOrthogonalLayer.h>
#include <Core/GameManager.h>
#include <Core/RenderManager.h>

MouseManager::MouseManager(sf::RenderWindow& window, RenderManager& renderManager, GameManager& gameManager)
    : m_window(window),
      m_renderManager(renderManager),
      m_gameManager(gameManager)
{}

MouseManager::~MouseManager()
{
    m_renderManager.removeFromRenderQueue(*m_tileIndicator, ZOrder::SpatialUI);
}

void MouseManager::setTowerLayer(MapLayer* towersLayer)
{
    if (towersLayer == nullptr)
    {
        std::cerr << "MouseManager::setTowerLayer() called with nullptr towersLayer" << std::endl;
        return;
    }

    m_towersLayer = towersLayer;
    m_currentTileSize = static_cast<sf::Vector2u>(m_towersLayer->getMapTileSize());
    
    if (m_tileIndicator == nullptr)
    {
        m_tileIndicator = std::make_unique<sf::RectangleShape>(sf::Vector2f(m_currentTileSize.x, m_currentTileSize.y));
        m_tileIndicator->setFillColor(m_fillIndicatorColor);
        m_tileIndicator->setOutlineColor(m_outlineIndicatorColor);
        m_tileIndicator->setOutlineThickness(3.f);
        m_renderManager.addToRenderQueue(*m_tileIndicator, ZOrder::SpatialUI);
        return;
    }

    if (m_tileIndicator->getSize() != static_cast<sf::Vector2f>(m_currentTileSize))
    {
        m_tileIndicator->setSize(sf::Vector2f(m_currentTileSize.x, m_currentTileSize.y));
    }
    
}

bool MouseManager::isMouseOnWindow() const
{
    return (m_pos.x >= 0 && m_pos.x <= static_cast<int>(m_window.getSize().x) &&
            m_pos.y >= 0 && m_pos.y <= static_cast<int>(m_window.getSize().y));
}

void MouseManager::update()
{
    bool isLeftClicked = false;

	// Check if user closed the window. Should always be checked.
	for (auto event = sf::Event(); m_window.pollEvent(event);)
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
		{
			isLeftClicked = true;
		}
	}
    
    m_pos = sf::Mouse::getPosition(m_window);
    
    if (!isMouseOnWindow()){return;}

    if (m_towersLayer == nullptr)
    {
        std::cerr << "MouseManager::Update() called with nullptr towersLayer" << std::endl;
        return;
    }

    m_worldPos = m_window.mapPixelToCoords(m_pos);

    sf::Vector2u tileCoordinates(m_worldPos.x / m_currentTileSize.x, m_worldPos.y / m_currentTileSize.y);
    tmx::TileLayer::Tile currentTile = m_towersLayer->getTile(tileCoordinates.x, tileCoordinates.y);        

    if (currentTile.ID == m_nullTileID)
    {
        m_tileIndicator->setPosition(tileCoordinates.x * m_currentTileSize.x , tileCoordinates.y * m_currentTileSize.y );
        if (isLeftClicked)
        {
            m_gameManager.onTileClicked(m_worldPos);
        }
    }
    else
    {
        m_tileIndicator-> setPosition(-100.f, -100.f);
    }
}

