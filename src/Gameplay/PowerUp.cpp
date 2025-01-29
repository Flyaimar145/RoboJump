#include <Gameplay/PowerUp.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool PowerUp::init(const PowerUpDescriptor& powerUpDescriptor)
{
	m_sprite.setTexture(*powerUpDescriptor.texture);
	m_sprite.setPosition(powerUpDescriptor.position);
	this->setPosition(powerUpDescriptor.position);
	m_tileWidth = powerUpDescriptor.tileWidth;
	m_tileHeight = powerUpDescriptor.tileHeight;
	return true;
}

void PowerUp::update(float deltaMilliseconds)
{
	m_animationTime += deltaMilliseconds;
	if (m_animationTime >= m_frameDuration)
	{
		this->updateAnimation(totalFrames, 0.f);
	}
}

void PowerUp::render(sf::RenderWindow& window)
{
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

void PowerUp::updateAnimation(int totalAnimationFrames, float spriteSheetRow)
{
	m_animationTime = 0.f;
	m_currentFrame = (m_currentFrame + 1) % totalAnimationFrames;
	m_currentSpriteStartingX = m_tileWidth * m_currentFrame;
	m_currentSpriteStartingY = m_tileHeight * spriteSheetRow;
}