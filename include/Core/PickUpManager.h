#pragma once

#include <Gameplay/PickUp.h>
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

private:
	std::vector<PickUp*> m_pickUpsVector;
};