#include <Core/AssetManager.h>
#include <UI/UIManager.h>
#include <UI/UIScreen.h>
#include <UI/UIScreenGameOver.h>
#include <UI/UIScreenMainMenu.h>
#include <UI/UIScreenVictory.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

UIManager::~UIManager()
{
	for (UIScreen* screen : m_screens)
	{
		delete screen;
	}
	//m_currentScreen = nullptr;
	//m_nextScreen = nullptr;
	m_screens.clear();
}

bool UIManager::load()
{
	sf::Texture* mainMenuTexture = AssetManager::getInstance()->loadTexture("../data/Images/GameScreens/RoboJumpMainScreen_2.png");
	sf::Texture* gameOverTexture = AssetManager::getInstance()->loadTexture("../data/Images/GameScreens/GameOverScreen_2.png");
	sf::Texture* victoryTexture = AssetManager::getInstance()->loadTexture("../data/Images/GameScreens/VictoryScreen_2.png");
	if (mainMenuTexture == nullptr || gameOverTexture == nullptr || victoryTexture == nullptr)
	{
		return false;
	}
	m_gameOverScreen = new UIScreenGameOver();
	m_mainMenuScreen = new UIScreenMainMenu();
	m_victoryScreen = new UIScreenVictory();

	if (!m_gameOverScreen->init(gameOverTexture) || !m_mainMenuScreen->init(mainMenuTexture) || !m_victoryScreen->init(victoryTexture))
	{
		return false;
	}

	m_screens.push_back(m_mainMenuScreen);
	m_screens.push_back(m_gameOverScreen);
	m_screens.push_back(m_victoryScreen);
	//m_currentScreen = mainMenuScreen;
	return true;
}

void UIManager::update(float milliseconds)
{
	//m_currentScreen->update(milliseconds);
}

void UIManager::render(sf::RenderWindow& window)
{
	//m_currentScreen->render(window);
}

//void UIManager::setNextScreen()
//{
	//m_currentScreenIndex = (m_currentScreenIndex + 1) % m_screens.size();
	//m_currentScreen = m_screens[m_currentScreenIndex];
//}
