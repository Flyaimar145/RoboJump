#include <Core/ColissionManager.h>


ColissionManager* ColissionManager::s_instance{ nullptr };

ColissionManager* ColissionManager::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new ColissionManager();
	}
	return s_instance;
}

void ColissionManager::checkGroundColission(const ObjectLayer* groundsColissionLayer, Player* objectToCheckColission) const
{
	const auto& groundShapes = groundsColissionLayer->getShapes();
	bool isGrounded = false;
	for (const auto* shape : groundShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckColission->getBounds()))
		{
			isGrounded = true;
			objectToCheckColission->setGravity(.0f);
			objectToCheckColission->setIsJumping(false);
			objectToCheckColission->setPosition({ objectToCheckColission->getPosition().x, shape->getGlobalBounds().top - objectToCheckColission->getBounds().height + 1.f });
			#if DEBUG_MODE
						//printf("Ground Collision \n");
			#endif
		}
	}
	if (!isGrounded)
	{
		objectToCheckColission->setGravity(980.f);
	}
}

void ColissionManager::checkWallColission(const ObjectLayer* wallsColissionLayer, Player* objectToCheckColission) const
{
	const auto& wallShapes = wallsColissionLayer->getShapes();
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
		if (shape->getGlobalBounds().intersects(objectToCheckColission->getBounds()))
		{
			sf::FloatRect playerBounds = objectToCheckColission->getBounds();
			sf::FloatRect wallBounds = shape->getGlobalBounds();

			// Check if the player is moving right and collides with the left side of the wall
			if (objectToCheckColission->getDirection().x > 0 && playerBounds.left + playerBounds.width > wallBounds.left && playerBounds.left < wallBounds.left)
			{
				objectToCheckColission->setPosition({ wallBounds.left - playerBounds.width, objectToCheckColission->getPosition().y });
				isCollidingWithWall = true;
				collidedLeft = true;
			}
			// Check if the player is moving left and collides with the right side of the wall
			else if (objectToCheckColission->getDirection().x < 0 && playerBounds.left < wallBounds.left + wallBounds.width && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width)
			{
				objectToCheckColission->setPosition({ wallBounds.left + wallBounds.width, objectToCheckColission->getPosition().y });
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
	sf::Vector2f newDirection = objectToCheckColission->getDirection();
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
	objectToCheckColission->setDirection(newDirection);
}

void ColissionManager::checkCeilingColission(const ObjectLayer* ceilingsColissionLayer, Player* objectToCheckColission) const
{
	const auto& ceilingShapes = ceilingsColissionLayer->getShapes();
	for (const auto* shape : ceilingShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckColission->getBounds()))
		{
			objectToCheckColission->setPosition({ objectToCheckColission->getPosition().x, shape->getGlobalBounds().top + shape->getGlobalBounds().height });
			objectToCheckColission->setSpeed({ objectToCheckColission->getSpeed().x, .0f });
			#if DEBUG_MODE
						//printf("Ceiling Colission\n");
			#endif
		}
	}
}

void ColissionManager::checkGemColission(const ObjectLayer* gemsColissionLayer, Player* objectToCheckColission) const
{
	const auto& gemShapes = gemsColissionLayer->getShapes();
	for (const auto* shape : gemShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckColission->getBounds()))
		{
			#if DEBUG_MODE
						//printf("Gem collected \n");
			#endif
		}
	}
}
