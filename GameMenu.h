#pragma once
#include "GameScreen.h"
#include "Button.h"
#include <vector>
#include <string>

using namespace std;

constexpr auto MENU_FRAME_TIME = 20;

// button color is 255,255,255,255
// button alt color is 255,100,100,255

class GameMenu :
	public GameScreen
{
protected:
	Label *title;
	vector<Button*> buttons;
	void SetMenu(const string title, Helpers::FRect &title_pos, const vector<string> &items, const vector<Helpers::FRect> &positions);
public:
	GameMenu(Helpers::SCR_SZ screen_size, string screen_name);
	~GameMenu();
	virtual void OnButtonDown(Button *button) = 0;
	void HandleEvent(SDL_Event *e);
	void OnDraw();
	void Update();
	void Reset();
};

