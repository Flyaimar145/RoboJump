#pragma once

#include <cstdint>

namespace tmx
{
	class Map;
}
namespace sf
{
	class RenderWindow;
	class View;
}

class MapLayer;
class ObjectLayer;

class Level
{
public:

	~Level();

	bool load();
	// To-Do: Implement a unload()
	void update(uint32_t deltaMilliseconds);
	void render(sf::RenderWindow& window);

	MapLayer* getLayerZero() const { return m_layerZero; }
	MapLayer* getLayerOne() const { return m_layerOne; }
	MapLayer* getLayerTwo() const { return m_layerTwo; }
	ObjectLayer* getGroundsLayer() const { return m_groundsLayer; }
	ObjectLayer* getWallsLayer() const { return m_wallsLayer; }
	ObjectLayer* getCeilingsLayer() const { return m_ceilingsLayer; }
	ObjectLayer* getTrapsLayer() const { return m_trapsLayer; }
	ObjectLayer* getEnemyWallsLayer() const { return m_enemyWallsLayer; }
	

private:
	
	tmx::Map* m_map{ nullptr };
	MapLayer* m_layerZero{ nullptr };
	MapLayer* m_layerOne{ nullptr };
	MapLayer* m_layerTwo{ nullptr };
	ObjectLayer* m_groundsLayer{ nullptr };
	ObjectLayer* m_wallsLayer{ nullptr };
	ObjectLayer* m_ceilingsLayer{ nullptr };
	ObjectLayer* m_trapsLayer{ nullptr };
	ObjectLayer* m_enemyWallsLayer{ nullptr };

};
