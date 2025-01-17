#pragma once

#include <cstdint>
#include <string>

namespace sf
{
	class RenderWindow;
}

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

		bool init(GameCreateInfo& createInfo);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

	private:

		sf::RenderWindow* m_window{ nullptr };
		World* m_world{ nullptr };
};