#pragma once

#include <Gameplay/Entity.h>

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

		void update(float deltaMilliseconds) override;

		void addScore(int points) { m_score += points; }
		void addLife();
		void applySpeedBoost();

		bool getIsJumping() const { return m_isJumping; }
		float getJumpSpeed() const { return m_jumpSpeed; }
		float getGravity() const { return m_currentGravity; }
		int getScore() const { return m_score; }

		void setIsJumping(bool isJumping) { m_isJumping = isJumping; }
		void setJumpSpeed(float jumpSpeed) { m_jumpSpeed = jumpSpeed; }
		void setGravity(float gravity) { m_currentGravity = gravity; }
		void setMakeJump(bool makeJump) { m_makeJump = makeJump; }
	
	protected:

		bool m_isJumping{ false };
		bool m_makeJump{ false };
		float m_jumpSpeed{ 0.0f };
		float m_currentGravity{ 980.f };

		int m_score{ 0 };

		bool m_isSpeedBoostActive{ false };
		float m_speedBoostDuration{ 0.0f };
		float m_speedBoostTimer{ 0.0f };
};
