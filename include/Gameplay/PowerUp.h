#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class PowerUp : public GameObject
{
	public:

		struct PowerUpDescriptor
		{
			sf::Vector2f position{ .0f, .0f };
			sf::Texture* texture{ nullptr };
			float tileWidth{ .0f };
			float tileHeight{ .0f };
		};

		enum class PowerUpType
		{
			ExtraLife,
			SpeedBoost,
		};

		~PowerUp() override = default;

		bool init(const PowerUpDescriptor& powerUpDescriptor);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;
		void updateAnimation(int totalAnimationFrames, float spriteSheetRow);

		PowerUpType getPowerUpType() const { return m_powerUpType; }

		void setPowerUpType(PowerUpType powerUpType) { m_powerUpType = powerUpType; }

	private:
		PowerUpType m_powerUpType{ };
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