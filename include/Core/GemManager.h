#pragma once

#include <Gameplay/Gem.h>
#include <External/json.hpp>

using json = nlohmann::json;

class GemManager
{
	public:
		GemManager() = default;
		~GemManager();
		bool loadGems();
		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);
		void destroyGem(Gem*);
		std::vector<Gem*>& getGemsVector() { return m_gemsVector; }

	private:
		std::vector<Gem*> m_gemsVector;
};