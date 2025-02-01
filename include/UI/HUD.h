#pragma once


#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class HUD
{
    public:

	    ~HUD() = default;

        bool init();
        void update(int score);
        void render(sf::RenderWindow& window);

    private:

	    sf::Font m_font;
	    sf::Text m_scoreText;
        int m_score{ 0 };
};