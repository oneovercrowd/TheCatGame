#pragma once
#include "Runnable.h"
class GameScreen :
	public Runnable
/*
abstract class of runnable in mainloop gamescreen
*/
{
protected:
	struct {
		GLclampf r, g, b, a;
	} background_color;
	int w, h; // screen size
	std::string name; // screen name
public:
	GameScreen(int ms_per_frame, Helpers::SCR_SZ screen_size, string screen_name) :
		Runnable(ms_per_frame) {
		this->name = screen_name;
		this->background_color.r = 0.4f;
		this->background_color.g = 0.3f;
		this->background_color.b = 0.35f;
		this->background_color.a = 1.0f;
		switch (screen_size)
		{
		case Helpers::SCR_SZ::N800X600:
			this->w = 800;
			this->h = 600;
			break;
		case Helpers::SCR_SZ::N1024X768:
			this->w = 1024;
			this->h = 768;
			break;
		case Helpers::SCR_SZ::N1280X1024:
			this->w = 1280;
			this->h = 1024;
			break;
		case Helpers::SCR_SZ::W1280X720:
			this->w = 1280;
			this->h = 720;
			break;
		case Helpers::SCR_SZ::W1366X768:
			this->w = 1366;
			this->h = 768;
			break;
		case Helpers::SCR_SZ::W1920X1080:
			this->w = 1920;
			this->h = 1080;
			break;
		default:
			break;
		}
	};
	void Resize(Helpers::SCR_SZ screen_size) {
		switch (screen_size)
		{
		case Helpers::SCR_SZ::N800X600:
			this->w = 800;
			this->h = 600;
			break;
		case Helpers::SCR_SZ::N1024X768:
			this->w = 1024;
			this->h = 768;
			break;
		case Helpers::SCR_SZ::N1280X1024:
			this->w = 1280;
			this->h = 1024;
			break;
		case Helpers::SCR_SZ::W1280X720:
			this->w = 1280;
			this->h = 720;
			break;
		case Helpers::SCR_SZ::W1366X768:
			this->w = 1366;
			this->h = 768;
			break;
		case Helpers::SCR_SZ::W1920X1080:
			this->w = 1920;
			this->h = 1080;
			break;
		default:
			break;
		}
	}
	virtual ~GameScreen() {
	};
	void SetCamera() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w, h, 0, 1, -1);
	}
};

