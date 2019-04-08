#include "OptionsMenu.h"



OptionsMenu::OptionsMenu(Helpers::SCR_SZ screen_size):
	GameMenu(screen_size, "options_menu") {

	Helpers::FRect title_pos;
	title_pos.x = 50;
	title_pos.y = 50;
	title_pos.w = 250;
	title_pos.h = 36;

	Helpers::FRect temp_rect;
	vector<Helpers::FRect> positions;
	// four buttons: VIDEO OPTIONS, GAME OPTIONS, KEYMAP, BACK
	// first button stays 40px under title label
	temp_rect.w = 250;
	temp_rect.h = 36;
	temp_rect.x = title_pos.x;
	temp_rect.y = title_pos.y + title_pos.h + 40;
	positions.push_back(temp_rect);

	temp_rect.y = positions.back().y + temp_rect.h + 20;
	positions.push_back(temp_rect);

	temp_rect.y = positions.back().y + temp_rect.h + 40;
	positions.push_back(temp_rect);

	temp_rect.y = positions.back().y + temp_rect.h + 40;
	positions.push_back(temp_rect);

	vector<string> items;
	items.push_back("VIDEO OPTIONS");
	items.push_back("GAME OPTIONS");
	items.push_back("KEYMAP");
	items.push_back("BACK");
	SetMenu("OPTIONS", title_pos, items, positions);

	items.clear();
	positions.clear();

}


OptionsMenu::~OptionsMenu() {
}

void OptionsMenu::OnButtonDown(Button * button) {
	SDL_Event e;
	if (button == buttons[0]) { // case VIDEO
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_PUSH;
		const char* temp = "video_options_menu";
		e.user.data1 = (void*)temp;
		SDL_PushEvent(&e);
	}
	else if (button == buttons[1]) { // case GAME
		printf("GAME\n");
	}
	else if (button == buttons[2]) { // case KEYMAP
		printf("KEYMAP\n");
	}
	else { // case BACK
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_POP;
		SDL_PushEvent(&e);
	}
}

void OptionsMenu::Resize(Helpers::SCR_SZ screen_size) {
	GameScreen::Resize(screen_size);
}
