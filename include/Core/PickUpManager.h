#pragma once

#include <Gameplay/PickUp.h>
#include <Gameplay/Gem.h>
#include <Gameplay/PowerUp.h>
#include <External/json.hpp>

class PickUpManager
{
public:
	PickUpManager() = default;
	~PickUpManager();
	bool loadPickUps();
	void update(uint32_t deltaMilliseconds);
	void render(sf::RenderWindow& window);
	void destroyPickUp(PickUp*);
	std::vector<PickUp*>& getPickUpsVector() { return m_pickUpsVector; }
	std::vector<Gem*>& getGemsVector() { return m_gemsVector; }
	std::vector<PowerUp*>& getPowerUpsVector() { return m_powerUpsVector; }

private:
	std::vector<PickUp*> m_pickUpsVector;
	std::vector<Gem*> m_gemsVector;
	std::vector<PowerUp*> m_powerUpsVector;
};