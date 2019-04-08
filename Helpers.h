#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#include <Windows.h>
#include <string>
#include <map>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>

namespace Helpers {
	static float gen_random_float(float min, float max) {
		return ((max - min) * ((float)rand() / RAND_MAX)) + min;
	}
	struct GameOverData {
		std::string message; // 1 - first, 2 - second, 0 - draw
		GLuint first_scores, second_scores;
	};
	struct FRect {
		GLfloat vec[4];
		GLfloat &x = vec[0],
			&y = vec[1],
			&w = vec[2],
			&h = vec[3];
		FRect(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
		FRect(const FRect &obj) {
			std::copy(obj.vec, obj.vec + 4, vec);
		}
		FRect() {
			std::fill_n(vec, 4, 0.f);
		}
		FRect& operator= (const FRect &obj) {
			std::copy(obj.vec, obj.vec + 4, vec);
			return *this;
		}
	};
	enum SCR_SZ { N800X600, N1024X768, N1280X1024, W1280X720, W1366X768, W1920X1080 };
	const Uint32 RUN_STACK_EVENT = SDL_USEREVENT + 1;
	// run stack event types
	const int RS_PUSH = 0;
	const int RS_QUIT = 1;
	const int RS_GAME_OPTIONS_CHANGED = 2;
	const int RS_SCREEN_OPTIONS_CHANGED = 3;
	const int RS_POP_AND_RESET = 4;
	const int RS_POP = 5;
	const int RS_GO_TO_MAIN_MENU = 6;


	const Uint32 CAT_GAME_EVENT = SDL_USEREVENT + 2;
	// userevent types
	const int CAT_STRIKE = 0; // bullet and static collision
	const int CAT_HIT = 1; // bullet and dynamic collision
	const int CLLD_B_HR = 2; // bullet and hero collision
	const int CLLD_B_EN = 3; // bullet and enemy collision
	const int WEAP_CD = 4; // weapon cooled down
	const int WEAP_RL = 5; // weapon reloaded
	const int DIED_EN = 6; // enemy died
	const int DIED_HR = 7; // hero died
	const int DIED_PL = 8; // player died
	const int GAME_OVER = 9; // game over
	const int BUTTON_DOWN = 10; // menu button down
	/*const int NEWEVENT = 11;
	const int NEWEVENT = 12;
	const int NEWEVENT = 13;
	const int NEWEVENT = 14;
	const int NEWEVENT = 15;
	const int NEWEVENT = 16;
	const int NEWEVENT = 17;
	const int NEWEVENT = 18;
	const int NEWEVENT = 19;
	const int NEWEVENT = 20;*/

	// checks if two rects collide
	bool CollideRR(SDL_Rect &rect1, SDL_Rect &rect2);
	bool CollideRR(FRect &rect1, FRect &rect2);
	// checks if two circles collide
	bool CollideCC(double x1, double y1, double r1, double x2, double y2, double r2);
	// checks if rect collides with circle
	bool CollideRC(SDL_Rect &rect, double x, double y, double r);
	bool CollideRC(FRect &rect, double x, double y, double r);
	// loads texture in renderer by filename
	SDL_Texture *LoadTexture(SDL_Renderer *renderer, std::string name);
	GLuint GLTextureFromSurface(SDL_Surface *sur);
	GLuint GLTextureText(GLfloat w, GLfloat h, const std::string text, GLclampf *color, const std::string fontname, int ptsize);
	GLuint LoadGLTexture(std::string name);
	// radians to degrees
	double Degree(double rad);
	// makes SDL_Color from R, G, B, A values
	SDL_Color Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	Uint32 callback1(Uint32 interval, void *param);
	void PushUserEvent(Uint32 event_type, Sint32 user_type, void* data1, void* data2);
};


