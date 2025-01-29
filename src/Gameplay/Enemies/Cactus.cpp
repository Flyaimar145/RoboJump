#include <Gameplay/Enemies/Cactus.h>
#include <SFML/Graphics/RenderWindow.hpp>

bool Cactus::init(const EnemyDescriptor& enemyDescriptor)
{
	m_enemyType = Enemy::EnemyType::Cactus;
	return Enemy::init(enemyDescriptor);
}

void Cactus::update(float deltaMilliseconds)
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

	m_animationTime += deltaMilliseconds;
	if (m_hasTakenDamage)
	{
		if (!m_damageAnimationStarted)
		{
			m_currentFrame = 0;
			m_damageAnimationStarted = true;
			m_lifeCount--;
		}

		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 2.f);
			if (m_currentFrame == m_totalFrames - 1)
			{
				m_hasTakenDamage = false;
				m_damageAnimationStarted = false;
				if (m_lifeCount <= 0)
				{
					m_isDead = true;
				}
			}
		}
	}
	else if (m_isDead)
	{
		if (!m_deathAnimationStarted)
		{
			m_currentFrame = 0;
			m_deathAnimationStarted = true;
		}

		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_deathAnimationTotalFrames, 4.f);
			if (m_currentFrame == m_deathAnimationTotalFrames - 1)
			{
				m_position = { 1000.f, 1000.f };
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
		else if (m_speed.x != 0.0f)
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