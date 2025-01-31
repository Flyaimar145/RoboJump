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
class UIScreenMainMenu;

class Game
{
	public:

		enum class GameState
		{
			MainMenu,
			Playing,
			GameOver
		};

		~Game();

		bool init();
		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

		GameState getGameState() const { return m_gameState; }

		void setGameState(GameState gameState) { m_gameState = gameState; }

	private:

		GameState m_gameState{ GameState::MainMenu };
		bool m_isWorldLoaded{ false };

		sf::RenderWindow* m_window{ nullptr };
		World* m_world{ nullptr };
		UIScreenMainMenu* m_mainMenu{ nullptr };

};