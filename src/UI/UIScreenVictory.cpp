#include <SFML/Window/Keyboard.hpp>
#include <UI/UIScreenVictory.h>

void UIScreenVictory::update(float milliseconds)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B))
	{
		m_goToNextScreen = true;
	}
}

