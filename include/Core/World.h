#pragma once

#include <cstdint>
#include <SFML/Graphics/RectangleShape.hpp>


class EnemyManager;
class Level;
class PickUpManager;
class Player;


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

		bool load();

		// To-Do: Implement a unload()

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		Player* m_player{ nullptr };

		Level* m_level{ nullptr };

		EnemyManager* m_enemyManager{ nullptr };
		PickUpManager* m_pickUpManager{ nullptr };

		sf::View* m_view{ nullptr };
		sf::FloatRect m_deadZone;

		void drawDeadZone(sf::RenderWindow& window);
		void updateDeadZone();
		void checkPlayerEnvironmentCollisions();
		void checkPlayerPickUpsCollisions();
		void checkPlayerEnemiesCollisions();
		void checkPlayerDeath();
};