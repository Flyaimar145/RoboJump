#include <cassert>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

bool Game::init(GameCreateInfo& createInfo, const json* gameInfoJSON)
{
	assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

	m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
	//m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle, sf::Style::Fullscreen);

	m_window->setFramerateLimit(createInfo.frameRateLimit);
	m_gameInfoJSON = gameInfoJSON;
	m_world = new World();
	const bool loadOk = m_world->load(gameInfoJSON);

	return loadOk;
}

Game::~Game()
{
	// To-Do: make sure m_world is unloaded()

	delete m_world;
	delete m_window;
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	
	// Check if user closed the window
	for (auto event = sf::Event(); m_window->pollEvent(event);)
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
		}
	}

	// Update scene here
	m_world->update(deltaMilliseconds);
}

void Game::render()
{
	m_window->clear();

	m_world->render(*m_window);

	m_window->display();
}