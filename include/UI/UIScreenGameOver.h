#pragma once

#include <UI/UIScreen.h>

class UIScreenGameOver : public UIScreen
{
	public:

		~UIScreenGameOver() override = default;

		void update(float milliseconds) override;
};