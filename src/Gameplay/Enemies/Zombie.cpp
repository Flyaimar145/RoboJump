#include <Gameplay/Enemies/Zombie.h>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>



bool Zombie::init(const ZombieDescriptor& zombieDescriptor)
{
	m_speed = zombieDescriptor.speed;

	return Enemy::init(zombieDescriptor);
}

void Zombie::update(float deltaMilliseconds)
{
	/*bool isMovingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool isMovingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	bool isMovingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	bool isMovingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	bool isMoving = isMovingLeft || isMovingRight || isMovingUp || isMovingDown;
	if (isMoving)
	{
		if (isMovingLeft)
		{
			m_direction.x = -1.0f;
			m_direction.y = 0.0f;
			//m_currentSpriteStartingX = m_tileWidth * 0.f;
			//m_currentSpriteStartingY = m_tileHeight * 4.f;
			m_sprite.setScale(-1.f, 1.f);
			m_sprite.setOrigin(m_tileWidth, 0.f);
		}
		else if (isMovingRight)
		{
			m_direction.x = 1.0f;
			m_direction.y = 0.0f;
			//m_currentSpriteStartingX = m_tileWidth *0.f;
			//m_currentSpriteStartingY = m_tileHeight * 4.f;
			m_sprite.setScale(1.f, 1.f);
			m_sprite.setOrigin(0.f, 0.f);
		}
		if (isMovingUp)
		{
			m_direction.y = -1.0f;
			m_direction.x = 0.0f;
			//m_currentSpriteStartingX = m_tileWidth * 0.f;
			//m_currentSpriteStartingY = m_tileHeight * 4.f;
		}
		else if (isMovingDown)
		{
			m_direction.y = 1.0f;
			m_direction.x = 0.0f;
			//m_currentSpriteStartingX = m_tileWidth * 0.f;
			//m_currentSpriteStartingY = m_tileHeight * 4.f;
		}
	}
	else
	{
		m_direction.x = .0f;
		m_direction.y = .0f;
		//m_currentSpriteStartingX = m_tileWidth * 0.f;
		//m_currentSpriteStartingY = m_tileHeight * 0.f;
	}
	// Update final position
	// IMPORTANT NOTE!! We are using delta time to change the position according to the elapsed time so, it doesn't matter how many FPS (calls to update per second) we do, 
	// the sprite changes according to the elapsed time and not to the number of calls
	m_position.x += (m_direction.x * m_speed.x * deltaMilliseconds);
	m_position.y += (m_direction.y * m_speed.y * deltaMilliseconds);

	// Update animation
	m_animationTime += deltaMilliseconds;
	if (m_direction.x != 0.0f)
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
		//m_currentFrame = 0;
		//m_currentSpriteStartingX = m_tileWidth * 0.f;
		//m_currentSpriteStartingY = m_tileHeight * 0.f;
	}
	//std::cout << "X: " << m_position.x << " Y: " << m_position.y << std::endl;*/
	Enemy::update(deltaMilliseconds);
}

