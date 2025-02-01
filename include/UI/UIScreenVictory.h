#pragma once

#include <UI/UIScreen.h>

class UIScreenVictory : public UIScreen
{
	public:

		~UIScreenVictory() = default;

		void update(float milliseconds) override;
};