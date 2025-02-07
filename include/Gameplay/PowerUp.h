#pragma once

#include <Gameplay/PickUp.h>

class PowerUp : public PickUp
{
	public:

		enum class PowerUpType
		{
			ExtraLife,
			SpeedBoost,
		};

		~PowerUp() override = default;

		int returnInfoOnPlayerCollision() override;

		PowerUpType getPowerUpType() const { return m_powerUpType; }

		void setPowerUpType(PowerUpType powerUpType) { m_powerUpType = powerUpType; }

	private:

		PowerUpType m_powerUpType;
};