#include <SFML/Window/Keyboard.hpp>
#include <UI/UIScreenMainMenu.h>

void UIScreenMainMenu::update(float milliseconds)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		m_goToNextScreen = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_exitGame = true;
	}
}
