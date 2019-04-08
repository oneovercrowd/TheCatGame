#include "VideoOptionsMenu.h"



VideoOptionsMenu::VideoOptionsMenu(ScreenSettings *sets):
	GameMenu(sets->size, "video_options_menu") {

	this->sets = sets;
	// make a copy of settings
	// then if changes are accepted save it in global
	// game will save to file and restart on RS_SCREEN_OPTIONS_CHANGED event and new settings will be loaded
	// if changes are denied just set temp_sets back to sets values
	this->temp_sets = new ScreenSettings(*sets);

	Helpers::FRect title_pos;
	title_pos.x = 50;
	title_pos.y = 50;
	title_pos.w = 0;
	title_pos.h = 36;

	Helpers::FRect temp_rect;
	vector<Helpers::FRect> positions;
	// four buttons: RESOLUTION, FULLSCREEN, ACCEPT, BACK
	// first button stays 40px under title label
	temp_rect.w = 0;
	temp_rect.h = 36;
	temp_rect.x = title_pos.x;
	temp_rect.y = title_pos.y + title_pos.h + 40;
	positions.push_back(temp_rect);
	// each other button stays 20px under previous one
	temp_rect.y = positions.back().y + temp_rect.h + 20;
	positions.push_back(temp_rect);
	temp_rect.y = positions.back().y + temp_rect.h + 20;
	positions.push_back(temp_rect);
	temp_rect.y = positions.back().y + temp_rect.h + 20;
	positions.push_back(temp_rect);

	vector<string> items;
	items.push_back("RESOLUTION");
	items.push_back("FULLSCREEN");
	items.push_back("APPLY");
	items.push_back("BACK");
	SetMenu("GAME OPTIONS", title_pos, items, positions);

	items.clear();
	positions.clear();

	GLclampf color[] = { 1.0f, 1.0f, 1.0f };

	temp_rect.x = 350;
	temp_rect.y = 126;
	temp_rect.w = 0;
	temp_rect.h = 36;
	resolutions.push_back(new Label(temp_rect, "800x600", color));
	resolutions.push_back(new Label(temp_rect, "1024x768", color));
	resolutions.push_back(new Label(temp_rect, "1280x1024", color));
	resolutions.push_back(new Label(temp_rect, "1280x720", color));
	resolutions.push_back(new Label(temp_rect, "1366x768", color));
	resolutions.push_back(new Label(temp_rect, "1920x1080", color));
	
	res_index = (int)sets->size;

	temp_rect.x = 350;
	temp_rect.y = 182;
	temp_rect.w = 0;
	temp_rect.h = 36;
	fullscreens.push_back(new Label(temp_rect, "YES", color));
	fullscreens.push_back(new Label(temp_rect, "NO", color));

	if (sets->full_screen)
		flscr_index = 0;
	else
		flscr_index = 1;
}


VideoOptionsMenu::~VideoOptionsMenu() {
	// free temp settings
	delete temp_sets;
	// free Labels
	for (auto label : resolutions)
		delete label;
	for (auto label : fullscreens)
		delete label;
}

void VideoOptionsMenu::OnButtonDown(Button * button) {
	SDL_Event e;
	if (button == buttons[0]) { // case RESOLUTION
		res_index = (res_index + 1) % resolutions.size();
		temp_sets->size = (Helpers::SCR_SZ)res_index;
	}
	else if (button == buttons[1]) { // case FULLSCREEN
		temp_sets->full_screen = !temp_sets->full_screen;
		flscr_index = (flscr_index + 1) % 2;
	}
	else if (button == buttons[2]) { // case ACCEPT
		sets->full_screen = temp_sets->full_screen;
		sets->size = temp_sets->size;

		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_SCREEN_OPTIONS_CHANGED;
		SDL_PushEvent(&e);
	}
	else { // case BACK
		temp_sets->size = sets->size;
		temp_sets->full_screen = sets->full_screen;
		res_index = (int)sets->size;
		if (sets->full_screen)
			flscr_index = 0;
		else
			flscr_index = 1;
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_POP;
		SDL_PushEvent(&e);
	}
}

void VideoOptionsMenu::Resize(Helpers::SCR_SZ screen_size) {
	GameScreen::Resize(screen_size);
}

void VideoOptionsMenu::OnDraw() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	title->OnDraw();
	for (auto b : buttons) {
		b->OnDraw();
	}
	resolutions[res_index]->OnDraw();
	fullscreens[flscr_index]->OnDraw();
}

