#include <Gameplay/Player.h>
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

bool Player::init(const PlayerDescriptor& playerDescriptor)
{
	m_sprite.setTexture(*playerDescriptor.texture);
	m_sprite.setPosition(playerDescriptor.position);
	m_tileWidth = playerDescriptor.tileWidth;
	m_tileHeight = playerDescriptor.tileHeight;
	m_speed = playerDescriptor.speed;
	return true;
}

void Player::update(float deltaMilliseconds)
{
	bool isMovingLeftInput = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool isMovingRightInput = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool isJumpingInput = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	
	bool isMoving = isMovingLeftInput || isMovingRightInput;
	if (isMoving)
	{
		if (isMovingLeftInput)
		{
			m_direction.x = -1.0f;
			m_sprite.setScale(-1.f, 1.f);
			m_sprite.setOrigin(m_tileWidth, 0.f);
		}
		else if (isMovingRightInput)
		{
			m_direction.x = 1.0f;
			m_sprite.setScale(1.f, 1.f);
			m_sprite.setOrigin(0.f, 0.f);
		}
	}
	else
	{
		m_direction.x = .0f;
	}

	if (isJumpingInput && !m_isJumping)
	{
		m_isJumping = true;
		m_speed.y = -m_jumpSpeed;
	}
	//printf("Gravity: %f\n", m_currentGravity);
	// Apply gravity

	if (m_currentGravity > 0.0f)
	{
		m_speed.y += m_currentGravity * (deltaMilliseconds / 1000.f);
	}

	// Check if player is on the ground
	if (!m_isJumping && m_currentGravity == 0.0f)
	{
		m_speed.y = 0.0f;
		m_direction.y = 0.0f;
	}
	else
	{
		m_direction.y = 1.0f;
	}

	/*
	if (m_isJumping)
	{
		m_position.y += m_speed.y * (deltaMilliseconds / 1000.f);
	}
	else if (m_currentGravity > 0.0f)
	{
		m_direction.y = 1.0f;
	}
	else
	{
		m_direction.y = .0f;
	}
	m_speed.y += m_currentGravity * (deltaMilliseconds / 1000.f);
	*/
	//printf("Speed Y: %f\n", m_speed.y);

	// Update final positioN
	// IMPORTANT NOTE!! We are using delta time to change the position according to the elapsed time so, it doesn't matter how many FPS (calls to update per second) we do, 
	// the sprite changes according to the elapsed time and not to the number of calls
	m_position.x += (m_direction.x * m_speed.x * deltaMilliseconds);
	m_position.y += m_speed.y * (deltaMilliseconds / 1000.f);
	//std::cout << "X: " << m_position.x << " Y: " << m_position.y << std::endl;
	//printf("Speed X: %f, Speed Y: %f \n", m_speed.x, m_speed.y);
	//m_position.y += (m_direction.y * m_speed.y * deltaMilliseconds);

	// Update animation
	m_animationTime += deltaMilliseconds;
	if (m_speed.y != 0)
	{
		if (m_speed.y > 0.0f)
		{
			m_currentSpriteStartingX = m_tileWidth * 1.f;
			m_currentSpriteStartingY = m_tileHeight * 3.f;
		}
		else
		{
			m_currentSpriteStartingX = m_tileWidth * 0.f;
			m_currentSpriteStartingY = m_tileHeight * 3.f;
		}
	}
	else if (m_direction.x != 0.0f)
	{
		m_animationTime += deltaMilliseconds;
		if (m_animationTime >= m_frameDuration)
		{
			m_animationTime = 0.f;
			m_currentFrame = (m_currentFrame + 1) % m_totalFrames;
			m_currentSpriteStartingX = m_tileWidth * m_currentFrame;
			m_currentSpriteStartingY = m_tileHeight * 1.f;
		}
	}
	else
	{
		if (m_animationTime >= m_frameDuration)
		{
			m_animationTime = 0.f;
			m_currentFrame = (m_currentFrame + 1) % m_totalFrames;
			m_currentSpriteStartingX = m_tileWidth * m_currentFrame;
			m_currentSpriteStartingY = m_tileHeight * 0.f;
		}
	}
	m_sprite.setPosition(m_position);
}

void Player::render(sf::RenderWindow& window)
{
	// Extend this mechanism to be able to support animations
	m_sprite.setTextureRect(sf::IntRect(m_currentSpriteStartingX, m_currentSpriteStartingY, m_tileWidth, m_tileHeight));

	window.draw(m_sprite);

	const sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();
	sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	boundsRect.setOutlineColor(sf::Color::Red);
	boundsRect.setOutlineThickness(.0f);
	boundsRect.setFillColor(sf::Color::Transparent);
	window.draw(boundsRect);
}