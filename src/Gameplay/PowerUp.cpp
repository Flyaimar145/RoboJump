#include <Gameplay/PowerUp.h>

int PowerUp::returnInfoOnPlayerCollision()
{
	if (m_powerUpType == PowerUpType::ExtraLife)
	{
		return 0;
	}
	else if (m_powerUpType == PowerUpType::SpeedBoost)
	{
		return 1;
	}
	else {
		return -1;
	}
}