#include <UI/UIScreenVictory.h>

#include <SFML/Window/Keyboard.hpp>


bool UIScreenVictory::init(sf::Texture* image)
{
	m_screenType = ScreenType::Victory;
	return UIScreen::init(image);
}

void UIScreenVictory::update(float milliseconds)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B))
	{
		m_goToNextScreen = true;
	}
}

