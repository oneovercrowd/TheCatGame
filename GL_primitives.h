#pragma once
#include "Helpers.h"

class Point {
protected:
	GLfloat vec[8];
	GLclampf color[3];
	GLuint size;
public:
	GLfloat
		&x1 = vec[0],
		&y1 = vec[1];
	Point(GLfloat x, GLfloat y, const GLclampf *color, GLuint size) {
		this->size = size;
		std::copy_n(color, 3, this->color);
		this->x1 = x;
		this->y1 = y;
	}
	void Move(GLfloat dx, GLfloat dy) {
		x1 += dx;
		y1 += dy;
	}
	void Draw() {
		glColor3fv(color);
		glPointSize((float)size);
		glBegin(GL_POINTS);
		{
			glVertex2f(x1, y1);
		}
		glEnd();
	}
	void ResetColor(const GLclampf *color) {
		std::copy_n(color, 3, this->color);
	}
};

class Line : public Point {
public:
	GLfloat
		&x2 = vec[2],
		&y2 = vec[3];
	Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, const GLclampf *color, GLuint size) :
		Point(x1, y1, color, size) {
		this->x2 = x2;
		this->y2 = y2;
	}
	void Move(GLfloat dx, GLfloat dy) {
		Point::Move(dx, dy);
		x2 += dx;
		y2 += dy;
	}
	void Draw() {
		glColor3fv(color);
		glLineWidth((float)size);
		glBegin(GL_LINES);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		}
		glEnd();
	}
};


class Triangle : public Line {
public:
	GLfloat
		&x3 = vec[4],
		&y3 = vec[5];
	Triangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, const GLclampf *color) :
		Line(x1, y1, x2, y2, color, 1) {

		vec[4] = x3;
		vec[5] = y3;
	}
	void Move(GLfloat dx, GLfloat dy) {
		Line::Move(dx, dy);
		vec[4] += dx;
		vec[5] += dy;
	}
	void Draw() {
		glColor3fv(color);
		glBegin(GL_TRIANGLES);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glVertex2f(x3, y3);
		}
		glEnd();
	}
};


class Rect : public Triangle {
protected:
	//
public:
	GLfloat w, h;
	GLfloat
		&x4 = vec[6],
		&y4 = vec[7];

	Rect(GLfloat x1, GLfloat y1, GLfloat w, GLfloat h, const GLclampf *color) :
		Triangle(x1, y1, x1 + w, y1, x1 + w, y1 + h, color) {

		vec[6] = x1;
		vec[7] = y1 + h;

		this->w = w;
		this->h = h;
	}
	void Move(GLfloat dx, GLfloat dy) {
		Triangle::Move(dx, dy);
		vec[6] += dx;
		vec[7] += dy;
	}
	void Draw() {
		glColor3fv(color);
		glBegin(GL_QUADS);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glVertex2f(x3, y3);
			glVertex2f(x4, y4);
		}
		glEnd();
	}
};


class Round : public Point {
protected:
	GLfloat radius;
	GLuint count;
public:
	Round(GLfloat x, GLfloat y, GLfloat radius, const GLclampf *color, GLuint count = 40):
		Point(x, y, color, 1) {
		this->radius = radius;
		this->count = count;
	}
	void Move(GLfloat dx, GLfloat dy) {
		Point::Move(dx, dy);
	}
	void Draw() {
		GLfloat sector_angle = 2.0f * 3.14f / count;
		glColor3fv(color);
		glBegin(GL_POLYGON);
		{
			for (GLuint i = 0; i < count; i++) {
				glVertex2f(x1 + radius * SDL_cosf(i*sector_angle), y1 + radius * SDL_sinf(i * sector_angle));
			}
		}
		glEnd();
	}
};
//
//class Text {
//	std::string str;
//	std::map<char, GLuint> numbers;
//	std::map<char, GLfloat> w_to_h_ratio; // s
//	//GLfloat size;
//public:
//	Text() {
//		//this->size = size;
//		numbers['0'] = Helpers::LoadGLTexture("textures\\num\\n0.png");
//		numbers['1'] = Helpers::LoadGLTexture("textures\\num\\n1.png");
//		numbers['2'] = Helpers::LoadGLTexture("textures\\num\\n2.png");
//		numbers['3'] = Helpers::LoadGLTexture("textures\\num\\n3.png");
//		numbers['4'] = Helpers::LoadGLTexture("textures\\num\\n4.png");
//		numbers['5'] = Helpers::LoadGLTexture("textures\\num\\n5.png");
//		numbers['6'] = Helpers::LoadGLTexture("textures\\num\\n6.png");
//		numbers['7'] = Helpers::LoadGLTexture("textures\\num\\n7.png");
//		numbers['8'] = Helpers::LoadGLTexture("textures\\num\\n8.png");
//		numbers['9'] = Helpers::LoadGLTexture("textures\\num\\n9.png");
//		numbers['.'] = Helpers::LoadGLTexture("textures\\num\\dot.png");
//		numbers['-'] = Helpers::LoadGLTexture("textures\\num\\minus.png");
//		w_to_h_ratio['0'] = 29.0f / 38.0f;
//		w_to_h_ratio['1'] = 28.0f / 38.0f;
//		w_to_h_ratio['2'] = 30.0f / 38.0f;
//		w_to_h_ratio['3'] = 30.0f / 38.0f;
//		w_to_h_ratio['4'] = 30.0f / 38.0f;
//		w_to_h_ratio['5'] = 30.0f / 38.0f;
//		w_to_h_ratio['6'] = 31.0f / 38.0f;
//		w_to_h_ratio['7'] = 30.0f / 38.0f;
//		w_to_h_ratio['8'] = 31.0f / 38.0f;
//		w_to_h_ratio['9'] = 30.0f / 38.0f;
//		w_to_h_ratio['.'] = 15.0f / 38.0f;
//		w_to_h_ratio['-'] = 15.0f / 38.0f;
//	}
//	void DrawInt(GLfloat x, GLfloat y, GLfloat size, int num_to_draw) {
//		glEnable(GL_TEXTURE_2D);
//		glPushMatrix();
//		{
//			glTranslatef(x, y, 0.0f);
//			str = std::to_string(num_to_draw);
//			for (auto ch : str) {
//				glBindTexture(GL_TEXTURE_2D, numbers[ch]);
//				glBegin(GL_QUADS);
//				{
//					glTexCoord2f(0.f, 1.f); glVertex2f(0, 0);
//					glTexCoord2f(1.f, 1.f); glVertex2f(size * w_to_h_ratio[ch], 0);
//					glTexCoord2f(1.f, 0.f); glVertex2f(size * w_to_h_ratio[ch], size);
//					glTexCoord2f(0.f, 0.f); glVertex2f(0, size);
//				}
//				glEnd();
//				glTranslatef(size * w_to_h_ratio[ch], 0.0f, 0.0f);
//			}
//		}
//		glPopMatrix();
//		glDisable(GL_TEXTURE_2D);
//	}
//};