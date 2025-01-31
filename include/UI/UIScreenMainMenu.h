// To-Do. Ideally a UIScreen transition should be managed through a UIManager class
#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>


namespace sf
{
	class RenderWindow;
}

class UIScreenMainMenu
{
public:
	~UIScreenMainMenu() = default;

	bool init();
	void update(float milliseconds);
	void render(sf::RenderWindow& window);

private:
	sf::Sprite m_sprite;
};