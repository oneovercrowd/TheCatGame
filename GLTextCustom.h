#pragma once
#include "Helpers.h"
/* CLASS ALLOWS US TO DRAW TEXT */

struct t_ch {
	GLuint texture;
	GLfloat ratio; // w/h ratio
};
class GLText {
protected:
	static t_ch* chars;
public:
	static void Init(std::string font_name);
	static void DrawGLText(GLfloat x, GLfloat y, GLfloat size, const char* text);
	static void DrawGLText(GLfloat x, GLfloat y, GLfloat size, const char* text, const GLclampf* color);
	static void DrawGLTextInverted(GLfloat x, GLfloat y, GLfloat size, const char* text);
	static void DrawGLTextInverted(GLfloat x, GLfloat y, GLfloat size, const char* text, const GLclampf* color);
	static GLfloat GetWidth(std::string str, GLfloat size); // return total width of text (sum of all char's width's)
	static void Uninit();
};
class GLTextStatic : public GLText
{
protected:
	std::string txt;
	GLclampf color[3];
	GLfloat x, y, size;
public:
	GLTextStatic(const char* text, const GLclampf* color, GLfloat x, GLfloat y, GLfloat size);
	void Reset(const char* text, const GLclampf* color);
	void ResetText(const char* text);
	void ResetColor(const GLclampf* color);
	void Draw();
	void DrawInverted(); // draw text inverted by x (needed when use screen coords)
	std::string GetText();
};
