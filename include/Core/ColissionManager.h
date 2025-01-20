#pragma once

#include <Render/SFMLOrthogonalLayer.h>
#include <Gameplay/Player.h>
class ColissionManager
{
public:
	static ColissionManager* getInstance();
	~ColissionManager() = default;
	void checkGroundColission(const ObjectLayer* groundsColissionLayer, Player* objectToCheckColission) const;
	void checkWallColission(const ObjectLayer* wallsColissionLayer, Player* objectToCheckColission) const;
	void checkCeilingColission(const ObjectLayer* ceilingsColissionLayer, Player* objectToCheckColission) const;
	void checkGemColission(const ObjectLayer* gemsColissionLayer, Player* objectToCheckColission) const;
private:
	static ColissionManager* s_instance;
};