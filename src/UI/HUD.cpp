#include <Core/AssetManager.h>
#include <External/json.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <UI/HUD.h>
#include <Utils/Constants.h>

using json = nlohmann::json;

bool HUD::init()
{
	json hudInfo = loadJsonFromFile(GAMEINFOJSON_UI)["HUDInfo"];
	
	if (!m_font.loadFromFile(hudInfo["fontPath"].get<std::string>()))
	{
		printf("Error loading font\n");
		return false;
	}
	m_scoreText.setFont(m_font);
	m_scoreText.setCharacterSize(hudInfo["characterSize"].get<float>());
	m_scoreText.setPosition(hudInfo["positionX"].get<float>(), hudInfo["positionY"].get<float>());

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