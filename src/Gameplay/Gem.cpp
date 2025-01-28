#include <Gameplay/Gem.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool Gem::init(const GemDescriptor& gemDescriptor)
{
	m_sprite.setTexture(*gemDescriptor.texture);
	m_sprite.setPosition(gemDescriptor.position);
	this->setPosition(gemDescriptor.position);
	m_tileWidth = gemDescriptor.tileWidth;
	m_tileHeight = gemDescriptor.tileHeight;
	m_points = gemDescriptor.points;
	return true;
}

void Gem::update(float deltaMilliseconds)
{
	m_animationTime += deltaMilliseconds;
	if (m_animationTime >= m_frameDuration)
	{
		this->updateAnimation(totalFrames, 0.f);
	}
}

void Gem::render(sf::RenderWindow& window)
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

void Gem::updateAnimation(int totalAnimationFrames, float spriteSheetRow)
{
	m_animationTime = 0.f;
	m_currentFrame = (m_currentFrame + 1) % totalAnimationFrames;
	m_currentSpriteStartingX = m_tileWidth * m_currentFrame;
	m_currentSpriteStartingY = m_tileHeight * spriteSheetRow;
}