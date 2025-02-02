#pragma once

class UIScreenMainMenu;
class UIScreenGameOver;
class UIScreenVictory;

namespace sf
{
	class RenderWindow;
}

class UIManager
{
	public:

		~UIManager();

		bool load();
		
		UIScreenMainMenu* getMainMenuScreen() const { return m_mainMenuScreen; }
		UIScreenGameOver* getGameOverScreen() const { return m_gameOverScreen; }
		UIScreenVictory* getVictoryScreen() const { return m_victoryScreen; }

	private:

		UIScreenMainMenu* m_mainMenuScreen = nullptr;
		UIScreenGameOver* m_gameOverScreen = nullptr;
		UIScreenVictory* m_victoryScreen = nullptr;
};