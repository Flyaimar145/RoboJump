#pragma once

#include <Render/SFMLOrthogonalLayer.h>
#include <Gameplay/Player.h>
class CollisionManager
{
public:
	static CollisionManager* getInstance();
	~CollisionManager() = default;
	void checkGroundCollision(const ObjectLayer* groundsCollisionLayer, Player* objectToCheckCollision) const;
	void checkWallCollision(const ObjectLayer* wallsCollisionLayer, Player* objectToCheckCollision) const;
	void checkCeilingCollision(const ObjectLayer* ceilingsCollisionLayer, Player* objectToCheckCollision) const;
	void checkGemCollision(const ObjectLayer* gemsCollisionLayer, Player* objectToCheckCollision) const;
private:
	static CollisionManager* s_instance;
};