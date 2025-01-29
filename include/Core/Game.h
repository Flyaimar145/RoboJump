#pragma once

#include <cstdint>
#include <string>
#include <External/json.hpp>

namespace sf
{
	class RenderWindow;
}

using json = nlohmann::json;

class World;

class Game
{
	public:
		~Game();

		bool init();
		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

	private:
		sf::RenderWindow* m_window{ nullptr };
		World* m_world{ nullptr };
};