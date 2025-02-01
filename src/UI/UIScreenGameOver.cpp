
#include <UI/UIScreenGameOver.h>
#include <SFML/Window/Keyboard.hpp>

bool UIScreenGameOver::init(sf::Texture* image)
{
	m_screenType = ScreenType::GameOver;
	return UIScreen::init(image);
}

void UIScreenGameOver::update(float milliseconds)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		m_goToNextScreen = true;
	}
}
