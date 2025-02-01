#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Entity : public GameObject
{
	public:

		struct EntityDescriptor
		{
			sf::Vector2f position{ .0f, .0f };
			sf::Vector2f speed{ .0f, .0f };

			sf::Texture* firstTexture{ nullptr };
			float tileWidth{ .0f };
			float tileHeight{ .0f };
			float offsetForAdjustedBoundsLeft{ 0.f };
			float offsetForAdjustedBoundsTop{ 0.f };
			float offsetForAdjustedBoundsWidth{ 0.f };
			float offsetForAdjustedBoundsHeight{ 0.f };

			int totalFrames{ 0 };
			int deathAnimationTotalFrames{ 0 };

			int lifeCount{ 0 };
		};

		virtual ~Entity() override = default;

		bool init(const EntityDescriptor& entityDescriptor);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }
		sf::FloatRect getAdjustedBounds() const { return sf::FloatRect(m_sprite.getGlobalBounds().left + m_offsetForAdjustedBoundsLeft, m_sprite.getGlobalBounds().top + m_offsetForAdjustedBoundsTop, m_sprite.getGlobalBounds().width - m_offsetForAdjustedBoundsWidth, m_sprite.getGlobalBounds().height - m_offsetForAdjustedBoundsHeight); }

		sf::Vector2f getAdjustedPosition() const { return sf::Vector2f(m_sprite.getPosition().x + m_offsetForAdjustedBoundsLeft, m_sprite.getPosition().y + m_offsetForAdjustedBoundsTop); }
		void setAdjustedPosition(sf::Vector2f position) { setPosition({ position.x - m_offsetForAdjustedBoundsLeft, position.y - m_offsetForAdjustedBoundsTop }); }

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;
		void updateAnimation(int totalAnimationFrames, float spriteSheetRow);

		sf::Vector2f getSpeed() const { return m_speed; }
		sf::Vector2f getDirection() const { return m_direction; }
		int getLifeCount() const { return m_lifeCount; }
		bool getHasTakenDamage() const { return m_hasTakenDamage; }
		bool getIsDead() const { return m_isDead; }
		bool getHasFinishedDying() const { return m_hasFinishedDying; }

		void setSpeed(sf::Vector2f speed) { m_speed = speed; }
		void setDirection(sf::Vector2f direction) { m_direction = direction; }
		void setLifeCount(int lifeCount) { m_lifeCount = lifeCount; }
		void setHasTakenDamage(bool damageTaken) { m_hasTakenDamage = damageTaken; }
		void setIsDead(bool isDead) { m_isDead = isDead; }

	protected:
	
		sf::Vector2f m_direction{ .0f, .0f };
		sf::Vector2f m_speed{ .0f, .0f };

		sf::Sprite m_sprite;
		float m_tileWidth{ .0f };
		float m_tileHeight{ .0f };
		float m_offsetForAdjustedBoundsLeft{ 0.f };
		float m_offsetForAdjustedBoundsTop{ 0.f };
		float m_offsetForAdjustedBoundsWidth{ 0.f };
		float m_offsetForAdjustedBoundsHeight{ 0.f };

		float m_currentSpriteStartingX{ .0f };
		float m_currentSpriteStartingY{ .0f };
		float m_animationTime{ .0f };
		float m_frameDuration{ 50.f };
		int m_currentFrame{ 0 };
		int m_totalFrames{ 0 };
		int m_deathAnimationTotalFrames{ 0 };
		bool m_damageAnimationStarted{ false };
		bool m_deathAnimationStarted{ false };

		int m_lifeCount{ 0 };
		bool m_hasTakenDamage{ false };
		bool m_hasFinishedDying{ false };
		bool m_isDead{ false };
		bool m_liveAmountChanged{ false };
};