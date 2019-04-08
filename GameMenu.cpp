#include "GameMenu.h"
#include <cmath>

void GameMenu::SetMenu(const string title, Helpers::FRect &title_pos, const vector<string>& items, const vector<Helpers::FRect>& positions) {
	Button *temp_button;
	GLclampf color[] = { 1.0f, 1.0f, 1.0f },
		alt_color[] = { 1.0f, 0.2f, 0.2f };
	if (items.size() == positions.size()) {
		for (unsigned int i = 0; i < items.size(); i++) {
			temp_button = new Button(positions[i], items[i], color, alt_color);
			buttons.push_back(temp_button);
		}
	}
	this->title = new Label(title_pos, title, color);
}

GameMenu::GameMenu(Helpers::SCR_SZ screen_size, string screen_name):
	GameScreen(MENU_FRAME_TIME, screen_size, screen_name) {
	title = nullptr;
}

GameMenu::~GameMenu() {
	for (auto button : buttons)
		delete button;
	buttons.clear();
	delete title;
}

void GameMenu::HandleEvent(SDL_Event * e) {
	SDL_Event ev;
	if (e->type == SDL_USEREVENT) {
		if (e->user.code == Helpers::BUTTON_DOWN) {
			OnButtonDown((Button*)(e->user.data1));
		}
	}
	else if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_ESCAPE) {
			/* BY DEFAULT THROW RS_POP EVENT TO GO BACK */
			ev.type = Helpers::RUN_STACK_EVENT;
			ev.user.code = Helpers::RS_POP;
			SDL_PushEvent(&ev);
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN)
		if (e->button.button == SDL_BUTTON_LEFT)
			for (auto b : buttons)
				b->OnClick();
}

void GameMenu::OnDraw() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	title->OnDraw();
	for (auto b : buttons) {
		b->OnDraw();
	}
}

void GameMenu::Update() {
	for (auto b : buttons)
		b->CheckMousePos(m_x, m_y);
}

void GameMenu::Reset() {
	for (auto b : buttons) {
		b->CheckMousePos(-1, -1); // this will reset all flags to false
	}
}
