#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class PickUp : public GameObject
{
	public:

		struct PickUpDescriptor
		{
			sf::Vector2f position{ .0f, .0f };
			sf::Texture* texture{ nullptr };
			float tileWidth{ .0f };
			float tileHeight{ .0f };
		};

		enum class PickUpType
		{
			Gem,
			PowerUp,
		};

		~PickUp() override = default;

		bool init(const PickUpDescriptor& pickUpDescriptor);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;
		void updateAnimation(int totalAnimationFrames, float spriteSheetRow);

		PickUpType getPickUpType() const { return m_pickUpType; }

		void setPickUpType(PickUpType pickUpType) { m_pickUpType = pickUpType; }

	private:
		PickUpType m_pickUpType{ };
		sf::Sprite m_sprite;
		float m_tileWidth{ .0f };
		float m_tileHeight{ .0f };
		float m_currentSpriteStartingX{ .0f };
		float m_currentSpriteStartingY{ .0f };
		int m_currentFrame{ 0 };
		float m_animationTime{ .0f };
		float m_frameDuration{ 100.f };
		int totalFrames{ 7 };
		float tileWidth{ .0f };
		float tileHeight{ .0f };

};