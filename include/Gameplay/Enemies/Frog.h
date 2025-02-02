#pragma once

#include <Gameplay/Enemies/Enemy.h>

class Frog : public Enemy
{
	public:

		~Frog() override = default;

		bool init(const EnemyDescriptor& enemyDescriptor) override;
		void onPlayerCollision() override;

		void update(float deltaMilliseconds) override;

	private:

		bool m_hasToAttack{ false };
		bool m_attackAnimationStarted{ false };
};