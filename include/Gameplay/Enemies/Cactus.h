#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <Gameplay/Enemies/Enemy.h>

class Cactus : public Enemy
{
	public:
		~Cactus() override = default;

		bool init(const EnemyDescriptor& enemyDescriptor) override;

		void update(float deltaMilliseconds) override;
};