#include <SFML/Window/Keyboard.hpp>
#include <UI/UIScreenGameOver.h>

void UIScreenGameOver::update(float milliseconds)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		m_goToNextScreen = true;
	}
}
