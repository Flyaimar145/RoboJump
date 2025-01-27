#include <Gameplay/Enemies/Stomp.h>
#include <SFML/Window/Keyboard.hpp>

bool Stomp::init(const EnemyDescriptor& enemyDescriptor)
{
	m_enemyType = Enemy::EnemyType::Stomp;
	m_hasToAttack = false;
	m_canMakeDamage = false;
	m_frameDuration = 100.f;
	return Enemy::init(enemyDescriptor);
}

void Stomp::onPlayerCollision()
{
	m_hasToAttack = true;
}

void Stomp::update(float deltaMilliseconds)
{

	bool hasToAttackInput = sf::Keyboard::isKeyPressed(sf::Keyboard::K);


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


	m_animationTime += deltaMilliseconds;
	if (hasToAttackInput)
	{
		if (!m_attackAnimationStarted)
		{
			m_currentFrame = 0;
			m_attackAnimationStarted = true;
		}
		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 2.f);
			if (m_currentFrame == 6)
			{
				m_canMakeDamage = true;
			}
			if (m_currentFrame == m_totalFrames - 1)
			{
				m_canMakeDamage = false;
				m_hasToAttack = false;
				m_attackAnimationStarted = false;
			}
		}
	}
	else
	{
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