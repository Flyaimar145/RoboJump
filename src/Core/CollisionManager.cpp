#include <Core/CollisionManager.h>
#include <Gameplay/Enemies/Enemy.h>
#include <Gameplay/PickUp.h>
#include <Gameplay/Player.h>
#include <Render/MapLayer.h>
#include <Render/ObjectLayer.h>


CollisionManager* CollisionManager::s_instance{ nullptr };

CollisionManager* CollisionManager::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new CollisionManager();
	}
	return s_instance;
}

const sf::Shape* CollisionManager::checkGroundCollision(const ObjectLayer* groundsCollisionLayer, Player* objectToCheckCollision) const
{
	const auto& groundShapes = groundsCollisionLayer->getShapes();
	for (const auto* shape : groundShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckCollision->getAdjustedBounds()))
		{
			return shape;
		}
	}
	return nullptr;
}

const sf::Shape* CollisionManager::checkWallCollision(const ObjectLayer* wallsCollisionLayer, Player* objectToCheckCollision) const
{
	const auto& wallShapes = wallsCollisionLayer->getShapes();
	for (const auto* shape : wallShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckCollision->getAdjustedBounds()))
		{
			return shape;
		}
	}
	return nullptr;
}

const sf::Shape* CollisionManager::checkCeilingCollision(const ObjectLayer* ceilingsCollisionLayer, Player* objectToCheckCollision) const
{
	const auto& ceilingShapes = ceilingsCollisionLayer->getShapes();
	for (const auto* shape : ceilingShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckCollision->getAdjustedBounds()))
		{
			return shape;
		}
	}
	return nullptr;
}

const sf::Shape* CollisionManager::checkTrapCollision(const ObjectLayer* trapsCollisionLayer, Player* objectToCheckCollision) const
{
	const auto& trapShapes = trapsCollisionLayer->getShapes();
	for (const auto* shape : trapShapes)
	{
		if (shape->getGlobalBounds().intersects(objectToCheckCollision->getAdjustedBounds()))
		{
			return shape;
		}
	}
	return nullptr;
}

bool CollisionManager::checkCollisionBetweenPlayerAndEnemy(Player* player, Enemy* enemy) const
{
	if (player->getAdjustedBounds().intersects(enemy->getAdjustedBounds()))
	{
		return true;
	}
	return false;
}

PickUp* CollisionManager::checkCollisionBetweenPlayerAndPickUp(Player* player, std::vector<PickUp*> pickUps) const
{
	for (PickUp* pickUp : pickUps)
	{
		if (player->getAdjustedBounds().intersects(pickUp->getBounds()))
		{
			return pickUp;
		}
	}
	return nullptr;
}

void CollisionManager::checkEnemyWallCollision(const ObjectLayer* wallsCollisionLayer, Enemy* enemy) const
{
	const auto& wallShapes = wallsCollisionLayer->getShapes();
	for (const auto* shape : wallShapes)
	{
		if (shape->getGlobalBounds().intersects(enemy->getAdjustedBounds()))
		{
			enemy->setDirection({ -enemy->getDirection().x, enemy->getDirection().y });
		}
	}
}