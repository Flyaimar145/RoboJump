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
		sf::Texture* texture{ nullptr };
		float tileWidth{ .0f };
		float tileHeight{ .0f };
		sf::Vector2f speed{ .0f, .0f };
		int totalFrames{ 0 };
		int deathAnimationTotalFrames{ 0 };
		int liveCount{ 0 };
	};

	~Entity() override = default;

	bool init(const EntityDescriptor& entityDescriptor);

	sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

	void update(float deltaMilliseconds) override;
	void render(sf::RenderWindow& window) override;
	void updateAnimation(int totalAnimationFrames, float spriteSheetRow);


	sf::Vector2f getSpeed() const { return m_speed; }
	sf::Vector2f getDirection() const { return m_direction; }
	int getLiveCount() const { return m_liveCount; }
	bool getHasTakenDamage() const { return m_hasTakenDamage; }
	bool getIsDead() const { return m_isDead; }

	void setSpeed(sf::Vector2f speed) { m_speed = speed; }
	void setDirection(sf::Vector2f direction) { m_direction = direction; }
	void setLiveCount(int liveCount) { m_liveCount = liveCount; }
	void setHasTakenDamage(bool damageTaken) { m_hasTakenDamage = damageTaken; }
	void setIsDead(bool isDead) { m_isDead = isDead; }

protected:
	
	sf::Sprite m_sprite;
	sf::Vector2f m_direction{ .0f, .0f };
	sf::Vector2f m_speed{ .0f, .0f };

	// Animation
	float m_tileWidth{ .0f };
	float m_tileHeight{ .0f };
	float m_currentSpriteStartingX{ .0f };
	float m_currentSpriteStartingY{ .0f };

	float m_animationTime{ .0f };
	float m_frameDuration{ 50.f };
	int m_currentFrame{ 0 };
	int m_totalFrames{ 0 };
	int m_deathAnimationTotalFrames{ 0 };

	int m_liveCount{ 0 };
	bool m_hasTakenDamage{ false };
	bool m_isDead{ false };
	bool m_liveAmountChanged{ false };
	bool m_damageAnimationStarted{ false };
	bool m_deathAnimationStarted{ false };
};