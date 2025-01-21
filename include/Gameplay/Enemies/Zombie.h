#pragma once

#include <Gameplay/Enemies/Enemy.h>


class Zombie //:// public Enemy
{
	public:

		/*struct ZombieDescriptor : EnemyDescriptor
		{
			sf::Vector2f speed{ .0f, .0f };
		};*/

		//bool init(const ZombieDescriptor& enemyDescriptor);

		void update(float deltaMilliseconds);// override;


	private:

		sf::Vector2f m_direction{ .0f, .0f };
		sf::Vector2f m_speed{ .0f, .0f };
		float m_animationTime{ .0f };
		float m_frameDuration{ 50.f };
		int m_currentFrame{ 0 };
		int m_totalFrames{ 12 };
};