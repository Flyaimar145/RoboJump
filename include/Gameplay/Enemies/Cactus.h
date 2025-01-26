#pragma once

#include <Gameplay/Enemies/Enemy.h>

class Cactus : public Enemy
{
	public:
		~Cactus() override = default;
		void update(float deltaMilliseconds) override;
};