#include "Cat.h"




Cat::Cat(GLfloat x, GLfloat y, GLfloat max_x, CAT_COLORS color) {
	/* EAT TIMERS */
	eat_delay = 175U;
	eat_timer = 0U;
	eat = false;
	/* STRIKE TIMERS */
	strike_delay = 175U;
	strike_timer = 0U;
	strike = false;
	/*  */
	scores = 0;

	turned_left = x > max_x / 2.f;  // if cat starts on right half of screen

	/* texture rect and hit box */
	this->max_x = max_x;
	rect.x = hit_box.x = x;
	rect.y = hit_box.y = y;
	rect.w = hit_box.w = 1.5f;
	rect.h = 1.0f;
	hit_box.h = 0.5f;
	/* TAIL BOX */
	tail_box.w = rect.w * 0.5556f;
	tail_box.h = rect.h * 0.8333f;
	tail_box.x = -tail_box.w;
	tail_box.y = 0.f;
	/* LEG BOXES */
	leg0_box.w = 0.2967f;
	leg0_box.h = 0.4f;
	leg0_box.x = 0.24f;
	leg0_box.y = 0.0f;

	leg1_box.w = 0.2333f;
	leg1_box.h = 0.4f;
	leg1_box.x = 0.5367f;
	leg1_box.y = 0.0f;

	leg2_box.w = 0.2833f;
	leg2_box.h = 0.4f;
	leg2_box.x = 0.8733f;
	leg2_box.y = 0.0f;

	leg3_box.w = 0.3433f;
	leg3_box.h = 0.4f;
	leg3_box.x = 1.1567f;
	leg3_box.y = 0.0f;
	/* FACE BOX */
	face_box.x = rect.w * 0.5f;
	face_box.y = rect.h * 0.4f;
	face_box.w = rect.w * 0.5f;
	face_box.h = rect.h * 0.6f;
	/* STRIKE BOX */
	/* size is always the same */
	/* X and Y position depends on cat's face direction so it is calculated on getter*/
	strike_box.w = 0.51f;
	strike_box.h = hit_box.h;
	/* STRIKE RECT (relative)*/
	strike_rect.w = 0.51f;
	strike_rect.h = 1.0f;
	strike_rect.x = 1.1567f;
	strike_rect.y = 0.0f;

	/* init kinematics */
	speed = 0.0f;
	jump_speed = 0.0f;

	gravity = -0.000008f;
	move_acceleration = 0.000008f;
	stop_acceleration = 0.000002f;
	base_jump_speed = 0.004f;
	max_speed = 0.003f;
	strike_speed = 0.001f;

	/* init health */
	health = 1.0f;
	hunger = 0.00003f;

	/* init textures */
	curr_face = 0U;


	switch (color)
	{
	case CAT_ORANGE:
		texture_body = Helpers::LoadGLTexture("cats\\cat_red_body.png");
		texture_tail = Helpers::LoadGLTexture("cats\\tail_red.png");
		texture_leg0 = Helpers::LoadGLTexture("cats\\cat_red_leg0.png");
		texture_leg1 = Helpers::LoadGLTexture("cats\\cat_red_leg1.png");
		texture_leg2 = Helpers::LoadGLTexture("cats\\cat_red_leg2.png");
		texture_leg3 = Helpers::LoadGLTexture("cats\\cat_red_leg3.png");
		texture_strike = Helpers::LoadGLTexture("cats\\cat_red_strike.png");
		//
		face_tex[FACE_NORMAL] = Helpers::LoadGLTexture("cats\\face_red.png");
		face_tex[FACE_INJUMP] = Helpers::LoadGLTexture("cats\\face_red_j.png");
		face_tex[FACE_EAT] = Helpers::LoadGLTexture("cats\\face_red_e.png");
		face_tex[FACE_INJUMP + FACE_EAT] = Helpers::LoadGLTexture("cats\\face_red_je.png");
		break;
	case CAT_WHITE:
		texture_body = Helpers::LoadGLTexture("cats\\cat_white_body.png");
		texture_tail = Helpers::LoadGLTexture("cats\\tail_white.png");
		texture_leg0 = Helpers::LoadGLTexture("cats\\cat_white_leg0.png");
		texture_leg1 = Helpers::LoadGLTexture("cats\\cat_white_leg1.png");
		texture_leg2 = Helpers::LoadGLTexture("cats\\cat_white_leg2.png");
		texture_leg3 = Helpers::LoadGLTexture("cats\\cat_white_leg3.png");
		texture_strike = Helpers::LoadGLTexture("cats\\cat_white_strike.png");
		//
		face_tex[FACE_NORMAL] = Helpers::LoadGLTexture("cats\\face_red.png");
		face_tex[FACE_INJUMP] = Helpers::LoadGLTexture("cats\\face_red_j.png");
		face_tex[FACE_EAT] = Helpers::LoadGLTexture("cats\\face_red_e.png");
		face_tex[FACE_INJUMP + FACE_EAT] = Helpers::LoadGLTexture("cats\\face_red_je.png");
		break;
	default:
		break;
	}
	/* init leg animation */
	leg_0_3_shift = 0.0f;
	leg_0_3_shift_modifier = -1.0f;
	leg_1_2_shift = 0.0f;
	leg_1_2_shift_modifier = 1.0f;
	/* init direction flags */
	go_left = go_right = in_jump = false;
}

Cat::~Cat() {
	glDeleteTextures(1, &texture_body);
	glDeleteTextures(1, &texture_tail);
	glDeleteTextures(1, &texture_leg0);
	glDeleteTextures(1, &texture_leg1);
	glDeleteTextures(1, &texture_leg2);
	glDeleteTextures(1, &texture_leg3);
	glDeleteTextures(1, &texture_strike);
	for (auto x : face_tex)
		glDeleteTextures(1, &(x.second));
}

void Cat::Update(Uint32 dt) {
	GLfloat dx = 0.f, dy = 0.f;
	// when on ground calculate x moving
	if (!in_jump) {
		if (go_right) {
			speed += dt * move_acceleration;
			if (speed > max_speed)
				speed = max_speed;
			// check if reached right screen border
		}
		else if (go_left) {
			speed -= dt * move_acceleration;
			if (speed < -max_speed)
				speed = -max_speed;
		}
		//else
		{
			if (fabs(speed) <= 0.00001f)
				speed = 0.0f;
			else if (speed > 0) {
				speed -= dt * stop_acceleration;
				turned_left = false;
			}
			else {
				speed += dt * stop_acceleration;
				turned_left = true;
			}
		}
	}
	// cat can't walk when jumping and friction does not affects it
	// 
	if (in_jump) {
		/* calculate Y position change */
		dy = dt * jump_speed;
		/* calculate Y speed change */
		jump_speed += dt * gravity;
		/* check if cat will hit the floor this step */
		if (rect.y + dy <= 0) {
			/* make him land right on the floor */
			dy = -rect.y;
			/* end jumping */
			jump_speed = 0;
			in_jump = false;
			/* reset leg's animation */
			leg_0_3_shift = 0.0f;
			leg_1_2_shift = 0.0f;
		}
	}

	/* update position */
	dx = dt * speed;
	rect.x += dx;
	if (rect.x > max_x - 1.5f) {
		rect.x = max_x - 1.5f;
		speed = 0;
	}
	if (rect.x < 0) {
		rect.x = 0.f;
		speed = 0.f;
	}
	rect.y += dy;
	hit_box.x = rect.x;
	hit_box.y = rect.y;
	/* update health */
	health -= dt * hunger;

	/* calculate leg's animation */
	if (in_jump) {
		if (turned_left) {
			leg_0_3_shift = 0.1f * speed / max_speed;
			leg_1_2_shift = 0.1f * speed / max_speed;
		}
		else {
			leg_0_3_shift = -0.1f * speed / max_speed;
			leg_1_2_shift = -0.1f * speed / max_speed;
		}
	}
	else {
		leg_0_3_shift += fabs(dx) * leg_0_3_shift_modifier;
		if (leg_0_3_shift > 0.1f) {
			leg_0_3_shift = 0.1f;
			leg_0_3_shift_modifier = -1.0f;
		}
		else if (leg_0_3_shift < -0.1f) {
			leg_0_3_shift = -0.1f;
			leg_0_3_shift_modifier = 1.0f;
		}

		leg_1_2_shift += fabs(dx) * leg_1_2_shift_modifier;
		if (leg_1_2_shift > 0.1f) {
			leg_1_2_shift = 0.1f;
			leg_1_2_shift_modifier = -1.0f;
		}
		else if (leg_1_2_shift < -0.1f) {
			leg_1_2_shift = -0.1f;
			leg_1_2_shift_modifier = 1.0f;
		}
	}
	/* strike's animation timer */
	if (strike) {
		strike_timer += dt;
		if (strike_timer >= strike_delay) {
			strike_timer = 0U;
			strike = false;
		}
	}
	//
	curr_face = FACE_NORMAL;
	if (in_jump) {
		curr_face += FACE_INJUMP;
	}
	if (eat) {
		curr_face += FACE_EAT;
		eat_timer += dt;
		if (eat_timer >= eat_delay) {
			eat = false;
		}
	}
}

void Cat::Draw() {
	glPushMatrix();
	{
		glTranslatef(rect.x, rect.y, 0.f);
		/* THIS TURNS CAT LEFT */
		if (turned_left) {
			glTranslatef(0.75f, 0, 0);
			glScalef(-1.0f, 1.0f, 1.0f);
			glTranslatef(-0.75f, 0, 0);
		}

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		/* DRAW BACK-END LEGS */
		/* LEG 1 */
		DrawLegShifted(leg1_box, texture_leg1, leg_1_2_shift);
		/* DRAW BODY */
		glBindTexture(GL_TEXTURE_2D, texture_body);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.f, 0.f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(rect.w, 0.f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(rect.w, rect.h);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.f, rect.h);
		}
		glEnd();
		/* DRAW TAIL */
		glBindTexture(GL_TEXTURE_2D, texture_tail);
		glPushMatrix(); {
			glTranslatef(rect.w * 0.3689f, rect.h * 0.4f, 0.0f);
			glRotatef(health * -30.0f + 10.0f, 0.0f, 0.0f, 1.0f);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0f, 1.0f); glVertex2f(tail_box.x, tail_box.y);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(tail_box.x + tail_box.w, tail_box.y);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(tail_box.x + tail_box.w, tail_box.y + tail_box.h);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(tail_box.x, tail_box.y + tail_box.h);
			}
			glEnd();
		}
		glPopMatrix();
		/* DRAW FRONT-END */
		/* LEG 0 */
		DrawLegShifted(leg0_box, texture_leg0, leg_0_3_shift);
		/* LEG 2 */
		DrawLegShifted(leg2_box, texture_leg2, leg_1_2_shift);
		/* LEG 3 */
		if (strike) {
			glBindTexture(GL_TEXTURE_2D, texture_strike);
			glPushMatrix(); {
				glTranslatef(strike_rect.x, strike_rect.y, 0.0f);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(strike_rect.w, 0.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(strike_rect.w, strike_rect.h);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, strike_rect.h);
				}
				glEnd();
			}
			glPopMatrix();
		}
		else {
			DrawLegShifted(leg3_box, texture_leg3, leg_0_3_shift);
		}
		// draw face
		glBindTexture(GL_TEXTURE_2D, face_tex[curr_face]);
		glPushMatrix(); {
			glTranslatef(face_box.x, face_box.y, 0.0f);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0f, 1.0f); glVertex2f(0.f, 0.f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(face_box.w, 0.f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(face_box.w, face_box.h);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(0.f, face_box.h);
			}
			glEnd();
		}
		glPopMatrix();
		//
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void Cat::Eat(GLuint d_scores, GLfloat d_health, GLuint affects) {
	eat = true;
	eat_timer = 0U;
	if (affects & AFF_HEALTH) {
		health += d_health;
		// limit health by max value = 1.0f
		if (health > 1.0f)
			health = 1.0f;
	}
	if (affects & AFF_SCORES) {
		scores += d_scores;
	}
	if ((affects & AFF_SPEEDUP) && (!speed_up)) {
		max_speed *= 1.5f;
		base_jump_speed *= 1.5f;
		speed_up = true;
	}
}

void Cat::Jump() {
	if (!in_jump) {
		in_jump = true;
		jump_speed = base_jump_speed;
	}
}

void Cat::Strike() {
	SDL_Event e;
	if (!strike) {
		e.type = Helpers::CAT_GAME_EVENT;
		e.user.code = Helpers::CAT_STRIKE;
		e.user.data1 = this;
		SDL_PushEvent(&e);
		strike = true;
		strike_timer = 0U;
	}
	return;
}
