#pragma once

#include <Gameplay/PickUp.h>

class Gem : public PickUp
{
	public:

		struct GemDescriptor : public PickUp::PickUpDescriptor
		{
			int points{ 0 };
		};

		~Gem() override = default;

		bool init(const GemDescriptor& gemDescriptor);

		int returnInfoOnPlayerCollision() override;

		int getPoints() const { return m_points; }

		void setPoints(int points) { m_points = points; }

	private:

		int m_points{ 0 };
};