#include <Core/Level.h>
#include <tmxlite/Map.hpp>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>

Level::~Level()
{
	delete m_layerZero;
	m_layerZero = nullptr;
	delete m_layerOne;
	m_layerOne = nullptr;
	delete m_layerTwo;
	m_layerTwo = nullptr;

	delete m_groundsLayer;
	m_groundsLayer = nullptr;
	delete m_wallsLayer;
	m_wallsLayer = nullptr;
	delete m_ceilingsLayer;
	m_ceilingsLayer = nullptr;
	delete m_trapsLayer;
	m_trapsLayer = nullptr;
	delete m_enemyWallsLayer;
	m_enemyWallsLayer = nullptr;

	delete m_map;
	m_map = nullptr;
}
bool Level::load()
{
	m_map = new tmx::Map();
	m_map->load("../Data/Levels/RoboJumpMap_Level1.tmx");
	m_layerZero = new MapLayer(*m_map, 0);
	m_layerOne = new MapLayer(*m_map, 1);
	m_layerTwo = new MapLayer(*m_map, 2);

	m_groundsLayer = new ObjectLayer(*m_map, 3);
	m_wallsLayer = new ObjectLayer(*m_map, 4);
	m_ceilingsLayer = new ObjectLayer(*m_map, 5);
	m_trapsLayer = new ObjectLayer(*m_map, 6);
	m_enemyWallsLayer = new ObjectLayer(*m_map, 7);

	m_layerZero->setOffset({ .0f, .0f });
	
	return true;
}

void Level::update(uint32_t deltaMilliseconds)
{
	m_layerZero->update(sf::milliseconds(deltaMilliseconds));
	m_layerOne->update(sf::milliseconds(deltaMilliseconds));
	m_layerTwo->update(sf::milliseconds(deltaMilliseconds));
}

void Level::render(sf::RenderWindow& window)
{
	window.draw(*m_layerZero);
	window.draw(*m_layerOne);
	window.draw(*m_layerTwo);

	window.draw(*m_groundsLayer);
	window.draw(*m_wallsLayer);
	window.draw(*m_ceilingsLayer);
	window.draw(*m_trapsLayer);
	window.draw(*m_enemyWallsLayer);
}