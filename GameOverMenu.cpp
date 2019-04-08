#include "GameOverMenu.h"



GameOverMenu::GameOverMenu(Helpers::SCR_SZ screen_size):
	GameMenu(screen_size, "gameover_menu")
{
	Helpers::FRect title_pos;

	title_pos.x = w / 2.f - 150.f;
	title_pos.y = h / 2.f - 200.f;
	title_pos.w = 0.f;
	title_pos.h = 36.f;

	Helpers::FRect temp_rect;
	vector<Helpers::FRect> positions;

	temp_rect.h = 36.f;
	temp_rect.x = w / 2.f - 150.f;
	temp_rect.y = h / 2.f - 10.f;
	positions.push_back(temp_rect);

	temp_rect.y = h / 2.f + 30.f;
	positions.push_back(temp_rect);

	vector<string> items;
	items.push_back("RESTART");
	items.push_back("MAIN MENU");
	SetMenu("GAME OVER", title_pos, items, positions);

	items.clear();
	positions.clear();

	temp_rect.y = h / 2.f - 150.f;
	temp_rect.w = 100.f;
	temp_rect.h = 36.f;
	GLclampf temp_color[] = { 1.0f, 1.0f, 1.0f };
	message = new Label(temp_rect, "message", temp_color);
	temp_rect.y = h / 2.f - 110.f;
	first_scores = new Label(temp_rect, "first", temp_color);
	temp_rect.y = h / 2.f - 70.f;
	second_scores = new Label(temp_rect, "second", temp_color);
}


GameOverMenu::~GameOverMenu() {
	delete first_scores;
	delete second_scores;
	delete message;
}

void GameOverMenu::OnButtonDown(Button * button)
{
	SDL_Event e;
	if (button == buttons[0]) { // case RESTART
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_POP_AND_RESET;
		SDL_PushEvent(&e);
	}
	else { // case GO TO MAIN
		e.type = Helpers::RUN_STACK_EVENT;
		e.user.code = Helpers::RS_GO_TO_MAIN_MENU;
		SDL_PushEvent(&e);
	}
}

void GameOverMenu::Resize(Helpers::SCR_SZ screen_size)
{
	/* DOES NOTHING, NOT CALLED FROM ANYWHERE */
}

void GameOverMenu::SetData(void * new_data) {
	go_data = (Helpers::GameOverData*)new_data;
	message->ResetText(go_data->message);
	first_scores->ResetText(std::to_string(go_data->first_scores));
	second_scores->ResetText(std::to_string(go_data->second_scores));
}

void GameOverMenu::OnDraw() {
	GameMenu::OnDraw();
	message->OnDraw();
	first_scores->OnDraw();
	second_scores->OnDraw();
}

void GameOverMenu::HandleEvent(SDL_Event * e) {
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
			ev.user.code = Helpers::RS_POP_AND_RESET;
			SDL_PushEvent(&ev);
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN)
		if (e->button.button == SDL_BUTTON_LEFT)
			for (auto b : buttons)
				b->OnClick();
}
