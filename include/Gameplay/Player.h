#pragma once

#include <Gameplay/GameObject.h>
#include <Gameplay/Entity.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Player : public Entity
{
public:

	struct PlayerDescriptor: public EntityDescriptor
	{
		//sf::Vector2f position;
		//sf::Texture* texture{ nullptr };
		//float tileWidth{ .0f };
		//float tileHeight{ .0f };
		//sf::Vector2f speed{ .0f, .0f };
		float jumpSpeed{ 0.0f };
	};

	~Player() override = default;

	bool init(const PlayerDescriptor& playerDescriptor);

	//sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }
	//sf::FloatRect getBounds() const override{ return sf::FloatRect(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top, m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height); }
	sf::FloatRect getAdjustedBounds() const { return sf::FloatRect(m_sprite.getGlobalBounds().left+6.f, m_sprite.getGlobalBounds().top+8.f, m_sprite.getGlobalBounds().width-12.f, m_sprite.getGlobalBounds().height-8.f); }
	sf::Vector2f getAdjustedPosition() const { return sf::Vector2f(m_sprite.getPosition().x + 6.f, m_sprite.getPosition().y + 8.f); }
	void updateAnimation(int totalAnimationFrames, float spriteSheetRow);

	void update(float deltaMilliseconds) override;
	void render(sf::RenderWindow& window) override;

	//sf::Vector2f getSpeed() const { return m_speed; }
	//sf::Vector2f getDirection() const { return m_direction; }
	bool getIsJumping() const { return m_isJumping; }
	float getJumpSpeed() const { return m_jumpSpeed; }
	float getGravity() const { return m_currentGravity; }
	int getLiveCount() const { return m_liveCount; }
	bool getHasTakenDamage() const { return m_hasTakenDamage; }
	bool getIsDead() const { return m_isDead; }

	void setAdjustedPosition(sf::Vector2f position) { setPosition({ position.x - 6.f, position.y - 8.f }); }
	void setIsJumping(bool isJumping) { m_isJumping = isJumping; }
	void setJumpSpeed(float jumpSpeed) { m_jumpSpeed = jumpSpeed; }
	void setMakeJump(bool makeJump) { m_makeJump = makeJump; }
	void setGravity(float gravity) { m_currentGravity = gravity; }
	void setLiveCount(int liveCount) { m_liveCount = liveCount; }
	void setHasTakenDamage(bool damageTaken) { m_hasTakenDamage = damageTaken; }
	void setIsDead(bool isDead) { m_isDead = isDead; }
	//void setSpeed(sf::Vector2f speed) { m_speed = speed; }
	//void setDirection(sf::Vector2f direction) { m_direction = direction; }

protected:

	
	//sf::Sprite m_sprite;
	//sf::Vector2f m_direction{ .0f, .0f };
	//sf::Vector2f m_speed{ .0f, .0f };
	bool m_makeJump{ false };
	bool m_isJumping{ false };
	float m_jumpSpeed{ 0.0f };
	float m_currentGravity{ 980.f };
	int m_liveCount{ 2 };
	bool m_hasTakenDamage{ false };
	bool m_damageAnimationStarted{ false };
	bool m_liveAmountChanged{ false };

	// Animation
	//float m_tileWidth{ .0f };
	//float m_tileHeight{ .0f };
	//float m_currentSpriteStartingX{ .0f };
	//float m_currentSpriteStartingY{ .0f };
	//float m_animationTime{ .0f };
	//float m_frameDuration{ 50.f };
	//int m_currentFrame{ 0 };
	//int m_totalFrames{ 12 };
	bool m_isDead{ false };
	bool m_deathAnimationStarted{ false };
	int m_deathAnimationTotalFrames{ 7 };
};