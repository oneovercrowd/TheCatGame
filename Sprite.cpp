#include "Sprite.h"


Sprite::Sprite(int life_time, Helpers::FRect rect, GLuint texture) {
	this->life_time = life_time;
	this->rect = rect;
	this->texture = texture;
}

void Sprite::Update(Uint32 dt) {
	rect.x += dt * vx;
	rect.y += dt * vy;
	life_time -= dt;
}

void Sprite::Draw() {
	glPushMatrix();
	{
		glTranslatef(rect.x, rect.y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(rect.w, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(rect.w, rect.h);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, rect.h);
		}
		glEnd();
	}
	glPopMatrix();
}

BangSprite::BangSprite(Helpers::FRect rect, GLuint texture) :
	Sprite(basic_life_time, rect, texture)
{
}

