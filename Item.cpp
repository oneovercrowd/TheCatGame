#include "Item.h"


Item::Item(Helpers::FRect rect, GLfloat speed, GLfloat acceleration, GLuint texture_id) {
	this->rect = rect;
	this->speed = speed;
	this->acceleration = acceleration;
	texture = texture_id;
	this->scores_up = 0U;
	this->hp_up = 0.0f;
}

void Item::Update(Uint32 dt) {
	speed += dt * acceleration;
	GLfloat dy = -(dt * speed);
	rect.y += dy;
}

void Item::Draw() {
	glPushMatrix();
	{
		glTranslatef(rect.x, rect.y, 0.f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.f, 0.f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(rect.w, 0.f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(rect.w, rect.h);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.f, rect.h);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

bool Item::IsOnGround()
{
	return rect.y <= 0.1f;
}

Sausage::Sausage(Helpers::FRect rect, GLfloat speed, GLfloat acceleration, GLuint scores, GLfloat hp, GLuint texture_id) :
	Item(rect, speed, acceleration, texture_id)
{
	this->scores_up = scores;
	this->hp_up = hp;
}

SpeedUp::SpeedUp(Helpers::FRect rect, GLfloat speed, GLfloat acceleration, GLuint texture_id) :
	Item(rect, speed, acceleration, texture_id)
{
	this->scores_up = 100U;
};