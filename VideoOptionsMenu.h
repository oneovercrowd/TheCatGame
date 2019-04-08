#pragma once
#include "GameMenu.h"
#include "ScreenSettings.h"

class VideoOptionsMenu :
	public GameMenu
{
protected:
	ScreenSettings *sets, // pointer to global settings
		*temp_sets; // pointer to temp settings
	vector<Label*> resolutions;
	int res_index;
	vector<Label*> fullscreens;
	int flscr_index;
public:
	VideoOptionsMenu(ScreenSettings *sets);
	~VideoOptionsMenu();
	void OnButtonDown(Button* button);
	void Resize(Helpers::SCR_SZ screen_size);
	void OnDraw();
};

