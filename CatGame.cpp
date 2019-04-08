#include "CatGame.h"



void CatGame_master::PushGameOverEvent() {
	go_data.first_scores = first_cat->scores;
	go_data.second_scores = second_cat->scores;
	Helpers::PushUserEvent(Helpers::RUN_STACK_EVENT, Helpers::RS_PUSH, (void*)"game_over_menu",(void*)(&go_data));
}

void CatGame_master::DrawHealthBox(GLfloat x, GLfloat y, GLfloat health) {
	glPushMatrix();
	{
		glTranslatef(x, y, 0.0f);
		glColor3f(2 * (1 - health), 2 * health, 0.0f);
		glBegin(GL_QUADS);
		{
			glVertex2f(0.0f, 0.0f);
			glVertex2f(health, 0.0f);
			glVertex2f(health, 0.2f);
			glVertex2f(0.0f, 0.2f);
		}
		glEnd();
		glColor3f(0.6f, 0.6f, 1.0f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
			glVertex2f(1.0f, 0.2f);
			glVertex2f(0.0f, 0.2f);
		}
		glEnd();
	}
	glPopMatrix();
}

CatGame_master::CatGame_master(GLfloat width, GLfloat height):
	Runnable(GAME_MS_PER_FRAME)
{
	item_gen = new ItemGenerator(0.f, width, height);
	sprite_gen = new SpriteGenerator();
	this->width = width;
	this->height = height;

	basic_game_time = 160000U;
	game_timer = basic_game_time;

	drop_timer = 0U; // count ms until drop
	drop_delay = 1337U; // ms per drop

	first_cat = new Cat(1.0f, 0.0f, width, CAT_ORANGE);
	second_cat = new Cat(width - 2.5f, 0.0f, width, CAT_WHITE);
	items.push_back(item_gen->MakeSausage());
}


CatGame_master::~CatGame_master() {
	delete first_cat;
	delete second_cat;
	for (auto x : items)
		delete x;
	items.clear();
	delete item_gen;
	delete sprite_gen;
}

void CatGame_master::SetCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, 1, -1);
}

void CatGame_master::Update() {
	/* check if game ended with anyone's death */
	/* in any of death cases pull game_over userevent with result */
	if (first_cat->IsDead())
		if (second_cat->IsDead()) {
			go_data.message = "DRAW!";
			PushGameOverEvent();
		}
		else {
			go_data.message = "WHITE CAT WON!";
			PushGameOverEvent();
		}
	else if (second_cat->IsDead()) {
		go_data.message = "ORANGE CAT WON!";
		PushGameOverEvent();
	}
	/* CHECK IF TIME IS OUT */
	game_timer -= dt;
	if (game_timer > basic_game_time || game_timer == 0) {
		game_timer = 0U;
		if (first_cat->scores == second_cat->scores){
			go_data.message = "DRAW!";
			PushGameOverEvent();
		}
		else if (first_cat->scores < second_cat->scores) {
			go_data.message = "WHITE CAT WON!";
			PushGameOverEvent();
		}
		else {
			go_data.message = "ORANGE CAT WON!";
			PushGameOverEvent();
		}
	}
	/* if reached here, game is still on */
	/* so handle inputs                  */
	first_cat->go_right = kbstate[SDL_SCANCODE_D];
	first_cat->go_left = kbstate[SDL_SCANCODE_A];
	second_cat->go_right = kbstate[SDL_SCANCODE_RIGHT];
	second_cat->go_left = kbstate[SDL_SCANCODE_LEFT];
	
	/* ADD NEW ITEM IF IT'S TIME */
	if (drop_timer >= drop_delay) {
		items.push_back(item_gen->MakeItem());
		drop_timer = 0;
	}
	else
		drop_timer += dt;
	/* and update everything */
	for (auto it : items) {
		it->Update(dt);
	}
	for (deque<Sprite*>::iterator i = sprites.begin(); i != sprites.end();) {
		(*i)->Update(dt);
		if ((*i)->Alive()) {
			i++;
		}
		else {
			delete *i;
			i = sprites.erase(i);
		}
	}
	first_cat->Update(dt);
	second_cat->Update(dt);
	/* now check if any item are eaten by cats*/
	for (deque<Item*>::iterator i = items.begin(); i != items.end();) {
		if ((*i)->IsOnGround()) {
			delete *i;
			i = items.erase(i);
		}
		else {
			bool first_ate = Helpers::CollideRR((*i)->GetHitBox(), first_cat->GetHitBox()),
				second_ate = Helpers::CollideRR((*i)->GetHitBox(), second_cat->GetHitBox());
			if (first_ate || second_ate) { // IF SOMEBODY ATE THIS ONE
				if (first_ate) { // IF FIRST ATE
					if (second_ate) { // IF BOTH CATS ATE
						first_cat->Eat((*i)->scores_up / 2, (*i)->hp_up * 0.75f, (*i)->GetAffects());
						second_cat->Eat((*i)->scores_up / 2, (*i)->hp_up * 0.75f, (*i)->GetAffects());
					}
					else { // IF ONLY FIRST
						first_cat->Eat((*i)->scores_up, (*i)->hp_up, (*i)->GetAffects());
					}
				}
				else { // IF ONLY SECOND
					second_cat->Eat((*i)->scores_up, (*i)->hp_up, (*i)->GetAffects());
				}
				delete *i;
				i = items.erase(i);
			}
			else {
				i++;
			}
		}
	}
}

void CatGame_master::OnDraw() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.1f, 0.2f, 0.3f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
	/* DRAW CATS */
	first_cat->Draw();
	second_cat->Draw();

	/* DRAW SAUSAGES */
	for (auto x : items) {
		x->Draw();
	}
	/* DRAW SPRITES */
	for (auto x : sprites) {
		x->Draw();
	}

	/* PRINT SCORES */
	char t[17];
	_itoa_s(first_cat->scores, t, 10);
	GLText::DrawGLText(0.1f, 5.5f, 0.3f, t, first_b_color);
	_itoa_s(second_cat->scores, t, 10);
	GLText::DrawGLText(width - 1.5f, 5.5f, 0.3f, t, second_b_color);
	/* PRINT HEALTHES */
	DrawHealthBox(0.1f, 5.2f, first_cat->GetHealth());
	DrawHealthBox(width - 1.5f, 5.2f, second_cat->GetHealth());
	/* PRINT TIMER */
	_itoa_s(game_timer / 1000U, t, 10);
	GLText::DrawGLText(width / 2 - 0.5f, 5.5f, 0.3f, t, timer_color);
}

void CatGame_master::HandleEvent(SDL_Event * e) {
	SDL_Event ev;
	switch (e->type)
	{
	case SDL_KEYDOWN:
		if (e->key.keysym.sym == SDLK_ESCAPE) {
			ev.type = Helpers::RUN_STACK_EVENT;
			ev.user.code = Helpers::RS_POP;
			SDL_PushEvent(&ev);
		}
		if (e->key.keysym.sym == SDLK_w) {
			first_cat->Jump();
		}
		if (e->key.keysym.sym == SDLK_UP) {
			second_cat->Jump();
		}
		if (e->key.keysym.sym == SDLK_SPACE) {
			first_cat->Strike();
		}
		if (e->key.keysym.sym == SDLK_RCTRL) {
			second_cat->Strike();
		}
		break;
	case Helpers::CAT_GAME_EVENT:
		if (e->user.code == Helpers::CAT_STRIKE) {
			Cat *cat_one, *cat_two;
			cat_one = (Cat*)e->user.data1;
			if (cat_one == first_cat)
				cat_two = second_cat;
			else
				cat_two = first_cat;
			/* if cat_one hits cat_two */
			if (Helpers::CollideRR(cat_one->GetStrikeBox(), cat_two->GetHitBox())) {
				cat_two->TakeHit(cat_one->GetSpeed() + cat_one->GetStrikeSpeed());
				cat_one->Stop();
				cat_one->Eat(0U, 0.05f, AFF_HEALTH);
				cat_two->Eat(0U, -0.05f, AFF_HEALTH);
				//sprites.push_back(sprite_gen->GenSprite(SP_BANG, cat_one->GetStrikePoint_x(), cat_one->GetStrikePoint_y()));
			}
		}
		break;
	default:
		break;
	}
}

void CatGame_master::Reset() {
	delete first_cat;
	delete second_cat;
	for (auto x : items)
		delete x;
	items.clear();

	drop_timer = 0U;
	game_timer = basic_game_time;

	first_cat = new Cat(1.0f, 0.0f, width, CAT_ORANGE);
	second_cat = new Cat(width - 2.5f, 0.0f, width, CAT_WHITE);

	items.push_back(item_gen->MakeSausage());
}
