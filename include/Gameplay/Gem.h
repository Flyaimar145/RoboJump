#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Gem : public GameObject
{
	public:

		struct GemDescriptor
		{
			sf::Vector2f position{ .0f, .0f };
			sf::Texture* texture{ nullptr };
			float tileWidth{ .0f };
			float tileHeight{ .0f };
			int points{ 0 };
		};
		~Gem() override = default;

		bool init(const GemDescriptor& gemDescriptor);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;
		void updateAnimation(int totalAnimationFrames, float spriteSheetRow);

		int getPoints() const { return m_points; }

		void setPoints(int points) { m_points = points; }

	private:
		sf::Sprite m_sprite;
		float m_tileWidth{ .0f };
		float m_tileHeight{ .0f };
		float m_currentSpriteStartingX{ .0f };
		float m_currentSpriteStartingY{ .0f };
		int m_currentFrame{ 0 };
		float m_animationTime{ .0f };
		float m_frameDuration{ 50.f };
		int totalFrames{ 7 };
		float tileWidth{ .0f };
		float tileHeight{ .0f };

		int m_points{ 1 };


};