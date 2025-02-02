#include <Core/AssetManager.h>
#include <Core/RenderManager.h>
#include <Core/World.h>
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

	// To-Do, read ALL from file, this is just a quick example to understand that here is where entities are created but consider grouping/managing actors in a smarter way
	// sf::Texture* zombieTexture = AssetManager::getInstance()->loadTexture("./data/Images/Enemies/zombie.png");
	// Zombie::ZombieDescriptor zombieDescriptor;
	// zombieDescriptor.texture = zombieTexture;
	// zombieDescriptor.position = { 50.f, 50.f };
	// zombieDescriptor.speed = { 400.f * millisecondsToSeconds, .0f }; // 400 units per second, or 0.4 units per millisecond, using the latter so it's in alignment with delta time
	// zombieDescriptor.tileWidth = 192.f;
	// zombieDescriptor.tileHeight = 256.f;
	// Zombie* zombie = new Zombie();
	// const bool initOk = zombie->init(zombieDescriptor);
	const bool initok = true;

	// m_enemy = zombie;
	// zombie->setPosition({ .0f, 50.f });

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
	// To-Do: update level

	// Update actors

	// Check for collisions (We could do it in a function here or have a collision manager if it gets complex)
}