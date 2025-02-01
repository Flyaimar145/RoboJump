// To-Do. Ideally a UIScreen transition should be managed through a UIManager class
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
	void update(float milliseconds);
	void render(sf::RenderWindow& window);

	UIScreen* getCurrentScreen() const { return m_screens[m_currentScreenIndex]; }
	UIScreen* getMainMenuScreen() const { return m_mainMenuScreen; }
	UIScreen* getGameOverScreen() const { return m_gameOverScreen; }
	UIScreen* getVictoryScreen() const { return m_victoryScreen; }

private:
	std::vector<UIScreen*> m_screens;
	int m_currentScreenIndex = 0;
	UIScreen* m_mainMenuScreen = nullptr;
	UIScreen* m_gameOverScreen = nullptr;
	UIScreen* m_victoryScreen = nullptr;
};