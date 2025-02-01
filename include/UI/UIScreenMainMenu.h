#pragma once

#include <UI/UIScreen.h>

class UIScreenMainMenu : public UIScreen
{
	public:

		~UIScreenMainMenu() = default;

		void update(float milliseconds) override;
};