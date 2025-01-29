#pragma once

#include <Render/SFMLOrthogonalLayer.h>
#include <Gameplay/Entity.h>
#include <Gameplay/Player.h>
#include <Gameplay/Gem.h>
#include <Gameplay/PickUp.h>
#include <Gameplay/Enemies/Enemy.h>
#include <Gameplay/Enemies/Cactus.h>
#include <Gameplay/Enemies/Frog.h>

class CollisionManager
{
public:
	static CollisionManager* getInstance();
	~CollisionManager() = default;
	const sf::Shape* checkGroundCollision(const ObjectLayer* groundsCollisionLayer, Player* objectToCheckCollision) const;
	const sf::Shape* checkWallCollision(const ObjectLayer* wallsCollisionLayer, Player* objectToCheckCollision) const;
	const sf::Shape* checkCeilingCollision(const ObjectLayer* ceilingsCollisionLayer, Player* objectToCheckCollision) const;
	const sf::Shape* checkTrapCollision(const ObjectLayer* trapsCollisionLayer, Player* objectToCheckCollision) const;
	bool checkCollisionBetweenPlayerAndEnemy(Player* player, Enemy* enemy) const;
	PickUp* checkCollisionBetweenPlayerAndPickUp(Player* player, std::vector<PickUp*>) const;
	void checkEnemyWallCollision(const ObjectLayer* wallsCollisionLayer, Enemy* objectToCheckCollision) const;
private:
	static CollisionManager* s_instance;
};