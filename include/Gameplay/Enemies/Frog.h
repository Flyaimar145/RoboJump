#pragma once

#pragma once

#include <Gameplay/Enemies/Enemy.h>

class Frog : public Enemy
{
	public:
		~Frog() override = default;
		void update(float deltaMilliseconds) override;

	private:
		bool m_hasToAttack{ false };

};