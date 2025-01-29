#pragma once

#include <Gameplay/Entity.h>

class Enemy : public Entity
{
	public:

		struct EnemyDescriptor : public EntityDescriptor
		{
			sf::Vector2f initialDirection{ .0f, .0f };
		};

		enum class EnemyType
		{
			Cactus,
			Frog,
			Stomp
		};

		virtual ~Enemy() override = default;

		virtual bool init(const EnemyDescriptor& enemyDescriptor);
		virtual void onPlayerCollision() {};

		virtual void update(float deltaMilliseconds) override;
		virtual void render(sf::RenderWindow& window) override;

		bool getCanMakeDamage() const { return m_canMakeDamage; }
		Enemy::EnemyType getEnemyType() const { return m_enemyType; }

		void setCanMakeDamage(bool canMakeDamage) { m_canMakeDamage = canMakeDamage; }
		void setEnemyType(Enemy::EnemyType enemyType) { m_enemyType = enemyType; }

	protected:

		EnemyType m_enemyType;
		bool m_canMakeDamage{ true };
};