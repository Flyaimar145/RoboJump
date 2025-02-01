#include <Core/AssetManager.h>
#include <External/json.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <UI/UIManager.h>
#include <UI/UIScreen.h>
#include <UI/UIScreenGameOver.h>
#include <UI/UIScreenMainMenu.h>
#include <UI/UIScreenVictory.h>
#include <Utils/Constants.h>

using json = nlohmann::json;

UIManager::~UIManager()
{
	delete m_mainMenuScreen;
	m_mainMenuScreen = nullptr;

	delete m_gameOverScreen;
	m_gameOverScreen = nullptr;

	delete m_victoryScreen;
	m_victoryScreen = nullptr;
}

bool UIManager::load()
{
	json uiInfo = loadJsonFromFile(GAMEINFOJSON_UI)["UIScreens"];
	sf::Texture* mainMenuTexture = AssetManager::getInstance()->loadTexture(uiInfo["mainMenuScreenTexturePath"].get<std::string>().c_str());
	sf::Texture* gameOverTexture = AssetManager::getInstance()->loadTexture(uiInfo["gameOverScreenTexturePath"].get<std::string>().c_str());
	sf::Texture* victoryTexture = AssetManager::getInstance()->loadTexture(uiInfo["victoryScreenTexturePath"].get<std::string>().c_str());
	if (mainMenuTexture == nullptr || gameOverTexture == nullptr || victoryTexture == nullptr)
	{
		return false;
	}
	m_gameOverScreen = new UIScreenGameOver();
	m_mainMenuScreen = new UIScreenMainMenu();
	m_victoryScreen = new UIScreenVictory();

	if (!m_gameOverScreen->init(gameOverTexture) || !m_mainMenuScreen->init(mainMenuTexture) || !m_victoryScreen->init(victoryTexture))
	{
		return false;
	}
	return true;
}

