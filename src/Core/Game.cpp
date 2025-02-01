#include <cassert>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>
#include <UI/UIScreen.h>
#include <UI/UIManager.h>
#include <Utils/Constants.h>
#include <SFML/Window/Keyboard.hpp>


bool Game::init()
{
	assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

	json gameConfigInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];

	//m_window = new sf::RenderWindow({ gameConfigInfo["screenWidth"], gameConfigInfo["screenHeight"] }, gameConfigInfo["gameTitle"].get<std::string>());
	m_window = new sf::RenderWindow({ gameConfigInfo["screenWidth"], gameConfigInfo["screenHeight"] }, gameConfigInfo["gameTitle"].get<std::string>(), sf::Style::Fullscreen);

	m_window->setFramerateLimit(gameConfigInfo["frameRateLimit"]);

	m_uiManager = new UIManager();
	const bool uiManagerLoaded = m_uiManager->load();

	return uiManagerLoaded;
}

Game::~Game()
{
	delete m_world;
	m_world = nullptr;
	delete m_window;
	m_window = nullptr;
	delete m_uiManager;
	m_uiManager = nullptr;
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	for (auto event = sf::Event(); m_window->pollEvent(event);)
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
		}
	}
	switch (m_gameState)
	{
		case GameState::MainMenu:
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				m_window->close();
			}
			m_uiManager->getMainMenuScreen()->update(deltaMilliseconds);
			if (m_uiManager->getMainMenuScreen()->getGoToNextScreen())
			{
				this->setGameState(GameState::Playing);
				m_uiManager->getMainMenuScreen()->setGoToNextScreen(false);
			}
			break;
		}
		case GameState::Playing:
		{
			if (m_isWorldLoaded == false)
			{
				m_world = new World();
				m_isWorldLoaded = m_world->load();
			}
			else if (m_world->getIsPlayerDead())
			{
				delete m_world;
				m_world = nullptr;
				m_isWorldLoaded = false;
				m_window->setView(m_window->getDefaultView());
				this->setGameState(GameState::GameOver);
			}
			else if (m_world->getPlayerHasReachedVictory())
			{
				delete m_world;
				m_world = nullptr;
				m_isWorldLoaded = false;
				m_window->setView(m_window->getDefaultView());
				this->setGameState(GameState::Victory);
			}
			else
			{
				m_world->update(deltaMilliseconds);
			}
			break;
		}
		case GameState::GameOver:
		{
			m_uiManager->getGameOverScreen()->update(deltaMilliseconds);
			if (m_uiManager->getGameOverScreen()->getGoToNextScreen())
			{
				this->setGameState(GameState::MainMenu);
				m_uiManager->getGameOverScreen()->setGoToNextScreen(false);
			}

			break;
		}
		case GameState::Victory:
		{
			m_uiManager->getVictoryScreen()->update(deltaMilliseconds);
			if (m_uiManager->getVictoryScreen()->getGoToNextScreen())
			{
				this->setGameState(GameState::MainMenu);
				m_uiManager->getVictoryScreen()->setGoToNextScreen(false);
			}
			break;
		}
		default:
		{
			assert(false && "Unknown game state");
			break;
		}
	}
}

void Game::render()
{
	m_window->clear();

	switch (m_gameState)
	{
		case GameState::MainMenu:
		{
			m_uiManager->getMainMenuScreen()->render(*m_window);
			break;
		}
		case GameState::Playing:
		{
			if (m_isWorldLoaded)
			{
				m_world->render(*m_window);
			}
			break;
		}
		case GameState::GameOver:
		{
			if (m_isWorldLoaded)
			{
				m_world->render(*m_window);
			}
			else
			{
				m_uiManager->getGameOverScreen()->render(*m_window);
			}
			break;
		}
		case GameState::Victory:
		{
			if (m_isWorldLoaded)
			{
				m_world->render(*m_window);
			}
			else
			{
				m_uiManager->getVictoryScreen()->render(*m_window);
			}
			break;
		}
		default:
		{
			assert(false && "Unknown game state");
			break;
		}
	}

	m_window->display();
}