#include <Gameplay/Entity.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


bool Entity::init(const EntityDescriptor& entityDescriptor)
{
	m_sprite.setTexture(*entityDescriptor.firstTexture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tileWidth, m_tileHeight));
	this->setPosition(entityDescriptor.position);
	m_tileWidth = entityDescriptor.tileWidth;
	m_tileHeight = entityDescriptor.tileHeight;
	m_speed = entityDescriptor.speed;
	m_totalFrames = entityDescriptor.totalFrames;
	m_deathAnimationTotalFrames = entityDescriptor.deathAnimationTotalFrames;
	m_lifeCount = entityDescriptor.lifeCount;
	return true;
}

void Entity::update(float  deltaMilliseconds)
{
	m_sprite.setPosition(m_position);
}

void Entity::render(sf::RenderWindow& window)
{
	// Extend this mechanism to be able to support animations
	m_sprite.setTextureRect(sf::IntRect(m_currentSpriteStartingX, m_currentSpriteStartingY, m_tileWidth, m_tileHeight));

	window.draw(m_sprite);

	const sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();
	sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	boundsRect.setOutlineColor(sf::Color::Blue);
	boundsRect.setOutlineThickness(.5f);
	boundsRect.setFillColor(sf::Color::Transparent);
	window.draw(boundsRect);
}

void Entity::updateAnimation(int totalAnimationFrames, float spriteSheetRow)
{
	m_animationTime = 0.f;
	m_currentFrame = (m_currentFrame + 1) % totalAnimationFrames;
	m_currentSpriteStartingX = m_tileWidth * m_currentFrame;
	m_currentSpriteStartingY = m_tileHeight * spriteSheetRow;
}