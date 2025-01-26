#pragma once

#include <Gameplay/GameObject.h>
#include <Gameplay/Entity.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <External/json.hpp>
#include <Utils/Constants.h>

using json = nlohmann::json;


class Player : public Entity
{
public:

	struct PlayerDescriptor: public EntityDescriptor
	{
		float jumpSpeed{ 0.0f };
	};

	~Player() override = default;

	PlayerDescriptor load();
	bool init(const PlayerDescriptor& playerDescriptor);

	sf::FloatRect getAdjustedBounds() const { return sf::FloatRect(m_sprite.getGlobalBounds().left+6.f, m_sprite.getGlobalBounds().top+8.f, m_sprite.getGlobalBounds().width-12.f, m_sprite.getGlobalBounds().height-8.f); }
	sf::Vector2f getAdjustedPosition() const { return sf::Vector2f(m_sprite.getPosition().x + 6.f, m_sprite.getPosition().y + 8.f); }
	void setAdjustedPosition(sf::Vector2f position) { setPosition({ position.x - 6.f, position.y - 8.f }); }

	void update(float deltaMilliseconds) override;
	void render(sf::RenderWindow& window) override;

	bool getIsJumping() const { return m_isJumping; }
	float getJumpSpeed() const { return m_jumpSpeed; }
	float getGravity() const { return m_currentGravity; }

	void setIsJumping(bool isJumping) { m_isJumping = isJumping; }
	void setJumpSpeed(float jumpSpeed) { m_jumpSpeed = jumpSpeed; }
	void setGravity(float gravity) { m_currentGravity = gravity; }
	void setMakeJump(bool makeJump) { m_makeJump = makeJump; }

protected:

	bool m_makeJump{ false };
	bool m_isJumping{ false };
	float m_jumpSpeed{ 0.0f };
	float m_currentGravity{ 980.f };
};