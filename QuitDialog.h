#pragma once
#include "GameMenu.h"
class QuitDialog :
	public GameMenu
{
public:
	QuitDialog(Helpers::SCR_SZ screen_size);
	~QuitDialog();
	void OnButtonDown(Button* button);
	void Resize(Helpers::SCR_SZ screen_size);
	void HandleEvent(SDL_Event * e);
};

