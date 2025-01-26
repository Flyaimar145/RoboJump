#pragma once

#include <cstdint>
#include <SFML/Graphics/RectangleShape.hpp>
#include <External/json.hpp>

class Player;
class Enemy;
class Cactus;
class Level;

using json = nlohmann::json;

namespace sf
{
	class RenderWindow;
	class View;
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
		bool load(const json* gameInfoJSON);

		// To-Do: Implement a unload()

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		// This is just an example. Think a good way to group the actors of your game. If they need any type of manager, etc...
		Player* m_player{ nullptr };
		Cactus* m_enemy{ nullptr };
		Enemy* m_enemyFrog{ nullptr };

		Level* m_level{ nullptr };
		// To-Do: This should be in its own class, something like "Level" should work
		//tmx::Map* m_map{ nullptr };
		//MapLayer* m_layerZero{ nullptr };
		//MapLayer* m_layerOne{ nullptr };
		//MapLayer* m_layerTwo{ nullptr };
		//ObjectLayer* m_groundsLayer{ nullptr };
		//ObjectLayer* m_wallsLayer{ nullptr };
		//ObjectLayer* m_ceilingsLayer{ nullptr };
		//ObjectLayer* m_trapsLayer{ nullptr };

		sf::View* m_view{ nullptr };
		sf::FloatRect m_deadZone;
		void drawDeadZone(sf::RenderWindow& window);
};