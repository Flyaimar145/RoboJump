#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
	class RenderWindow;
}

class UIScreen
{
	public:

		~UIScreen() = default;

		virtual bool init(sf::Texture* image);
		virtual void update(float milliseconds) = 0;
		void render(sf::RenderWindow& window);

		bool getGoToNextScreen() const { return m_goToNextScreen; }

		void setGoToNextScreen(bool goToNextScreen) { m_goToNextScreen = goToNextScreen; }

	protected:

		sf::Sprite m_sprite;

		bool m_goToNextScreen{ false };
};