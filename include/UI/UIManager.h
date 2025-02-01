#pragma once

class UIScreen;

namespace sf
{
	class RenderWindow;
}

class UIManager
{
	public:
		~UIManager();

		bool load();
		
		UIScreen* getMainMenuScreen() const { return m_mainMenuScreen; }
		UIScreen* getGameOverScreen() const { return m_gameOverScreen; }
		UIScreen* getVictoryScreen() const { return m_victoryScreen; }

	private:
		UIScreen* m_mainMenuScreen = nullptr;
		UIScreen* m_gameOverScreen = nullptr;
		UIScreen* m_victoryScreen = nullptr;
};