#include <Core/AssetManager.h>
#include <Core/World.h>
#include <Gameplay/Player.h>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>
#include <Utils/Constants.h>


World::~World()
{
	delete m_player;
	delete m_layerZero;
	delete m_layerOne;
	delete m_layerTwo;
	delete m_map;
}

bool World::load()
{
	constexpr float millisecondsToSeconds = 1 / 1000.f;

	// To-Do, Load level: this should have its own class
	m_map = new tmx::Map();
	m_map->load("../Data/Levels/RoboJumpMap_Level1.tmx");
	m_layerZero = new MapLayer(*m_map, 0);
	m_layerOne = new MapLayer(*m_map, 1);
	m_layerTwo = new MapLayer(*m_map, 2);

	m_collisionLayer = new ObjectLayer(*m_map, 3);
	m_gemsLayer = new ObjectLayer(*m_map, 4);

	m_layerZero->setOffset({ .0f, .0f });

	// To-Do, read ALL from file, this is just a quick example to understand that here is where entities are created but consider grouping/managing actors in a smarter way
	sf::Texture* playerTexture = AssetManager::getInstance()->loadTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/1 Main Characters/MainCharacter2.png");
	Player::PlayerDescriptor playerDescriptor;
	playerDescriptor.texture = playerTexture;
	playerDescriptor.position = { 0.f, 0.f };
	playerDescriptor.speed = { 100.f * millisecondsToSeconds, 100.f * millisecondsToSeconds }; // 400 units per second, or 0.4 units per millisecond, using the latter so it's in alignment with delta time
	playerDescriptor.tileWidth = 32.f;
	playerDescriptor.tileHeight = 32.f;
	Player* player = new Player();
	const bool initOk = player->init(playerDescriptor);

	m_player = player;
	player->setPosition({ MAP_TILE_SIZE * 1.f, MAP_TILE_SIZE * 17.f });



	return initOk;
}

void World::update(uint32_t deltaMilliseconds)
{
	// To-Do: update level
	m_layerZero->update(sf::milliseconds(deltaMilliseconds));
	m_layerOne->update(sf::milliseconds(deltaMilliseconds));
	m_layerTwo->update(sf::milliseconds(deltaMilliseconds));


	// Update actors
	// Check for collisions (We could do it in a function here or have a collision manager if it gets complex)
	const auto& collisionShapes = m_collisionLayer->getShapes();
	bool isColliding = false;
	for (const auto* shape : collisionShapes)
	{	
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
		{
			isColliding = true;
			m_player->setGravity(.0f);
			m_player->setIsJumping(false);
			m_player->setPosition({ m_player->getPosition().x, shape->getGlobalBounds().top - m_player->getBounds().height +1.f});
			//m_player->setSpeed({ m_player->getSpeed().x, .0f });
			#if DEBUG_MODE
				//printf("Wall Collision \n");
			#endif
		}
	}
	//printf("Is colliding: %d\n", isColliding);
	if (!isColliding)
	{
		m_player->setGravity(980.f);
	}
	m_player->update(deltaMilliseconds);

	const auto& gemShapes = m_gemsLayer->getShapes();
	for (const auto* shape : gemShapes)
	{
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
		{
			#if DEBUG_MODE
				//printf("Gem collected \n");
			#endif
		}
	}
}

void World::render(sf::RenderWindow& window)
{
	window.draw(*m_layerZero);
	window.draw(*m_layerOne);
	window.draw(*m_layerTwo);
	window.draw(*m_collisionLayer);
	window.draw(*m_gemsLayer);
	m_player->render(window);
}