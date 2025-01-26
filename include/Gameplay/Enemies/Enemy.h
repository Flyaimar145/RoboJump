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
			sf::Vector2f initialDirection{ .0f, .0f };
		};

		virtual ~Enemy() override = default;

		bool init(const EnemyDescriptor& enemyDescriptor);

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		bool getCanMakeDamage() const { return m_canMakeDamage; }

		void setCanMakeDamage(bool canMakeDamage) { m_canMakeDamage = canMakeDamage; }

	protected:
		bool m_canMakeDamage{ true };
};