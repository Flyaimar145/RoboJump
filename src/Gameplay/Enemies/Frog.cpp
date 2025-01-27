#include <Gameplay/Enemies/Frog.h>


bool Frog::init(const EnemyDescriptor& enemyDescriptor)
{
	m_enemyType = Enemy::EnemyType::Frog;
	m_hasToAttack = false;
	m_canMakeDamage = false;
	return Enemy::init(enemyDescriptor);
}

void Frog::onPlayerCollision()
{
	m_hasToAttack = true;
}

void Frog::update(float deltaMilliseconds)
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

	m_animationTime += deltaMilliseconds;
	if (m_hasToAttack)
	{
		if (!m_attackAnimationStarted)
		{
			m_currentFrame = 0;
			m_attackAnimationStarted = true;
		}
		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 1.f);
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
		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 0.f);
		}
	}
	
	Enemy::update(deltaMilliseconds);
}