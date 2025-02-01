// To-Do. Ideally a UIScreen transition should be managed through a UIManager class
#pragma once

#include <UI/UIScreen.h>

class UIScreenVictory : public UIScreen
{
public:
	~UIScreenVictory() = default;

	bool init(sf::Texture* image) override;

	void update(float milliseconds) override;
};