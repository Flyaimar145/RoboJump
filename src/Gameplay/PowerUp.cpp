#include <Gameplay/Player.h>
#include <Gameplay/PowerUp.h>

void PowerUp::affectPlayer(Player* player)
{
	if (m_powerUpType == PowerUpType::ExtraLife)
	{
		player->setLifeCount(player->getLifeCount() + 1);
	}
	else if (m_powerUpType == PowerUpType::SpeedBoost)
	{
		player->setSpeed({ player->getSpeed().x * 2.f, player->getSpeed().y });
	}
}