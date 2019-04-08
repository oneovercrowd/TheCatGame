#pragma once
#include "Helpers.h"

class Sprite
{
private:
	/* basic sprite attributes */
protected:
	int life_time;
	Helpers::FRect rect;
	GLuint texture;
	bool is_dynamic;
	GLfloat vx, vy;
public:
	Sprite(int life_time, Helpers::FRect rect, GLuint texture);
	void Update(Uint32 curr_time);
	void Draw();
	bool Alive() {
		return life_time > 0;
	}
};

class BangSprite : public Sprite {
protected:
	static const int basic_life_time = 200;
public:
	BangSprite(Helpers::FRect rect, GLuint texture);
};