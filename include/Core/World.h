#pragma once

#include <cstdint>
#include <SFML/Graphics/RectangleShape.hpp>



class EnemyManager;
class Level;
class PickUpManager;
class Player;
class HUD;

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

		bool getIsPlayerDead() const { return m_isPlayerDead; }
		bool getPlayerHasReachedVictory() const { return m_playerHasReachedVictory; }

	private:

		Player* m_player{ nullptr };

		Level* m_level{ nullptr };

		EnemyManager* m_enemyManager{ nullptr };
		PickUpManager* m_pickUpManager{ nullptr };

		sf::View* m_view{ nullptr };
		HUD* m_hud{ nullptr };
		sf::View* m_hudView;
		sf::FloatRect m_deadZone;
		sf::FloatRect m_victoryZone;

		bool m_isPlayerDead{ false };
		bool m_playerHasReachedVictory{ false };

		void drawDeadZone(sf::RenderWindow& window);
		void updateDeadZone();
		void checkPlayerEnvironmentCollisions();
		void checkPlayerPickUpsCollisions();
		void checkPlayerEnemiesCollisions();
		void checkPlayerDeath();
		void checkIfPlayerHasReachedVictory();

		void checkSpecialsForCactusEnemies();
		void checkSpecialsForStompEnemies();
};