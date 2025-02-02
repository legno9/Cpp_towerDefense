#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <Core/World.h>
#include <Gameplay/Turrets/TurretShooter.h>
#include <Gameplay/Zombie.h>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>


World::World(RenderManager* renderManager)
{
	m_renderManager = renderManager;
}

World::~World()
{
	delete m_layerZero;
	delete m_layerOne;
	delete m_layerTwo;
	delete m_map;
}

bool World::load()
{
	constexpr float millisecondsToSeconds = 1 / 1000.f;

	const bool initok = true;

	// m_turret = new TurretShooter(m_renderManager); // In progress


	// To-Do, Load level: this should have its own class
	m_map = new tmx::Map();
	m_map->load("./Data/Levels/BaseLevel.tmx");
	m_layerZero = new MapLayer(*m_map, 0);
	m_layerOne = new MapLayer(*m_map, 1);
	m_layerTwo = new MapLayer(*m_map, 2);

	m_renderManager->addToRenderQueue(m_layerZero, ZOrder::Background);
	m_renderManager->addToRenderQueue(m_layerOne, ZOrder::Background);
	m_renderManager->addToRenderQueue(m_layerTwo, ZOrder::Background);
	
	return initok;
}

void World::update(uint32_t deltaMilliseconds)
{
	// m_turret->update(deltaMilliseconds);
}