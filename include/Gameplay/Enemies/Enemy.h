#pragma once

#include <Gameplay/GameObject.h>
#include <Gameplay/Entity.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Enemy : public Entity
{
	public:

		struct EnemyDescriptor : public EntityDescriptor
		{
			sf::Vector2f direction{ .0f, .0f };
		};

		~Enemy() override = default;

		bool init(const EnemyDescriptor& enemyDescriptor);

		void updateAnimation(int totalAnimationFrames, float spriteSheetRow);

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		int getLiveCount() const { return m_liveCount; }
		bool getHasTakenDamage() const { return m_hasTakenDamage; }
		bool getIsDead() const { return m_isDead; }
		bool getCanMakeDamage() const { return m_canMakeDamage; }

		void setCanMakeDamage(bool canMakeDamage) { m_canMakeDamage = canMakeDamage; }
		void setLiveCount(int liveCount) { m_liveCount = liveCount; }
		void setIsDead(bool isDead) { m_isDead = isDead; }
		void setHasTakenDamage(bool damageTaken) { m_hasTakenDamage = damageTaken; }

	protected:
		bool m_canMakeDamage{ true };
		bool m_hasTakenDamage{ false };
		bool m_damageAnimationStarted{ false };
		bool m_deathAnimationStarted{ false };
		bool m_liveAmountChanged{ false };
		int m_liveCount{ 1 };
		bool m_isDead{ false };
		int m_deathAnimationTotalFrames{ 7 };
};