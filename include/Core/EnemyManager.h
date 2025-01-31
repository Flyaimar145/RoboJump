#pragma once

#include <cstdint>
#include <Gameplay/Enemies/Cactus.h>
#include <Gameplay/Enemies/Enemy.h>
#include <Gameplay/Enemies/Frog.h>
#include <Gameplay/Enemies/Stomp.h>

namespace sf
{
	class RenderWindow;
}

class EnemyManager 
{
	public:
		~EnemyManager();
		
		bool loadEnemies();
		
		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);
		void destroyCactus(Cactus*);

		std::vector<Enemy*>& getEnemiesVector() { return m_enemiesVector; }
		std::vector<Cactus*>& getCactusTypeEnemiesVector() { return m_cactusTypeEnemiesVector; }
		std::vector<Frog*>& getFrogTypeEnemiesVector() { return m_frogTypeEnemiesVector; }
		std::vector<Stomp*>& getStompTypeEnemiesVector() { return m_stompTypeEnemiesVector; }

	private:
		std::vector<Enemy*> m_enemiesVector;
		std::vector<Cactus*> m_cactusTypeEnemiesVector;
		std::vector<Frog*> m_frogTypeEnemiesVector;
		std::vector<Stomp*> m_stompTypeEnemiesVector;
};

