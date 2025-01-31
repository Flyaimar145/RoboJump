#include <Core/CollisionManager.h>
#include <Core/Level.h>
#include <Core/World.h>
#include <Core/EnemyManager.h>
#include <Core/PickUpManager.h>
#include <Gameplay/Player.h>
#include <Gameplay/Enemies/Enemy.h>
#include <Gameplay/Enemies/Cactus.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


World::~World()
{
	delete m_player;
	m_player = nullptr;

	delete m_enemyManager;
	m_enemyManager = nullptr;

	delete m_pickUpManager;
	m_pickUpManager = nullptr;
	
	delete m_view;
	m_view = nullptr;

	delete m_level;
	m_level = nullptr;
}

bool World::load()
{
	m_level = new Level();
	const bool levelLoaded = m_level->load();

	m_view = new sf::View(sf::FloatRect({ 0.f, 0.f }, { 960.f, 540.f }));
	float deadZoneWidth = m_view->getSize().x * 0.15f;
	float deadZoneHeight = m_view->getSize().y * 0.15f;
	float deadZoneX = (m_view->getSize().x - deadZoneWidth) / 2.f;
	float deadZoneY = (m_view->getSize().y - deadZoneHeight) / 2.f;
	m_deadZone = sf::FloatRect(deadZoneX, deadZoneY, deadZoneWidth, deadZoneHeight);

	m_pickUpManager = new PickUpManager();
	const bool pickUpsLoaded = m_pickUpManager->loadPickUps();

	Player* player = new Player();
	Player::PlayerDescriptor playerDescriptor2 = player->load();
	const bool playerLoaded = player->init(playerDescriptor2);
	m_player = player;

	m_enemyManager = new EnemyManager();
	const bool enemiesLoaded = m_enemyManager->loadEnemies();

	return playerLoaded && enemiesLoaded && pickUpsLoaded;
}

void World::update(uint32_t deltaMilliseconds)
{
	m_level->update(deltaMilliseconds);

	m_pickUpManager->update(deltaMilliseconds);

	m_player->update(deltaMilliseconds);
	checkPlayerEnvironmentCollisions();
	checkPlayerEnemiesCollisions();
	checkPlayerPickUpsCollisions();
	checkPlayerDeath();
	
	updateDeadZone();

	m_enemyManager->update(deltaMilliseconds);
	checkSpecialsForCactusEnemies();
	checkSpecialsForStompEnemies();
}

void World::render(sf::RenderWindow& window)
{
	window.setView(*m_view);

	m_level->render(window);

	m_player->render(window);

	m_pickUpManager->render(window);

	m_enemyManager->render(window);

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

void World::updateDeadZone()
{
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

void World::checkPlayerEnvironmentCollisions()
{
	bool isGrounded = false;
	const sf::Shape* collidedGroundShape = CollisionManager::getInstance()->checkGroundCollision(m_level->getGroundsLayer(), m_player);
	if (collidedGroundShape != nullptr)
	{
		if (m_player->getSpeed().y > 800.f)
		{
			m_player->setHasTakenDamage(true);
			m_player->setIsDead(true);
		}
		isGrounded = true;
		m_player->setGravity(.0f);
		m_player->setIsJumping(false);
		m_player->setAdjustedPosition({ m_player->getAdjustedPosition().x, collidedGroundShape->getGlobalBounds().top - m_player->getAdjustedBounds().height + 0.1f });
	}
	if (!isGrounded)
	{
		m_player->setGravity(980.f);
	}


	bool isCollidingWithWall = false;
	bool collidedLeft = false;
	bool collidedRight = false;
	const sf::Shape* collidedWallShape = CollisionManager::getInstance()->checkWallCollision(m_level->getWallsLayer(), m_player);
	if (collidedWallShape != nullptr)
	{
		sf::FloatRect playerBounds = m_player->getAdjustedBounds();
		sf::FloatRect wallBounds = collidedWallShape->getGlobalBounds();

		if (m_player->getDirection().x > 0 && playerBounds.left + playerBounds.width > wallBounds.left && playerBounds.left < wallBounds.left)
		{
			m_player->setAdjustedPosition({ wallBounds.left - playerBounds.width - 1.f, m_player->getAdjustedPosition().y });
			isCollidingWithWall = true;
			collidedLeft = true;
		}
		else if (m_player->getDirection().x < 0 && playerBounds.left < wallBounds.left + wallBounds.width && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width)
		{
			m_player->setAdjustedPosition({ wallBounds.left + wallBounds.width + 1.f, m_player->getAdjustedPosition().y });
			isCollidingWithWall = true;
			collidedRight = true;

		}
	}
	sf::Vector2f newDirection = m_player->getDirection();
	if (collidedLeft && newDirection.x < 0)
	{
		newDirection.x = 0.f;
	}
	if (collidedRight && newDirection.x > 0)
	{
		newDirection.x = 0.f;
	}
	m_player->setDirection(newDirection);


	const sf::Shape* collidedCeilingShape = CollisionManager::getInstance()->checkCeilingCollision(m_level->getCeilingsLayer(), m_player);
	if (collidedCeilingShape != nullptr)
	{
		m_player->setAdjustedPosition({ m_player->getAdjustedPosition().x, collidedCeilingShape->getGlobalBounds().top + collidedCeilingShape->getGlobalBounds().height + 2.f });
		m_player->setSpeed({ m_player->getSpeed().x, .0f });
	}


	const sf::Shape* collidedTrapShape = CollisionManager::getInstance()->checkTrapCollision(m_level->getTrapsLayer(), m_player);
	if (collidedTrapShape != nullptr)
	{
		m_player->setHasTakenDamage(true);
	}
	
	
		
}

void World::checkPlayerEnemiesCollisions()
{
	for (Enemy* enemy : m_enemyManager->getEnemiesVector())
	{
		bool isCollidingWithEnemy = CollisionManager::getInstance()->checkCollisionBetweenPlayerAndEnemy(m_player, enemy);
		if (isCollidingWithEnemy)
		{
			switch (enemy->getEnemyType())
			{
			case Enemy::EnemyType::Cactus:
				//printf("Player touched a Cactus \n");
				if (m_player->getAdjustedBounds().top + m_player->getAdjustedBounds().height - 5.f < enemy->getAdjustedBounds().top && m_player->getSpeed().y > 0.f)
				{
					m_player->setMakeJump(true);
					enemy->setHasTakenDamage(true);
					//enemy->setCanMakeDamage(false);
				}
				else
				{
					if (enemy->getCanMakeDamage())
					{
						m_player->setHasTakenDamage(true);
					}
				}
				break;

			case Enemy::EnemyType::Frog:
				//printf("Player touched a Frog \n");
				enemy->onPlayerCollision();
				if (enemy->getCanMakeDamage())
				{
					m_player->setHasTakenDamage(true);
				}
				break;

			case Enemy::EnemyType::Stomp:
				//printf("Player touched a Stomp \n");
				if (enemy->getCanMakeDamage())
				{
					m_player->setHasTakenDamage(true);
				}
				break;

			default:

				break;

			}
		}
	}
}

void World::checkPlayerPickUpsCollisions()
{
	PickUp* collidedPickUp = CollisionManager::getInstance()->checkCollisionBetweenPlayerAndPickUp(m_player, m_pickUpManager->getPickUpsVector());
	if (collidedPickUp != nullptr)
	{
		switch (collidedPickUp->getPickUpType())
		{
			case PickUp::PickUpType::Gem:
			{
				int gemValue = collidedPickUp->returnInfoOnPlayerCollision();
				m_player->addScore(gemValue);
				break;
			}
			case PickUp::PickUpType::PowerUp:
			{
				int powerUpType = collidedPickUp->returnInfoOnPlayerCollision();
				switch (powerUpType)
				{
					case 0:
						m_player->addLife();
						break;
					case 1:
						m_player->applySpeedBoost();
						break;
					default:
						break;
				}
				break;
			}
			default:
				printf("Player touched an unknown pickup \n");
			break;
		}
		m_pickUpManager->destroyPickUp(collidedPickUp);
	}
	
}

void World::checkPlayerDeath()
{
	if (m_player->getHasFinishedDying())
	{
		m_isPlayerDead = true;
	}
}

void World::checkSpecialsForCactusEnemies() 
{
	for (Cactus* cactus : m_enemyManager->getCactusTypeEnemiesVector())
	{
		CollisionManager::getInstance()->checkEnemyWallCollision(m_level->getEnemyWallsLayer(), cactus);

		if (cactus->getHasFinishedDying())
		{
			m_enemyManager->destroyCactus(cactus);
		}
	}
}

void World::checkSpecialsForStompEnemies()
{
	for (Stomp* stomp : m_enemyManager->getStompTypeEnemiesVector())
	{
		CollisionManager::getInstance()->checkEnemyWallCollision(m_level->getEnemyWallsLayer(), stomp);

		if (stomp->getDetectionZone().intersects(m_player->getAdjustedBounds()))
		{
			stomp->onPlayerCollision();
		}
	}
}