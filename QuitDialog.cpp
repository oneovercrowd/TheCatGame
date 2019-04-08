#include "QuitDialog.h"



QuitDialog::QuitDialog(Helpers::SCR_SZ screen_size):
	GameMenu(screen_size, "quit_dialog") {
	// quit dialog is 200x150 window in the center of display

	Helpers::FRect title_pos;
	// label stays 15px below top line
	// centered by X coord
	title_pos.x = w / 2.f - 45.f;
	title_pos.y = h / 2.f - 65.f;
	title_pos.w = 0;
	title_pos.h = 36.f;

	Helpers::FRect temp_rect;
	vector<Helpers::FRect> positions;
	// two buttons: YES, NO
	// buttons stay 15px above bottom line
	temp_rect.w = 0;
	temp_rect.h = 36.f;
	temp_rect.x = w / 2.f - 75.f;
	temp_rect.y = h / 2.f + 30.f;
	positions.push_back(temp_rect);

	temp_rect.w = 0;
	temp_rect.x = w / 2.f + 30.f;
	positions.push_back(temp_rect);

	vector<string> items;
	items.push_back("YES");
	items.push_back("NO");
	SetMenu("QUIT?", title_pos, items, positions);

	items.clear();
	positions.clear();
}


QuitDialog::~QuitDialog() {
}

void QuitDialog::OnButtonDown(Button * button) {
	SDL_Event e;
	if (button == buttons[0]) { // case YES
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_QUIT;
		SDL_PushEvent(&e);
	}
	else { // case NO
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_POP;
		SDL_PushEvent(&e);
	}
}

void QuitDialog::Resize(Helpers::SCR_SZ screen_size) {
	GameScreen::Resize(screen_size);
	title->ChangePos(w / 2.f - 45.f, h / 2.f - 65.f);
	buttons[0]->ChangePos(w / 2.f - 75.f, h / 2.f + 30.f);
	buttons[1]->ChangePos(w / 2.f + 35.f, h / 2.f + 30.f);
}

void QuitDialog::HandleEvent(SDL_Event * e) {
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
			ev.user.code = Helpers::RS_POP;
			SDL_PushEvent(&ev);
		}
		if (e->key.keysym.sym == SDLK_RETURN) {
			ev.type = Helpers::RUN_STACK_EVENT;
			ev.user.code = Helpers::RS_QUIT;
			SDL_PushEvent(&ev);
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN)
		if (e->button.button == SDL_BUTTON_LEFT)
			for (auto b : buttons)
				b->OnClick();
}

