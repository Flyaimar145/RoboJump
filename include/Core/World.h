#pragma once

#include <cstdint>

class Player;

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

class World
{
	public:

		~World();

		// TO-DO: Ideally the scene should be read from file.
		bool load();

		// To-Do: Implement a unload()

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		// This is just an example. Think a good way to group the actors of your game. If they need any type of manager, etc...
		Player* m_player{ nullptr };

		// To-Do: This should be in its own class, something like "Level" should work
		tmx::Map* m_map{ nullptr };
		MapLayer* m_layerZero{ nullptr };
		MapLayer* m_layerOne{ nullptr };
		MapLayer* m_layerTwo{ nullptr };
		ObjectLayer* m_groundsLayer{ nullptr };
		ObjectLayer* m_wallsLayer{ nullptr };
		ObjectLayer* m_ceilingsLayer{ nullptr };
		ObjectLayer* m_gemsLayer{ nullptr };
};