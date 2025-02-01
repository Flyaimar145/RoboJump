// To-Do. Ideally a UIScreen transition should be managed through a UIManager class
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
	enum class ScreenType
	{
		MainMenu,
		GameOver,
		Victory
	};

	~UIScreen() = default;

	virtual bool init(sf::Texture* image);
	virtual void update(float milliseconds) = 0;
	void render(sf::RenderWindow& window);

	ScreenType getScreenType() const { return m_screenType; }
	bool getGoToNextScreen() const { return m_goToNextScreen; }

	void setScreenType(ScreenType screenType) { m_screenType = screenType; }
	void setGoToNextScreen(bool goToNextScreen) { m_goToNextScreen = goToNextScreen; }

protected:
	ScreenType m_screenType;
	sf::Sprite m_sprite;

	bool m_goToNextScreen{ false };
};