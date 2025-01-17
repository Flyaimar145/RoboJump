#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Player : public GameObject
{
public:

	struct PlayerDescriptor
	{
		sf::Vector2f position;
		sf::Texture* texture{ nullptr };
		float tileWidth{ .0f };
		float tileHeight{ .0f };
		sf::Vector2f speed{ .0f, .0f };
	};

	~Player() override = default;

	bool init(const PlayerDescriptor& playerDescriptor);

	sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

	void update(float deltaMilliseconds) override;
	void render(sf::RenderWindow& window) override;

	sf::Vector2f getSpeed() const { return m_speed; }
	void setGravity(float gravity) { m_currentGravity = gravity; }
	void setIsJumping(bool isJumping) { m_isJumping = isJumping; }
	void setSpeed(sf::Vector2f speed) { m_speed = speed; }

protected:

	
	sf::Sprite m_sprite;
	sf::Vector2f m_direction{ .0f, .0f };
	sf::Vector2f m_speed{ .0f, .0f };
	bool m_isJumping{ false };
	float m_jumpSpeed{ 300.0f };
	float m_currentGravity{ 980.f };

	// Animation
	float m_tileWidth{ .0f };
	float m_tileHeight{ .0f };
	float m_currentSpriteStartingX{ .0f };
	float m_currentSpriteStartingY{ .0f };
	float m_animationTime{ .0f };
	float m_frameDuration{ 50.f };
	int m_currentFrame{ 0 };
	int m_totalFrames{ 12 };
};