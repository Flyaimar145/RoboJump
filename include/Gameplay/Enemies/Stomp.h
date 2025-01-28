#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <Gameplay/Enemies/Enemy.h>

class Stomp : public Enemy
{
public:
	~Stomp() override = default;

	bool init(const EnemyDescriptor& enemyDescriptor) override;
	void onPlayerCollision() override;

	void update(float deltaMilliseconds) override;
	void render(sf::RenderWindow& window) override;

	bool getHasToAttack() const { return m_hasToAttack; }
	sf::FloatRect getDetectionZone() const { return m_detectionZone; }

	void setHasToAttack(bool hasToAttack) { m_hasToAttack = hasToAttack; }

private:
	bool m_hasToAttack{ false };
	bool m_hasToRise{ false };
	bool m_attackAnimationStarted{ false };

	float m_fallingSpeed{ 100.0f };
	float m_heightToFall{ 48.0f };
	float m_initialPositionY{ .0f };
	float m_beforeFallingDirectionX{ .0f };

	sf::FloatRect m_detectionZone;
};