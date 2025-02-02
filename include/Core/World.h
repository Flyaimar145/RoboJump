#pragma once

#include <cstdint>
#include <SFML/Graphics/RectangleShape.hpp>

class EnemyManager;
class HUD;
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

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

		bool getIsPlayerDead() const { return m_isPlayerDead; }
		bool getPlayerHasReachedVictory() const { return m_playerHasReachedVictory; }

	private:

		Player* m_player{ nullptr };

		Level* m_level{ nullptr };

		EnemyManager* m_enemyManager{ nullptr };
		PickUpManager* m_pickUpManager{ nullptr };

		sf::View* m_view{ nullptr };
		sf::View* m_hudView{ nullptr };
		HUD* m_hud{ nullptr };
		sf::FloatRect m_deadZone;

		bool m_isPlayerDead{ false };
		bool m_playerHasReachedVictory{ false };
 
		void updateDeadZone();

		void checkPlayerEnvironmentCollisions();
		void checkPlayerPickUpsCollisions();
		void checkPlayerEnemiesCollisions();
		void checkPlayerDeath();
		void checkIfPlayerHasReachedVictory();

		void checkSpecialsForCactusEnemies();
		void checkSpecialsForStompEnemies();
};