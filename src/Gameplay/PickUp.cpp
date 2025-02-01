#include <Gameplay/PickUp.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool PickUp::init(const PickUpDescriptor& pickUpDescriptor)
{
	m_sprite.setTexture(*pickUpDescriptor.texture);
	m_sprite.setPosition(pickUpDescriptor.position);
	this->setPosition(pickUpDescriptor.position);
	m_tileWidth = pickUpDescriptor.tileWidth;
	m_tileHeight = pickUpDescriptor.tileHeight;
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tileWidth, m_tileHeight));

	m_frameDuration = pickUpDescriptor.frameDuration;
	m_totalFrames = pickUpDescriptor.totalFrames;

	return true;
}

void PickUp::update(float deltaMilliseconds)
{
	m_animationTime += deltaMilliseconds;
	if (m_animationTime >= m_frameDuration)
	{
		this->updateAnimation(m_totalFrames, 0.f);
	}
}

void PickUp::render(sf::RenderWindow& window)
{
	m_sprite.setTextureRect(sf::IntRect(m_currentSpriteStartingX, m_currentSpriteStartingY, m_tileWidth, m_tileHeight));
	window.draw(m_sprite);

	// Uncomment to draw the bounds of the sprite
	/*const sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();
	sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	boundsRect.setOutlineColor(sf::Color::Blue);
	boundsRect.setOutlineThickness(.5f);
	boundsRect.setFillColor(sf::Color::Transparent);
	window.draw(boundsRect);*/
}

void PickUp::updateAnimation(int totalAnimationFrames, float spriteSheetRow)
{
	m_animationTime = 0.f;
	m_currentFrame = (m_currentFrame + 1) % totalAnimationFrames;
	m_currentSpriteStartingX = m_tileWidth * m_currentFrame;
	m_currentSpriteStartingY = m_tileHeight * spriteSheetRow;
}