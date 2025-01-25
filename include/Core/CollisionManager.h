#pragma once

#include <Render/SFMLOrthogonalLayer.h>
#include <Gameplay/Entity.h>
#include <Gameplay/Player.h>
#include <Gameplay/Enemies/Enemy.h>
class CollisionManager
{
public:
	static CollisionManager* getInstance();
	~CollisionManager() = default;
	void checkGroundCollision(const ObjectLayer* groundsCollisionLayer, Player* objectToCheckCollision) const;
	void checkWallCollision(const ObjectLayer* wallsCollisionLayer, Player* objectToCheckCollision) const;
	void checkCeilingCollision(const ObjectLayer* ceilingsCollisionLayer, Player* objectToCheckCollision) const;
	void checkTrapCollision(const ObjectLayer* trapsCollisionLayer, Player* objectToCheckCollision) const;
	void checkCollisionBetweenPlayerAndEnemy(Player* player, Enemy* enemy) const;
	void checkEnemyWallCollision(const ObjectLayer* wallsCollisionLayer, Enemy* objectToCheckCollision) const;
private:
	static CollisionManager* s_instance;
};