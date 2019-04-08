#pragma once
#include "GameMenu.h"
#include "ScreenSettings.h"

class OptionsMenu :
	public GameMenu
{
public:
	OptionsMenu(Helpers::SCR_SZ screen_size);
	~OptionsMenu();
	void OnButtonDown(Button* button);
	void Resize(Helpers::SCR_SZ screen_size);
};

