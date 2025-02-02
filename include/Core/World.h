#pragma once


class Enemy;

namespace sf
{
	class RenderWindow;
}

namespace tmx
{
	class Map;
}

class MapLayer;
class ObjectLayer;
class RenderManager;
class TurretBase;

class World
{
	public:

		World(RenderManager* renderManager);

		~World();

		// TO-DO: Ideally the scene should be read from file.
		bool load();

		// To-Do: Implement a unload()

		void update(uint32_t deltaMilliseconds);
		
		MapLayer* getTowersLayer() { return m_layerTwo; }

	private:

		// This is just an example. Think a good way to group the actors of your game. If they need any type of manager, etc...
		TurretBase* m_turret{ nullptr };

		// To-Do: This should be in its own class, something like "Level" should work
		RenderManager* m_renderManager{ nullptr };
		tmx::Map* m_map{ nullptr };
		MapLayer* m_layerZero{ nullptr };
		MapLayer* m_layerOne{ nullptr };
		MapLayer* m_layerTwo{ nullptr };
		ObjectLayer* m_collisionLayer{ nullptr };
};

