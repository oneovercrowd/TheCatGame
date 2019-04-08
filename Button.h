#pragma once
#include "Label.h"


class Button :
	public Label
{
protected:
	GLclampf under_cursor_color[3];  // color of button when it's under cursor
	bool under_cursor;  // flag when button sis under cursor
	GLTextStatic *alt_text;  // text when under cursor
public:
	Button(Helpers::FRect rect, const std::string text, GLclampf *color, GLclampf *alt_color);
	~Button();
	void OnDraw(); // draws button
	void CheckMousePos(int m_x, int m_y); // checks if button is under cursor and sets flag
	void OnClick();  // runs on MouseLeftButton down
	string GetText();  // getter
	void Reset(std::string new_text, const GLclampf *new_color);  // reset text and color
	void ResetText(std::string new_text); // reset text only
};



inline Button::Button(Helpers::FRect rect, const std::string text_str, GLclampf *color, GLclampf *alt_color):
	Label(rect, text_str, color) {
	copy_n(alt_color, 3, under_cursor_color);
	
	alt_text = new GLTextStatic(text_str.c_str(), under_cursor_color, rect.x, rect.y, rect.h);

	under_cursor = false;
}

inline Button::~Button() {
}

inline void Button::OnDraw() {
	if (under_cursor) {
		alt_text->DrawInverted();
	}
	else {
		text->DrawInverted();
	}
}

inline void Button::CheckMousePos(int m_x, int m_y) {
	if (Helpers::CollideRC(rect, m_x, m_y, 0))
		under_cursor = true;
	else
		under_cursor = false;
}

inline void Button::OnClick() {
	if (under_cursor){
		SDL_Event e;
		e.type = SDL_USEREVENT;
		e.user.code = Helpers::BUTTON_DOWN;
		e.user.data1 = this;
		SDL_PushEvent(&e); // will make a copy of e
	}
}

inline string Button::GetText()
{
	return text->GetText();
}

inline void Button::Reset(std::string new_text, const GLclampf * new_color) {
	Label::Reset(new_text, new_color);
	alt_text->Reset(new_text.c_str(), new_color);
}

inline void Button::ResetText(std::string new_text) {
	Label::ResetText(new_text);
	alt_text->ResetText(new_text.c_str());
}
