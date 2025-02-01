#include <UI/HUD.h>
#include <Core/AssetManager.h>
#include <cstdio>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <Utils/Constants.h>

bool HUD::init()
{
	
	if (!m_font.loadFromFile("../data/Fonts/Pixel.ttf"))
	{
		printf("Error loading font\n");
		return false;
	}
	m_scoreText.setFont(m_font);
	m_scoreText.setCharacterSize(32);
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setPosition(10.f, 5.f);

    return true;
}

void HUD::update(int score)
{
	m_score = score;
	m_scoreText.setString("Score: " + std::to_string(m_score));
}

void HUD::render(sf::RenderWindow& window)
{
	window.draw(m_scoreText);
}