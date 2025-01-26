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

		struct GameCreateInfo
		{
			std::string gameTitle{ "Game Title Example" };
			uint32_t screenWidth{ 1920 };
			uint32_t screenHeight{ 1080 };
			uint32_t frameRateLimit{ 60 };
		};

		~Game();

		bool init(GameCreateInfo& createInfo, const json* gameInfoJSON);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

	private:

		sf::RenderWindow* m_window{ nullptr };
		World* m_world{ nullptr };
		const json* m_gameInfoJSON{ nullptr };
};