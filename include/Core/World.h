#pragma once

#include <memory>

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

		World(RenderManager& renderManager);

		~World();

		bool load();

		// To-Do: Implement a unload()

		void update(uint32_t deltaMilliseconds);
		
		MapLayer* getTowersLayer() { return m_layerTwo.get(); }

	private:

		RenderManager& m_renderManager;

		// To-Do: This should be in its own class, something like "Level" should work
		std::unique_ptr<tmx::Map> m_map{ nullptr };
		std::unique_ptr<MapLayer> m_layerZero{ nullptr };
		std::unique_ptr<MapLayer> m_layerOne{ nullptr };
		std::unique_ptr<MapLayer> m_layerTwo{ nullptr };
};

