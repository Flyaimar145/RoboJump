#include <Core/CollisionManager.h>


CollisionManager* CollisionManager::s_instance{ nullptr };

CollisionManager* CollisionManager::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new CollisionManager();
	}
	return s_instance;
}

void CollisionManager::checkGroundCollision(const ObjectLayer* groundsCollisionLayer, Player* objectToCheckCollision) const
{
	const auto& groundShapes = groundsCollisionLayer->getShapes();
	bool isGrounded = false;
	for (const auto* shape : groundShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckCollision->getBounds()))
		{
			//printf("Speed X: %f, Speed Y: %f \n", objectToCheckCollision->getSpeed().x, objectToCheckCollision->getSpeed().y);
			if (objectToCheckCollision->getSpeed().y > 800.f)
			{
				objectToCheckCollision->setHasTakenDamage(true);
				objectToCheckCollision->setIsDead(true);
			}
			isGrounded = true;
			objectToCheckCollision->setGravity(.0f);
			objectToCheckCollision->setIsJumping(false);
			objectToCheckCollision->setPosition({ objectToCheckCollision->getPosition().x, shape->getGlobalBounds().top - objectToCheckCollision->getBounds().height + 0.1f });
			#if DEBUG_MODE
						//printf("Ground Collision \n");
			#endif
		}
	}
	if (!isGrounded)
	{
		objectToCheckCollision->setGravity(980.f);
	}
}

void CollisionManager::checkWallCollision(const ObjectLayer* wallsCollisionLayer, Player* objectToCheckCollision) const
{
	const auto& wallShapes = wallsCollisionLayer->getShapes();
	bool isCollidingWithWall = false;
	bool collidedLeft = false;
	bool collidedRight = false;
	for (const auto* shape : wallShapes)
	{
		//printf("Player Bounds \n");
		/*
		printf("Left: %f \n", m_player->getBounds().left);
		printf("Top: %f \n", m_player->getBounds().top);
		printf("Right: %f \n", m_player->getBounds().left + m_player->getBounds().width);
		printf("Bottom: %f \n", m_player->getBounds().top + m_player->getBounds().height);
		*/
		if (shape->getGlobalBounds().intersects(objectToCheckCollision->getBounds()))
		{
			sf::FloatRect playerBounds = objectToCheckCollision->getBounds();
			sf::FloatRect wallBounds = shape->getGlobalBounds();

			// Check if the player is moving right and collides with the left side of the wall
			if (objectToCheckCollision->getDirection().x > 0 && playerBounds.left + playerBounds.width > wallBounds.left && playerBounds.left < wallBounds.left)
			{
				objectToCheckCollision->setPosition({ wallBounds.left - playerBounds.width -1.f, objectToCheckCollision->getPosition().y });
				isCollidingWithWall = true;
				collidedLeft = true;
			}
			// Check if the player is moving left and collides with the right side of the wall
			else if (objectToCheckCollision->getDirection().x < 0 && playerBounds.left < wallBounds.left + wallBounds.width && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width)
			{
				objectToCheckCollision->setPosition({ wallBounds.left + wallBounds.width+1.f, objectToCheckCollision->getPosition().y });
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
	sf::Vector2f newDirection = objectToCheckCollision->getDirection();
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
	objectToCheckCollision->setDirection(newDirection);
}

void CollisionManager::checkCeilingCollision(const ObjectLayer* ceilingsCollisionLayer, Player* objectToCheckCollision) const
{
	const auto& ceilingShapes = ceilingsCollisionLayer->getShapes();
	for (const auto* shape : ceilingShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckCollision->getBounds()))
		{
			objectToCheckCollision->setPosition({ objectToCheckCollision->getPosition().x, shape->getGlobalBounds().top + shape->getGlobalBounds().height+2.f });
			objectToCheckCollision->setSpeed({ objectToCheckCollision->getSpeed().x, .0f });
			#if DEBUG_MODE
						//printf("Ceiling Collision\n");
			#endif
		}
	}
}

void CollisionManager::checkGemCollision(const ObjectLayer* gemsCollisionLayer, Player* objectToCheckCollision) const
{
	const auto& gemShapes = gemsCollisionLayer->getShapes();
	for (const auto* shape : gemShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckCollision->getBounds()))
		{
			#if DEBUG_MODE
						//printf("Gem collected \n");
			#endif
		}
	}
}

void CollisionManager::checkCollisionBetweenPlayerAndEnemy(Player* player, Enemy* enemy) const
{
	if (player->getBounds().intersects(enemy->getBounds()))
	{
		//Check if players bottom touches enemy top
		if (player->getBounds().top + player->getBounds().height-5.f < enemy->getBounds().top && player->getSpeed().y > 0.f)
		{
			printf("Player jumped on enemy \n");
			enemy->setPosition({1000.f,1000.f });
			//player->setSpeed({ player->getSpeed().x, -300.f });
		}
		else
		{
			printf("Player damaged by enemy \n");
			player->setHasTakenDamage(true);
		}
		//printf("Player collided with enemy \n");
	}
}

void CollisionManager::checkEnemyWallCollision(const ObjectLayer* wallsCollisionLayer, Enemy* enemy) const
{
	const auto& wallShapes = wallsCollisionLayer->getShapes();
	for (const auto* shape : wallShapes)
	{
		if (shape->getGlobalBounds().intersects(enemy->getBounds()))
		{
			//printf("Enemy collided with wall \n");
			enemy->setDirection({ -enemy->getDirection().x, enemy->getDirection().y });
		}
	}
}