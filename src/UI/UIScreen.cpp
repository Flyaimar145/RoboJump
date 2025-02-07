#include <SFML/Graphics/RenderWindow.hpp>
#include <UI/UIScreen.h>

bool UIScreen::init(sf::Texture*image)
{
	m_sprite.setTexture(*image);
	if (m_sprite.getTexture() == nullptr)
	{
		return false;
	}
	return true;
}

void UIScreen::render(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}