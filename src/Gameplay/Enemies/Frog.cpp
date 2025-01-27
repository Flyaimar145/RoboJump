#include <Gameplay/Enemies/Frog.h>

void Frog::update(float deltaMilliseconds)
{
	m_animationTime += deltaMilliseconds;
	if (m_animationTime >= m_frameDuration)
	{
		updateAnimation(m_totalFrames, 0.f);
	}

	Enemy::update(deltaMilliseconds);
}