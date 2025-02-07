#include <Gameplay/Enemies/Enemy.h>

bool Enemy::init(const EnemyDescriptor& enemyDescriptor)
{
	m_direction = enemyDescriptor.initialDirection;
	
	return Entity::init(enemyDescriptor);
}

void Enemy::update(float deltaMilliseconds)
{	
	Entity::update(deltaMilliseconds);
}

void Enemy::render(sf::RenderWindow& window)
{
	Entity::render(window);
}
