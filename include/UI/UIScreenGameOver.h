#pragma once

#include <UI/UIScreen.h>

class UIScreenGameOver : public UIScreen
{
	public:

		~UIScreenGameOver() = default;

		void update(float milliseconds) override;
};