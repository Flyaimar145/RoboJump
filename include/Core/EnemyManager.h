#pragma once

#include <cstdint>
#include <string>
#include <External/json.hpp>
#include <Gameplay/Enemies/Enemy.h>
#include <Gameplay/Enemies/Cactus.h>
#include <Gameplay/Enemies/Frog.h>
#include <Gameplay/Enemies/Stomp.h>

namespace sf
{
	class RenderWindow;
}

using json = nlohmann::json;

class EnemyManager 
{
	public:
		EnemyManager() = default;
		~EnemyManager();
		
		bool loadEnemies();
		
		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

		std::vector<Enemy*>& getEnemiesVector() { return m_enemiesVector; }
		std::vector<Enemy*>& getCactusTypeEnemiesVector() { return m_cactusTypeEnemiesVector; }
		std::vector<Enemy*>& getFrogTypeEnemiesVector() { return m_frogTypeEnemiesVector; }

	private:
		std::vector<Enemy*> m_enemiesVector;
		std::vector<Enemy*> m_cactusTypeEnemiesVector;
		std::vector<Enemy*> m_frogTypeEnemiesVector;
		std::vector<Enemy*> m_stompTypeEnemiesVector;
};

