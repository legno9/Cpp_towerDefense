#include <Core/World.h>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>
#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <Gameplay/Turrets/TurretShooter.h>

World::World(RenderManager& renderManager)
	: m_renderManager(renderManager)
{}

World::~World()
{
	m_renderManager.removeFromRenderQueue(*m_layerZero, ZOrder::Background);
	m_renderManager.removeFromRenderQueue(*m_layerOne, ZOrder::Background);
	m_renderManager.removeFromRenderQueue(*m_layerTwo, ZOrder::Background);
}

bool World::load()
{
	constexpr float millisecondsToSeconds = 1 / 1000.f;

	const bool initok = true;

	// To-Do, Load level: this should have its own class
	m_map = std::make_unique<tmx::Map>();
	m_map->load("./data/Levels/BaseLevel.tmx");
	m_layerZero = std::make_unique<MapLayer>(*m_map, 0);
	m_layerOne = std::make_unique<MapLayer>(*m_map, 1);
	m_layerTwo = std::make_unique<MapLayer>(*m_map, 2);

	m_renderManager.addToRenderQueue(*m_layerZero, ZOrder::Background);
	m_renderManager.addToRenderQueue(*m_layerOne, ZOrder::Background);
	m_renderManager.addToRenderQueue(*m_layerTwo, ZOrder::Background);
	
	return initok;
}

void World::update(uint32_t deltaMilliseconds)
{
}