#include <UI/UIScreenMainMenu.h>

#include <SFML/Window/Keyboard.hpp>

bool UIScreenMainMenu::init(sf::Texture* image)
{
	m_screenType = ScreenType::MainMenu;
	return UIScreen::init(image);
}

void UIScreenMainMenu::update(float milliseconds)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		m_goToNextScreen = true;
	}
}
