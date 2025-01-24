#include <Core/AssetManager.h>
#include <Core/CollisionManager.h>
#include <Core/World.h>
#include <Gameplay/Player.h>
#include <Gameplay/Entity.h>
#include <Gameplay/Enemies/Enemy.h>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>
#include <Utils/Constants.h>


World::~World()
{
	delete m_player;
	delete m_layerZero;
	delete m_layerOne;
	delete m_layerTwo;
	delete m_groundsLayer;
	delete m_wallsLayer;
	delete m_ceilingsLayer;
	delete m_gemsLayer;
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

	m_groundsLayer = new ObjectLayer(*m_map, 3);
	m_wallsLayer = new ObjectLayer(*m_map, 4);
	m_ceilingsLayer = new ObjectLayer(*m_map, 5);
	m_gemsLayer = new ObjectLayer(*m_map, 6);

	m_layerZero->setOffset({ .0f, .0f });

	m_view = new sf::View(sf::FloatRect({ 0.f, 0.f }, { 960.f, 540.f }));
	// Define the dead zone
	float deadZoneWidth = m_view->getSize().x * 0.15f;
	float deadZoneHeight = m_view->getSize().y * 0.25f;
	float deadZoneX = (m_view->getSize().x - deadZoneWidth) / 2.f;
	float deadZoneY = (m_view->getSize().y - deadZoneHeight) / 2.f;

	// Dead zone is defined relative to the center of the view
	m_deadZone = sf::FloatRect(deadZoneX, deadZoneY, deadZoneWidth, deadZoneHeight);
	//m_view1.setCenter({ createInfo.screenWidth / 4.f, createInfo.screenHeight /4.f });

	//sf::View view1({ 0.f, 0.f, 960.f, 540.f });


	//m_window->setView(m_view);

	// To-Do, read ALL from file, this is just a quick example to understand that here is where entities are created but consider grouping/managing actors in a smarter way
	
	// Player
	sf::Texture* playerTexture = AssetManager::getInstance()->loadTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/1 Main Characters/MainCharacter2.png");
	Player::PlayerDescriptor playerDescriptor;
	playerDescriptor.texture = playerTexture;
	playerDescriptor.position = { 0.f, 0.f };
	playerDescriptor.speed = { 100.f * millisecondsToSeconds, 100.f * millisecondsToSeconds }; // 400 units per second, or 0.4 units per millisecond, using the latter so it's in alignment with delta time
	playerDescriptor.tileWidth = 32.f;
	playerDescriptor.tileHeight = 32.f;
	playerDescriptor.jumpSpeed = 300.f; // 300 units per second, or 0.4 units per millisecond, using the latter so it's in alignment with delta time
	playerDescriptor.totalFrames = 12;
	Player* player = new Player();
	const bool playerLoaded = player->init(playerDescriptor);
	m_player = player;
	player->setPosition({ MAP_TILE_SIZE * 33.f, MAP_TILE_SIZE * 27.f });

	// Enemy
	Entity::EntityDescriptor enemyDescriptor;
	enemyDescriptor.texture = AssetManager::getInstance()->loadTexture("../data/Levels/images/png/craftpix-net-396765-free-simple-platformer-game-kit-pixel-art/4 Enemies/PatrolEnemyTileSet.png");
	enemyDescriptor.position = { MAP_TILE_SIZE * 24.f, MAP_TILE_SIZE * 17.f };
	enemyDescriptor.speed = { 100.f * millisecondsToSeconds, 0.f * millisecondsToSeconds }; // 400 units per second, or 0.4 units per millisecond, using the latter so it's in alignment with delta time
	enemyDescriptor.tileWidth = 32.f;
	enemyDescriptor.tileHeight = 32.f;
	enemyDescriptor.totalFrames = 12;
	Enemy* enemy = new Enemy();
	const bool enemyLoaded = enemy->init(enemyDescriptor);
	m_enemy = enemy;
	enemy->setPosition({ MAP_TILE_SIZE * 24.f, MAP_TILE_SIZE * 17.f });


	return playerLoaded && enemyLoaded;
}

void World::update(uint32_t deltaMilliseconds)
{
	// To-Do: update level
	m_layerZero->update(sf::milliseconds(deltaMilliseconds));
	m_layerOne->update(sf::milliseconds(deltaMilliseconds));
	m_layerTwo->update(sf::milliseconds(deltaMilliseconds));


	// Check for collisions (We could do it in a function here or have a collision manager if it gets complex)
	// Collision management (with CollisionManager)
	CollisionManager::getInstance()->checkGroundCollision(m_groundsLayer, m_player);
	CollisionManager::getInstance()->checkWallCollision(m_wallsLayer, m_player);
	CollisionManager::getInstance()->checkCeilingCollision(m_ceilingsLayer, m_player);
	CollisionManager::getInstance()->checkGemCollision(m_gemsLayer, m_player);

	CollisionManager::getInstance()->checkCollisionBetweenPlayerAndEnemy(m_player, m_enemy);

	// Update player
	m_player->update(deltaMilliseconds);
	//printf("Player Position: %f, %f \n", m_player->getPosition().x, m_player->getPosition().y);

	// Update enemy
	m_enemy->update(deltaMilliseconds);
	//printf("Enemy Position: %f, %f \n", m_enemy->getPosition().x, m_enemy->getPosition().y);

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
	// Collision management (old code; no CollisionManager; keeping it for the moment just in case)
	/*
	const auto& groundShapes = m_groundsLayer->getShapes();
	bool isGrounded = false;
	for (const auto* shape : groundShapes)
	{	
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
		{
			isGrounded = true;
			m_player->setGravity(.0f);
			m_player->setIsJumping(false);
			m_player->setPosition({ m_player->getPosition().x, shape->getGlobalBounds().top - m_player->getBounds().height +1.f});
			#if DEBUG_MODE
				//printf("Ground Collision \n");
			#endif
		}
	}
	if (!isGrounded)
	{
		m_player->setGravity(980.f);
	}
	
	const auto& wallShapes = m_wallsLayer->getShapes();
	bool isCollidingWithWall = false;
	bool collidedLeft = false;
	bool collidedRight = false;
	for (const auto* shape : wallShapes)
	{
		//printf("Player Bounds \n");
		//printf("Left: %f \n", m_player->getBounds().left);
		//printf("Top: %f \n", m_player->getBounds().top);
		//printf("Right: %f \n", m_player->getBounds().left + m_player->getBounds().width);
		//printf("Bottom: %f \n", m_player->getBounds().top + m_player->getBounds().height);
		
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
		{
			sf::FloatRect playerBounds = m_player->getBounds();
			sf::FloatRect wallBounds = shape->getGlobalBounds();

			// Check if the player is moving right and collides with the left side of the wall
			if (m_player->getDirection().x > 0 && playerBounds.left + playerBounds.width > wallBounds.left && playerBounds.left < wallBounds.left)
			{
				m_player->setPosition({ wallBounds.left - playerBounds.width, m_player->getPosition().y });
				isCollidingWithWall = true;
				collidedLeft = true;
			}
			// Check if the player is moving left and collides with the right side of the wall
			else if (m_player->getDirection().x < 0 && playerBounds.left < wallBounds.left + wallBounds.width && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width)
			{
				m_player->setPosition({ wallBounds.left + wallBounds.width, m_player->getPosition().y });
				isCollidingWithWall = true;
				collidedRight = true;

			}
			#if DEBUG_MODE
			//printf("Wall Collision \n");
			//printf("Collided Left: %d, Collided Right: %d", collidedLeft, collidedRight);
			#endif
		}
	}
	// Allow movement away from the wall
	sf::Vector2f newDirection = m_player->getDirection();
	// If collidedLeft, disallow left movement (negative x-direction)
	if (collidedLeft && newDirection.x < 0)
	{
		newDirection.x = 0.f;
	}
	// If collidedRight, disallow right movement (positive x-direction)
	if (collidedRight && newDirection.x > 0)
	{
		newDirection.x = 0.f;
	}
	// Update the player's direction with the new direction
	m_player->setDirection(newDirection);
	
	const auto& ceilingShapes = m_ceilingsLayer->getShapes();
	for (const auto* shape : ceilingShapes)
	{
		if (shape->getGlobalBounds().intersects(m_player->getBounds()))
		{
			m_player->setPosition({ m_player->getPosition().x, shape->getGlobalBounds().top + shape->getGlobalBounds().height });
			m_player->setSpeed({ m_player->getSpeed().x, .0f });
		#if DEBUG_MODE
					//printf("Ceiling Collision\n");
		#endif
		}
	}
	
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
	*/
}

void World::render(sf::RenderWindow& window)
{
	window.setView(*m_view);
	window.draw(*m_layerZero);
	window.draw(*m_layerOne);
	window.draw(*m_layerTwo);
	window.draw(*m_groundsLayer);
	window.draw(*m_wallsLayer);
	window.draw(*m_ceilingsLayer);
	window.draw(*m_gemsLayer);
	m_player->render(window);
	m_enemy->render(window);

	drawDeadZone(window);
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