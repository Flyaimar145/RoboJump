#pragma once

#include <UI/UIScreen.h>

class UIScreenMainMenu : public UIScreen
{
	public:

		~UIScreenMainMenu() override = default;

		void update(float milliseconds) override;

		bool getExitGame() const { return m_exitGame; }

	private:

		bool m_exitGame{ false };
};