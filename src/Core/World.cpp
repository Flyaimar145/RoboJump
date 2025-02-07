#include <Core/AudioManager.h>
#include <Core/CollisionManager.h>
#include <Core/EnemyManager.h>
#include <Core/Level.h>
#include <Core/PickUpManager.h>
#include <Core/World.h>
#include <External/json.hpp>
#include <Gameplay/Enemies/Cactus.h>
#include <Gameplay/Enemies/Enemy.h>
#include <Gameplay/Enemies/Stomp.h>
#include <Gameplay/PickUp.h>
#include <Gameplay/Player.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <UI/HUD.h>
#include <Utils/Constants.h>

using json = nlohmann::json;

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

	delete m_hud;
	m_hud = nullptr;

	delete m_hudView;
	m_hudView = nullptr;
}

bool World::load()
{
	json gameConfigInfo = loadJsonFromFile(GAMEINFOJSON_CONFIG)["GameInfo"];

	m_level = new Level();
	const bool levelLoaded = m_level->load();

	m_view = new sf::View(sf::FloatRect({ 0.f, 0.f }, { gameConfigInfo["screenWidth"].get<float>() / 2, gameConfigInfo["screenHeight"].get<float>() / 2 }));
	float deadZoneWidth = m_view->getSize().x * 0.15f;
	float deadZoneHeight = m_view->getSize().y * 0.15f;
	float deadZoneX = (m_view->getSize().x - deadZoneWidth) / 2.f;
	float deadZoneY = (m_view->getSize().y - deadZoneHeight) / 2.f;
	m_deadZone = sf::FloatRect(deadZoneX, deadZoneY, deadZoneWidth, deadZoneHeight);

	m_hud = new HUD();
	const bool hudLoaded = m_hud->init();
	m_hudView = new sf::View(sf::FloatRect({ 0.f, 0.f }, m_view->getSize()));

	m_pickUpManager = new PickUpManager();
	const bool pickUpsLoaded = m_pickUpManager->loadPickUps();

	m_enemyManager = new EnemyManager();
	const bool enemiesLoaded = m_enemyManager->loadEnemies();

	Player* player = new Player();
	Player::PlayerDescriptor playerDescriptor = player->load();
	const bool playerLoaded = player->init(playerDescriptor);
	m_player = player;

	return playerLoaded && enemiesLoaded && pickUpsLoaded && levelLoaded && hudLoaded;
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
	checkIfPlayerHasReachedVictory();
	
	updateDeadZone();
	m_hud->update(m_player->getScore());

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

	//This is necessary to make the HUD follow the camera. First we save the current view, 
	//then we set the view to the HUD view, render the HUD and then we set the view back to the original view
	const sf::View originalView = window.getView();
	window.setView(*m_hudView);
	m_hud->render(window);
	window.setView(originalView);
}

void World::updateDeadZone()
{
	sf::Vector2f playerPos = m_player->getPosition();
	sf::Vector2f viewCenter = m_view->getCenter();
	if (!m_player->getIsDead())
	{
		sf::FloatRect viewDeadZone(viewCenter.x - m_deadZone.width / 2.f, viewCenter.y - m_deadZone.height / 2.f, m_deadZone.width, m_deadZone.height);
		if (playerPos.x < viewDeadZone.left) 
		{
			viewCenter.x = playerPos.x + m_deadZone.width / 2.f;
		}
		else if (playerPos.x > viewDeadZone.left + m_deadZone.width) 
		{
			viewCenter.x = playerPos.x - m_deadZone.width / 2.f;
		}

		if (playerPos.y < viewDeadZone.top) 
		{
			viewCenter.y = playerPos.y + m_deadZone.height / 2.f;
		}
		else if (playerPos.y > viewDeadZone.top + m_deadZone.height) 
		{
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
					if (m_player->getAdjustedBounds().top + m_player->getAdjustedBounds().height - 5.f < enemy->getAdjustedBounds().top && m_player->getSpeed().y > 0.f)
					{
						m_player->setMakeJump(true);
						enemy->setHasTakenDamage(true);
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
					enemy->onPlayerCollision();
					if (enemy->getCanMakeDamage())
					{
						m_player->setHasTakenDamage(true);
					}
					break;

				case Enemy::EnemyType::Stomp:
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
				AudioManager::getInstance()->playSound(SoundType::GemCollected);
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
						AudioManager::getInstance()->playSound(SoundType::ExtraLifeCollected);
						m_player->addLife();
						break;
					case 1:
						AudioManager::getInstance()->playSound(SoundType::SpeedBoostCollected);
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

void World::checkIfPlayerHasReachedVictory()
{
	if (m_player->getAdjustedBounds().intersects(m_level->getVictoryZone()))
	{
		m_playerHasReachedVictory = true;
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