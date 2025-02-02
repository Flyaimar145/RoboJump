#pragma once

#include <Gameplay/Enemies/Enemy.h>
#include <SFML/Graphics/RectangleShape.hpp>

class Stomp : public Enemy
{
	public:

		~Stomp() override = default;

		bool init(const EnemyDescriptor& enemyDescriptor) override;
		void onPlayerCollision() override;

		void update(float deltaMilliseconds) override;

		sf::FloatRect getDetectionZone() const { return m_detectionZone; }

	private:

		float m_fallingSpeed{ 100.0f };
		float m_heightToFall{ 48.0f };
		float m_initialPositionY{ .0f };
		float m_originalSpeedX{ .0f };

		bool m_hasToAttack{ false };
		bool m_hasToRise{ false };
		bool m_attackAnimationStarted{ false };

		sf::FloatRect m_detectionZone;
};