#pragma once
#include "Helpers.h"

#define AFF_HEALTH 1
#define AFF_SCORES 2
#define AFF_SPEEDUP 4

class Item
{
protected:
	GLfloat speed, acceleration;
	GLuint texture;
	Helpers::FRect rect;
public:
	GLuint scores_up;
	GLfloat hp_up;
	Item(Helpers::FRect rect, GLfloat speed, GLfloat acceleration, GLuint texture_id);
	void Update(Uint32 dt);
	void Draw();
	bool IsOnGround();
	Helpers::FRect& GetHitBox() {
		return rect;
	}
	virtual GLuint GetAffects() {
		return 0U;
	}
};

class Sausage : public Item
{
public:
	Sausage(Helpers::FRect rect, GLfloat speed, GLfloat acceleration, GLuint scores, GLfloat hp, GLuint texture_id);
	GLuint GetAffects() {
		return AFF_HEALTH | AFF_SCORES;
	}
};

class SpeedUp : public Item
{
public:
	SpeedUp(Helpers::FRect rect, GLfloat speed, GLfloat acceleration, GLuint texture_id);
	GLuint GetAffects() {
		return AFF_SCORES | AFF_SPEEDUP;
	}
};
