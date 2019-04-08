#pragma once
#include <deque>
#include <random>
#include "Runnable.h"
#include "Cat.h"
#include "ItemGenerator.h"
#include "SpriteGenerator.h"
#include "KeyMap.h"
#include "GLTextCustom.h"

#define GAME_MS_PER_FRAME 10

class CatGame_master :
	public Runnable
{
private:
	Helpers::GameOverData go_data;
	void PushGameOverEvent();
protected:
	Uint32 game_timer;
	Uint32 basic_game_time;
	ItemGenerator *item_gen;
	SpriteGenerator *sprite_gen;
	GLclampf first_b_color[3] = { 0.9f, 0.33f, 0.0f },
		second_b_color[3] = { 0.6f, 0.6f, 0.6f },
		timer_color[3] = {1.0f, 1.0f, 1.0f};
	KeyMap km;
	GLfloat width, height;
	Uint32 drop_timer, drop_delay;
	Cat *first_cat, *second_cat;
	std::deque<Item*> items;
	std::deque<Sprite*> sprites;

	void DrawHealthBox(GLfloat x, GLfloat y, GLfloat health);
public:
	CatGame_master(GLfloat width = 10.0f, GLfloat height = 7.5f);
	~CatGame_master();
	void SetCamera();
	void Update();
	void OnDraw();
	void HandleEvent(SDL_Event *e);
	void Reset();
};

