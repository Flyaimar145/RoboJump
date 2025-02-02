#pragma once

#include <cstdint>

namespace sf
{
	class RenderWindow;
}

class World;
class UIManager;

class Game
{
	public:

		enum class GameState
		{
			MainMenu,
			Playing,
			GameOver,
			Victory
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
		UIManager* m_uiManager{ nullptr };

};