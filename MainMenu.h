#pragma once
#include "GameMenu.h"
class MainMenu :
	public GameMenu
{
public:
	MainMenu(Helpers::SCR_SZ screen_size);
	~MainMenu();
	void OnButtonDown(Button* button);
	void Resize(Helpers::SCR_SZ screen_size);
	void HandleEvent(SDL_Event * e);
};

