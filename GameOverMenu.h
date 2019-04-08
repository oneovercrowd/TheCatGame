#pragma once
#include "GameMenu.h"
class GameOverMenu :
	public GameMenu
{
protected:
	Helpers::GameOverData* go_data;
	Label *first_scores, *second_scores, *message;
public:
	GameOverMenu(Helpers::SCR_SZ screen_size);
	~GameOverMenu();
	void OnButtonDown(Button* button);
	void Resize(Helpers::SCR_SZ screen_size);
	void SetData(void *new_data);
	void OnDraw();
	void HandleEvent(SDL_Event * e);
};

