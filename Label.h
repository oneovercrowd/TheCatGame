#pragma once
#include <string>
#include <Windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>

#include "GLTextCustom.h"

using namespace std;

class Label
/*
class of on-screen transparent label
*/
{
protected:
	Helpers::FRect rect;
	GLTextStatic *text;
public:
	Label(Helpers::FRect &rect, const std::string text_str, GLclampf *color) { // if rect.w is 0 it will be calculated automatically
		this->rect.x = rect.x;
		this->rect.y = rect.y;
		this->rect.h = rect.h;
		if (rect.w != 0) {
			this->rect.w = rect.w;
		}
		else {
			this->rect.w = GLText::GetWidth(text_str, rect.h);
		}
		this->text = new GLTextStatic(text_str.c_str(), color, rect.x, rect.y, rect.h);
	}
	~Label() {
	}
	void OnDraw() {
		text->DrawInverted();
	}
	void ChangePos(GLfloat dx, GLfloat dy) {
		rect.x += dx;
		rect.y += dy;
	}
	void Reset(std::string new_text, const GLclampf *new_color) {
		text->Reset(new_text.c_str(), new_color);
	}
	void ResetText(std::string new_text) {
		text->ResetText(new_text.c_str());
	}
};
