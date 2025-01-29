#include <Gameplay/Gem.h>
#include <Gameplay/Player.h>

bool Gem::init(const GemDescriptor& gemDescriptor)
{
	m_points = gemDescriptor.points;
	PickUp::init(gemDescriptor);
	return true;
}

void Gem::affectPlayer(Player* player)
{
	player->addPoints(m_points);
}
