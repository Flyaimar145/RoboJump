#pragma once

#include <Gameplay/PowerUp.h>
#include <External/json.hpp>

class PowerUpManager
{
public:
	PowerUpManager() = default;
	~PowerUpManager();
	bool loadPowerUps();
	void update(uint32_t deltaMilliseconds);
	void render(sf::RenderWindow& window);
	void destroyPowerUp(PowerUp*);
	std::vector<PowerUp*>& getPowerUpsVector() { return m_powerUpsVector; }

private:
	std::vector<PowerUp*> m_powerUpsVector;
};