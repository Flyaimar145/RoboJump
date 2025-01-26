#include <Core/AssetManager.h>
#include <Core/CollisionManager.h>
#include <Core/Level.h>
#include <Core/World.h>
#include <Gameplay/Player.h>
#include <Gameplay/Entity.h>
#include <Gameplay/Enemies/Enemy.h>
#include <Gameplay/Enemies/Cactus.h>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>
#include <Utils/Constants.h>


World::~World()
{
	delete m_player;
	delete m_enemy;
	delete m_enemyFrog;
	//delete m_layerZero;
	//delete m_layerOne;
	//delete m_layerTwo;
	//delete m_groundsLayer;
	//delete m_wallsLayer;
	//delete m_ceilingsLayer;
	//delete m_trapsLayer;
	//delete m_map;
	delete m_view;
	delete m_level;
}

bool World::load()
{
	constexpr float millisecondsToSeconds = 1 / 1000.f;

	// Load level
	m_level = new Level();
	const bool levelLoaded = m_level->load();

	m_view = new sf::View(sf::FloatRect({ 0.f, 0.f }, { 960.f, 540.f }));
	// Define the dead zone
	float deadZoneWidth = m_view->getSize().x * 0.15f;
	float deadZoneHeight = m_view->getSize().y * 0.25f;
	float deadZoneX = (m_view->getSize().x - deadZoneWidth) / 2.f;
	float deadZoneY = (m_view->getSize().y - deadZoneHeight) / 2.f;

	// Dead zone is defined relative to the center of the view
	m_deadZone = sf::FloatRect(deadZoneX, deadZoneY, deadZoneWidth, deadZoneHeight);


	// To-Do, read ALL from file, this is just a quick example to understand that here is where entities are created but consider grouping/managing actors in a smarter way
	
	json playerInfo = loadJsonFromFile(GAMEINFOJSON_PLAYER)["Player"];
	json gameInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];
	// Player
	//sf::Texture* playerTexture1 = AssetManager::getInstance()->loadTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/1 Main Characters/MainCharacter2.png");
	//sf::Texture* playerTexture2 = AssetManager::getInstance()->loadTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/1 Main Characters/MainCharacter2_1Live.png");
	sf::Texture* playerFirstTexture = AssetManager::getInstance()->loadTexture(playerInfo["firstTexture"].get<std::string>().c_str());
	sf::Texture* oneLifeLeftTexture = AssetManager::getInstance()->loadTexture(playerInfo["oneLifeLeftTexture"].get<std::string>().c_str());
	Player::PlayerDescriptor playerDescriptor;
	//playerDescriptor.firstTexture = playerTexture1;
	//playerDescriptor.position = { gameInfo["mapTileSize"] * 56.f, gameInfo["mapTileSize"] * 47.f};
	//playerDescriptor.speed = { 100.f * millisecondsToSeconds, 100.f * millisecondsToSeconds }; 
	//playerDescriptor.tileWidth = 32.f;
	//playerDescriptor.tileHeight = 32.f;
	//playerDescriptor.jumpSpeed = 300.f; 
	//playerDescriptor.totalFrames = 12;
	//playerDescriptor.deathAnimationTotalFrames = 7;
	//playerDescriptor.lifeCount = 2;
	playerDescriptor.firstTexture = playerFirstTexture;
	playerDescriptor.position = { gameInfo["mapTileSize"] * playerInfo["positionX"].get<float>(), gameInfo["mapTileSize"] * playerInfo["positionY"].get<float>() };
	playerDescriptor.speed = { playerInfo["speedX"].get<float>() * millisecondsToSeconds, playerInfo["speedY"].get<float>() * millisecondsToSeconds };
	playerDescriptor.tileWidth = playerInfo["tileWidth"].get<float>();
	playerDescriptor.tileHeight = playerInfo["tileHeight"].get<float>();
	playerDescriptor.jumpSpeed = playerInfo["jumpSpeed"].get<float>();
	playerDescriptor.totalFrames = playerInfo["totalFrames"].get<int>();
	playerDescriptor.deathAnimationTotalFrames = playerInfo["deathAnimationTotalFrames"].get<int>();
	playerDescriptor.lifeCount = playerInfo["lifeCount"].get<int>();
	
	Player* player = new Player();
	const bool playerLoaded = player->init(playerDescriptor);
	m_player = player;
	//player->setPosition({ MAP_TILE_SIZE * 56.f, MAP_TILE_SIZE * 47.f });

	// Enemy (Cactus)
	Enemy::EnemyDescriptor cactusDescriptor;
	cactusDescriptor.firstTexture = AssetManager::getInstance()->loadTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/4 Enemies/PatrolEnemyTileSet.png");
	cactusDescriptor.position = { gameInfo["mapTileSize"] * 51.f, gameInfo["mapTileSize"] * 37.f };
	cactusDescriptor.speed = { 100.f * millisecondsToSeconds, 0.f * millisecondsToSeconds };
	cactusDescriptor.tileWidth = 32.f;
	cactusDescriptor.tileHeight = 32.f;
	cactusDescriptor.totalFrames = 12;
	cactusDescriptor.deathAnimationTotalFrames = 7;
	cactusDescriptor.initialDirection = { -1.f, 0.f };
	cactusDescriptor.lifeCount = 1;
	Cactus* enemy = new Cactus();
	const bool enemyLoaded = enemy->init(cactusDescriptor);
	m_enemy = enemy;
	//enemy->setPosition({ MAP_TILE_SIZE * 51.f, MAP_TILE_SIZE * 37.f });


	// Enemy (Frog)
	Enemy::EnemyDescriptor enemyFrogDescriptor;
	enemyFrogDescriptor.firstTexture = AssetManager::getInstance()->loadTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/4 Enemies/FrogEnemyTileSet.png");
	enemyFrogDescriptor.position = { gameInfo["mapTileSize"] * 136.f, gameInfo["mapTileSize"] * 34.f };
	enemyFrogDescriptor.speed = { 0.f * millisecondsToSeconds, 0.f * millisecondsToSeconds };
	enemyFrogDescriptor.tileWidth = 64.f;
	enemyFrogDescriptor.tileHeight = 32.f;
	enemyFrogDescriptor.totalFrames = 11;
	enemyFrogDescriptor.deathAnimationTotalFrames = 7;
	enemyFrogDescriptor.initialDirection = { 1.f, 0.f };
	enemyFrogDescriptor.lifeCount = 1;
	Enemy* enemyFrog = new Enemy();
	const bool enemyFrogLoaded = enemyFrog->init(enemyFrogDescriptor);
	m_enemyFrog = enemyFrog;
	//enemyFrog->setPosition({ MAP_TILE_SIZE * 136.f, MAP_TILE_SIZE * 34.f });

	return playerLoaded && enemyLoaded && enemyFrogLoaded;
}

void World::update(uint32_t deltaMilliseconds)
{
	m_level->update(deltaMilliseconds);

	// Check for collisions (We could do it in a function here or have a collision manager if it gets complex)
	// Collision management (with CollisionManager)
	CollisionManager::getInstance()->checkGroundCollision(m_level->getGroundsLayer(), m_player);
	CollisionManager::getInstance()->checkWallCollision(m_level->getWallsLayer(), m_player);
	CollisionManager::getInstance()->checkCeilingCollision(m_level->getCeilingsLayer(), m_player);
	CollisionManager::getInstance()->checkTrapCollision(m_level->getTrapsLayer(), m_player);

	CollisionManager::getInstance()->checkCollisionBetweenPlayerAndEnemy(m_player, m_enemy);

	CollisionManager::getInstance()->checkEnemyWallCollision(m_level->getWallsLayer(), m_enemy);

	// Update player
	m_player->update(deltaMilliseconds);

	// Update enemy
	m_enemy->update(deltaMilliseconds);
	m_enemyFrog->update(deltaMilliseconds);

	// Adjust the view's center based on the dead zone
	sf::Vector2f playerPos = m_player->getPosition();
	sf::Vector2f viewCenter = m_view->getCenter();

	if (!m_player->getIsDead())
	{
		// Dead zone boundaries relative to the view's center
		sf::FloatRect viewDeadZone(
			viewCenter.x - m_deadZone.width / 2.f,
			viewCenter.y - m_deadZone.height / 2.f,
			m_deadZone.width,
			m_deadZone.height
		);

		// Check if the player is outside the dead zone
		if (playerPos.x < viewDeadZone.left) {
			viewCenter.x = playerPos.x + m_deadZone.width / 2.f;
		}
		else if (playerPos.x > viewDeadZone.left + m_deadZone.width) {
			viewCenter.x = playerPos.x - m_deadZone.width / 2.f;
		}

		if (playerPos.y < viewDeadZone.top) {
			viewCenter.y = playerPos.y + m_deadZone.height / 2.f;
		}
		else if (playerPos.y > viewDeadZone.top + m_deadZone.height) {
			viewCenter.y = playerPos.y - m_deadZone.height / 2.f;
		}

		m_view->setCenter(viewCenter);
	}
}

void World::render(sf::RenderWindow& window)
{
	window.setView(*m_view);

	m_level->render(window);

	m_player->render(window);
	m_enemy->render(window);
	m_enemyFrog->render(window);

	//drawDeadZone(window);
}

void World::drawDeadZone(sf::RenderWindow& window)
{
	// Create a rectangle shape for the dead zone
	sf::RectangleShape deadZoneRect;
	deadZoneRect.setSize({ m_deadZone.width, m_deadZone.height });
	deadZoneRect.setFillColor(sf::Color(0, 0, 255, 50)); // Semi-transparent blue
	deadZoneRect.setOutlineColor(sf::Color::Blue);       // Blue outline
	deadZoneRect.setOutlineThickness(0.5f);

	// Position the rectangle relative to the view's center
	sf::Vector2f viewCenter = m_view->getCenter();
	sf::Vector2f topLeft(
		viewCenter.x - m_deadZone.width / 2.f,
		viewCenter.y - m_deadZone.height / 2.f
	);

	deadZoneRect.setPosition(topLeft);

	// Draw the rectangle
	window.draw(deadZoneRect);
}