#pragma once
#include "Helpers.h"
#include "Sprite.h"

enum SPRITE_NAMES {SP_BANG};

class SpriteGenerator
{
protected:
	/* SP_BANG */
	GLuint sp_bang_texture;
	int sp_bang_lifetime;
	bool sp_bang_is_dynamic;
	Helpers::FRect sp_bang_rect;
public:
	SpriteGenerator();
	~SpriteGenerator();
	Sprite* GenSprite(SPRITE_NAMES sprite_name, GLfloat cen_x, GLfloat cen_y);
};

