#include "MainMenu.h"



MainMenu::MainMenu(Helpers::SCR_SZ screen_size):
	GameMenu(screen_size, "main_menu") {

	Helpers::FRect title_pos;
	title_pos.x = 50.f;
	title_pos.y = 50.f;
	title_pos.w = 0;
	title_pos.h = 36.f;

	Helpers::FRect temp_rect;
	vector<Helpers::FRect> positions;
	// four buttons: NEW GAME, LOAD, OPTIONS, QUIT
	// first button stays 40px under title label
	temp_rect.w = 0;
	temp_rect.h = 36.f;
	temp_rect.x = title_pos.x;
	temp_rect.y = title_pos.y + title_pos.h + 40.f;
	positions.push_back(temp_rect);
	for (int i = 0; i < 3; i++) {
		// each other button stays 20px under previous one
		temp_rect.y = positions.back().y + temp_rect.h + 20.f;
		positions.push_back(temp_rect);
	}

	vector<string> items;
	items.push_back("NEW GAME");
	items.push_back("LOAD GAME");
	items.push_back("OPTIONS");
	items.push_back("QUIT");
	SetMenu("CAT GAME", title_pos, items, positions);

	items.clear();
	positions.clear();
}

void MainMenu::HandleEvent(SDL_Event * e) {
	SDL_Event ev;
	if (e->type == SDL_USEREVENT) {
		if (e->user.code == Helpers::BUTTON_DOWN) {
			OnButtonDown((Button*)(e->user.data1));
		}
	}
	else if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_ESCAPE) {
			/* IN MAIN MENU PUSH QUIT DIALOG */
			ev.type = Helpers::RUN_STACK_EVENT;
			ev.user.code = Helpers::RS_PUSH;
			const char* temp = "quit_dialog";
			ev.user.data1 = (void*)temp;
			SDL_PushEvent(&ev);
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN)
		if (e->button.button == SDL_BUTTON_LEFT)
			for (auto b : buttons)
				b->OnClick();
}

MainMenu::~MainMenu() {
}

void MainMenu::OnButtonDown(Button * button) {
	SDL_Event e;
	if (button == buttons[0]) { // case NEW GAME
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_PUSH;
		const char* temp = "cat_game";
		e.user.data1 = (void*)temp;
		SDL_PushEvent(&e);
	}
	else if (button == buttons[1]) { // case LOAD GAME
		printf("LOAD GAME\n");
	}
	else if (button == buttons[2]) { // case OPTIONS
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_PUSH;
		const char* temp = "options_menu";
		e.user.data1 = (void*)temp;
		SDL_PushEvent(&e);
	}
	else { // case QUIT
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_PUSH;
		const char* temp = "quit_dialog";
		e.user.data1 = (void*)temp;
		SDL_PushEvent(&e);
	}
}

void MainMenu::Resize(Helpers::SCR_SZ screen_size) {
	GameScreen::Resize(screen_size);
}
