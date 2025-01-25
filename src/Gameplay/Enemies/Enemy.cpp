#include <Gameplay/Enemies/Enemy.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool Enemy::init(const EnemyDescriptor& enemyDescriptor)
{
	//m_sprite.setTexture(*enemyDescriptor.texture);
	//m_sprite.setPosition(enemyDescriptor.position);
	//m_tileWidth = enemyDescriptor.tileWidth;
	//m_tileHeight = enemyDescriptor.tileHeight;
	m_direction = enemyDescriptor.direction;
	
	return Entity::init(enemyDescriptor);
}

void Enemy::update(float deltaMilliseconds)
{
	//m_sprite.setPosition(m_position);
	
	if(m_direction.x > 0)
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
		// First moment of taking damage: Reset the current frame to 0 to start the animation from the beginning, and reduce the live count
		if (!m_damageAnimationStarted)
		{
			m_currentFrame = 0;
			m_damageAnimationStarted = true;
			printf("Damage taken\n");
			m_liveCount--;
			//m_liveAmountChanged = true;
		}

		if (m_animationTime >= m_frameDuration)
		{
			updateAnimation(m_totalFrames, 2.f);
			if (m_currentFrame == m_totalFrames - 1)
			{
				m_hasTakenDamage = false;
				m_damageAnimationStarted = false;
				if (m_liveCount <= 0)
				{
					m_isDead = true;
				}
			}
		}
	}
	else if (m_isDead)
	{
		// Reset the current frame to 0 when damage is taken
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
	Entity::update(deltaMilliseconds);
}

void Enemy::render(sf::RenderWindow& window)
{
	// Extend this mechanism to be able to support animations
	//m_sprite.setTextureRect(sf::IntRect(m_currentSpriteStartingX, m_currentSpriteStartingY, m_tileWidth, m_tileHeight));

	//window.draw(m_sprite);

	//const sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();
	//sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	//boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	//boundsRect.setOutlineColor(sf::Color::Red);
	//boundsRect.setOutlineThickness(2.f);
	//boundsRect.setFillColor(sf::Color::Transparent);
	//window.draw(boundsRect);
	Entity::render(window);
}

void Enemy::updateAnimation(int totalAnimationFrames, float spriteSheetRow)
{
	m_animationTime = 0.f;
	m_currentFrame = (m_currentFrame + 1) % totalAnimationFrames;
	m_currentSpriteStartingX = m_tileWidth * m_currentFrame;
	m_currentSpriteStartingY = m_tileHeight * spriteSheetRow;
}