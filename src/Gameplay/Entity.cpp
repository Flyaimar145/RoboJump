#include <Gameplay/Entity.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


bool Entity::init(const EntityDescriptor& entityDescriptor)
{
	m_speed = entityDescriptor.speed;

	m_sprite.setTexture(*entityDescriptor.firstTexture);
	m_sprite.setPosition(entityDescriptor.position);
	this->setPosition(entityDescriptor.position);

	m_tileWidth = entityDescriptor.tileWidth;
	m_tileHeight = entityDescriptor.tileHeight;
	m_offsetForAdjustedBoundsLeft = entityDescriptor.offsetForAdjustedBoundsLeft;
	m_offsetForAdjustedBoundsTop = entityDescriptor.offsetForAdjustedBoundsTop;
	m_offsetForAdjustedBoundsWidth = entityDescriptor.offsetForAdjustedBoundsWidth;
	m_offsetForAdjustedBoundsHeight = entityDescriptor.offsetForAdjustedBoundsHeight;
	
	m_lifeCount = entityDescriptor.lifeCount;

	m_totalFrames = entityDescriptor.totalFrames;
	m_deathAnimationTotalFrames = entityDescriptor.deathAnimationTotalFrames;
	m_frameDuration = entityDescriptor.frameDuration;
	
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tileWidth, m_tileHeight));

	return true;
}

void Entity::update(float  deltaMilliseconds)
{
	m_sprite.setPosition(m_position);
}

void Entity::render(sf::RenderWindow& window)
{
	m_sprite.setTextureRect(sf::IntRect(m_currentSpriteStartingX, m_currentSpriteStartingY, m_tileWidth, m_tileHeight));
	window.draw(m_sprite);
}

void Entity::updateAnimation(int totalAnimationFrames, float spriteSheetRow)
{
	m_animationTime = 0.f;
	m_currentFrame = (m_currentFrame + 1) % totalAnimationFrames;
	m_currentSpriteStartingX = m_tileWidth * m_currentFrame;
	m_currentSpriteStartingY = m_tileHeight * spriteSheetRow;
}