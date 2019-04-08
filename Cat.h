#pragma once
#include "GL_primitives.h"
enum CAT_COLORS {CAT_ORANGE, CAT_WHITE};
#define FACE_NORMAL 0
#define FACE_INJUMP 1
#define FACE_EAT 2
constexpr float leg_step = 1.0f / 5.0f;

#define AFF_HEALTH 1
#define AFF_SCORES 2
#define AFF_SPEEDUP 4

class Cat
{
private:
	void DrawLegShifted(Helpers::FRect &rect, GLuint tex, GLfloat shift) { // GL_TEXTURE_2D must be enabled
		glBindTexture(GL_TEXTURE_2D, tex);
		glPushMatrix(); {
			glTranslatef(rect.x, rect.y, 0.0f);
			glBegin(GL_QUAD_STRIP);
			{
				for (GLfloat i = 0.0f; i <= 1.0f; i += leg_step) {
					glTexCoord2f(0.0f, 1.0f - i); glVertex2f(0.0f + shift, rect.h * i);
					glTexCoord2f(1.0f, 1.0f - i); glVertex2f(rect.w + 0.0f + shift, rect.h * i);
					shift /= 2;
				}
			}
			glEnd();
		}
		glPopMatrix();
	}
protected:
	bool turned_left;

	/* body rect. used in drawing */
	Helpers::FRect rect;
	/* tail's rect. used in drawing. relative to cat's position */
	Helpers::FRect tail_box;
	/* leg boxes */
	Helpers::FRect leg0_box;
	Helpers::FRect leg1_box;
	Helpers::FRect leg2_box;
	Helpers::FRect leg3_box;
	/* face's rect. used in drawing. relative to cat's position */
	Helpers::FRect face_box;
	/* hit box. used in collisions */
	Helpers::FRect hit_box;
	/* strike box. used in collisions. it is an area where another cat takes hit */
	/* it is small box in front of this cat's face */
	Helpers::FRect strike_box;
	/* strike drawing rect */
	Helpers::FRect strike_rect;

	/* passed distance. used in leg's animation */
	GLfloat passed;
	/* max X position */
	GLfloat max_x;
	/* current cat's speed */
	GLfloat speed;
	/* Y acceleration */
	GLfloat gravity;
	/* cat's own acceleration */
	GLfloat move_acceleration;
	/* force of friction's acceleration */
	GLfloat stop_acceleration;
	/* Y speed */
	GLfloat jump_speed;
	/* jump's start speed */
	GLfloat base_jump_speed;
	/* limit of X speed */
	GLfloat max_speed;
	/* speed given to another cat with strike */
	GLfloat strike_speed;

	/* current health */
	GLfloat health;
	/* health's descent speed */
	GLfloat hunger;

	/* body texture */
	GLuint texture_body;

	/* texture of tail */
	GLuint texture_tail;

	/* textures of legs */
	/* 0, 2 - front-end; 1, 3 - back-end. in left-to-right order */
	GLuint texture_leg0;
	GLuint texture_leg1;
	GLuint texture_leg2;
	GLuint texture_leg3;

	/* strike texture */
	GLuint texture_strike;

	/* map of face textures */
	std::map<Uint8, GLuint> face_tex;
	/* current face texture index */
	Uint8 curr_face;

	/* LEG SHIFT */
	GLfloat leg_0_3_shift;
	GLfloat leg_0_3_shift_modifier; // +1 or -1
	GLfloat leg_1_2_shift;
	GLfloat leg_1_2_shift_modifier; // +1 or -1

	/* timer for showing EAT FACE */
	Uint32 eat_timer;
	/* default time to show EAT FACE */
	Uint32 eat_delay;
	/* timer for showing EAT FACE */
	Uint32 strike_timer;
	/* default time to show EAT FACE */
	Uint32 strike_delay;
public:
	/* flag of SPEED UP bonus */
	bool speed_up;
	/* flag of EAT MODE */
	bool eat;
	/* flag of STRIKE MODE */
	bool strike;
	/* flags of direction buttons */
	bool go_left;
	bool go_right;
	/* flag "is in jump now" */
	bool in_jump;
	/* current scores */
	GLuint scores;
	/* constructor */
	Cat(GLfloat x, GLfloat y, GLfloat max_x, CAT_COLORS color);
	~Cat();
	void Update(Uint32 dt);
	void Draw();
	/* takes gifts of eaten sausage */
	void Eat(GLuint d_scores, GLfloat d_health, GLuint affects);
	/* begin jump */
	void Jump();
	/* pull strike event */
	void Strike();
	/* set speed to 0.0f, used on succesful strike */
	void Stop() {
		speed = 0.0f;
	}
	/* getter */
	GLfloat GetSpeed() {
		return speed;
	}
	/* returns strike speed with correct sing (by direction) */
	GLfloat GetStrikeSpeed() {
		if (turned_left)
			return -strike_speed;
		else
			return strike_speed;
	}
	/* update speed if striken by another cat */
	void TakeHit(GLfloat velocity) {
		speed += velocity;
	}
	/* checks if dead or alive */
	bool IsDead() {
		return health <= 0.0f;
	}
	/* getter */
	Helpers::FRect& GetHitBox() {
		return hit_box;
	}
	/* calculates and returns strike_box */
	Helpers::FRect& GetStrikeBox() {
		strike_box.y = hit_box.y;
		if (turned_left) {
			strike_box.x = hit_box.x + 0.3433f - strike_box.w;
		}
		else {
			strike_box.x = hit_box.x + hit_box.w - 0.3433f;
		}
		return strike_box;
	}
	/* getter */
	GLfloat GetHealth() {
		return health;
	}
	GLfloat GetStrikePoint_x() {
		if (turned_left) {
			return rect.x + rect.w - strike_rect.x - strike_rect.w;
		}
		else {
			return rect.x + strike_rect.x + strike_rect.w;
		}
	}
	GLfloat GetStrikePoint_y() {
		return rect.y + strike_rect.y + strike_rect.h / 2;
	}
};

