#include <Core/AssetManager.h>
#include <UI/UIScreenMainMenu.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>


bool UIScreenMainMenu::init()
{
	sf::Texture* texture =  AssetManager::getInstance()->loadTexture("../data/Images/GameScreens/RoboJumpMain_2.png");
	m_sprite.setTexture(*texture);
	//if (m_sprite.getTexture() == nullptr)
	//{
	//	return false;
	//}
	return true;
}

void UIScreenMainMenu::update(float milliseconds)
{
	
}

void UIScreenMainMenu::render(sf::RenderWindow& window)
{
	window.draw(m_sprite);
	/*const sf::FloatRect spriteBounds = m_exampleRect;
	sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	boundsRect.setOutlineColor(sf::Color::Blue);
	boundsRect.setOutlineThickness(.5f);
	boundsRect.setFillColor(sf::Color::Red);
	window.draw(boundsRect);*/
}