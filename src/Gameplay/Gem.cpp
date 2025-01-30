#include <Gameplay/Gem.h>
#include <Gameplay/Player.h>

bool Gem::init(const GemDescriptor& gemDescriptor)
{
	m_points = gemDescriptor.points;
	PickUp::init(gemDescriptor);
	return true;
}

int Gem::returnInfoOnPlayerCollision()
{
	return m_points;
}
