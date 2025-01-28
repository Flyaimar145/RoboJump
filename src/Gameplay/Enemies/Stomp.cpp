#include <Gameplay/Enemies/Stomp.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool Stomp::init(const EnemyDescriptor& enemyDescriptor)
{
	m_enemyType = Enemy::EnemyType::Stomp;
	m_frameDuration = 100.f;
	m_initialPositionY = enemyDescriptor.position.y;
	m_originalSpeedX = enemyDescriptor.speed.x;
	m_detectionZone = sf::FloatRect(enemyDescriptor.position.x + enemyDescriptor.tileWidth/2.f - 2.f, enemyDescriptor.position.y + enemyDescriptor.tileHeight*2 -2.f , 2.f, 2.f);
	return Enemy::init(enemyDescriptor);
}

void Stomp::onPlayerCollision()
{
	m_hasToAttack = true;
}

void Stomp::update(float deltaMilliseconds)
{
	if (m_direction.x > 0)
	{
		m_sprite.setScale(1.f, 1.f);
		m_sprite.setOrigin(0.f, 0.f);
	}
	else
	{
		m_sprite.setScale(-1.f, 1.f);
		m_sprite.setOrigin(m_tileWidth, 0.f);
	}

	m_position.x += (m_direction.x * m_speed.x * deltaMilliseconds);
	m_position.y += m_speed.y * (deltaMilliseconds / 1000.f);
	
	//Move the m_detectionZone rectangle with the Stomp
	m_detectionZone.left = m_position.x + m_tileWidth / 2.f - 2.f;
	m_detectionZone.top = m_position.y + m_tileHeight * 2 - 2.f;

	m_animationTime += deltaMilliseconds;

	if (m_hasToAttack)
	{
		
		if (!m_attackAnimationStarted)
		{
			m_speed.x = 0.f;
			m_currentFrame = 0;
			m_attackAnimationStarted = true;
		}
		if (m_position.y < m_initialPositionY + m_heightToFall)
		{
			m_speed.y = m_fallingSpeed;
		}
		else
		{
			m_speed.y = 0.f;
		}
		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 2.f);
			if (m_currentFrame == m_totalFrames - 1)
			{
				m_hasToRise = true;
				m_hasToAttack = false;
				m_attackAnimationStarted = false;
			}
		}
	}
	else
	{
		if (m_hasToRise)
		{
			if (m_position.y > m_initialPositionY)
			{
				m_speed.y = -m_fallingSpeed;
			}
			else
			{
				m_speed.x = m_originalSpeedX;
				m_speed.y = 0.f;
				m_hasToRise = false;
			}
		}
		else if (m_direction.x != 0.0f)
		{
			if (m_animationTime >= m_frameDuration)
			{
				updateAnimation(m_totalFrames, 1.f);
			}
		}
		else
		{
			if (m_animationTime >= m_frameDuration)
			{
				updateAnimation(m_totalFrames, 0.f);
			}
		}
	}

	Enemy::update(deltaMilliseconds);
}

void Stomp::render(sf::RenderWindow& window)
{
	Enemy::render(window);
	sf::RectangleShape detectionZoneRect(sf::Vector2f(m_detectionZone.width, m_detectionZone.height));
	detectionZoneRect.setPosition(m_detectionZone.left, m_detectionZone.top);
	detectionZoneRect.setOutlineColor(sf::Color::Green);
	detectionZoneRect.setOutlineThickness(.5f);
	detectionZoneRect.setFillColor(sf::Color::Transparent);
	window.draw(detectionZoneRect);

}