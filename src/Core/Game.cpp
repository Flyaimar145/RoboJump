#include <cassert>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>
#include <UI/UIScreenMainMenu.h>
#include <Utils/Constants.h>
#include <SFML/Window/Keyboard.hpp>


bool Game::init()
{
	assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

	json gameConfigInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];

	m_window = new sf::RenderWindow({ gameConfigInfo["screenWidth"], gameConfigInfo["screenHeight"] }, gameConfigInfo["gameTitle"].get<std::string>());
	//m_window = new sf::RenderWindow({ gameConfigInfo["screenWidth"], gameConfigInfo["screenHeight"] }, gameConfigInfo["gameTitle"].get<std::string>(), sf::Style::Fullscreen);

	m_window->setFramerateLimit(gameConfigInfo["frameRateLimit"]);

	m_mainMenu = new UIScreenMainMenu();
	const bool mainMenuInitOk = m_mainMenu->init();

	//m_world = new World();
	//const bool loadOk = m_world->load();

	return mainMenuInitOk;//loadOk;
}

Game::~Game()
{
	// To-Do: make sure m_world is unloaded()

	delete m_world;
	m_world = nullptr;
	delete m_window;
	m_window = nullptr;
	delete m_mainMenu;
	m_mainMenu = nullptr;
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			{
				this->setGameState(GameState::Playing);
			}
			m_mainMenu->update(deltaMilliseconds);
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
				this->setGameState(GameState::GameOver);
			}
			else
			{
				m_world->update(deltaMilliseconds);
			}
			break;
		}
		case GameState::GameOver:
		{
			// To-Do: update the game over screen
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
			{
				this->setGameState(GameState::MainMenu);
			}
			m_mainMenu->update(deltaMilliseconds);

			break;
		}
	}
	//m_world->update(deltaMilliseconds);
}

void Game::render()
{
	m_window->clear();

	switch (m_gameState)
	{
	case GameState::MainMenu:
	{
		m_mainMenu->render(*m_window);
		break;
	}
	case GameState::Playing:
	{
		if (m_isWorldLoaded)
		{
			m_world->render(*m_window);
		}
		//m_world->render(*m_window);
		break;
	}
	case GameState::GameOver:
	{
		// To-Do: render the game over screen
		m_mainMenu->render(*m_window);
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