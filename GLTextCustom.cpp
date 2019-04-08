#include "GLTextCustom.h"

t_ch* GLText::chars = nullptr;

void GLText::Init(std::string font_name) {
	chars = new t_ch[256];
	/* LOAD FONT WITH MAX SIZE */
	// load font
	TTF_Font *font = TTF_OpenFont(font_name.c_str(), 48);
	if (!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	/* CREATE SEPARATE TEXTURE FOR EACH CHAR */
	SDL_Surface *temp_surf;
	char temp_str[2];
	temp_str[1] = '\0';
	for (Uint16 i = 1; i < 256; i++) {
		temp_str[0] = char(i);
		temp_surf = TTF_RenderText_Blended(font, temp_str, Helpers::Color(250, 250, 250, 255));
		chars[i].texture = Helpers::GLTextureFromSurface(temp_surf);
		chars[i].ratio = temp_surf->w / float(temp_surf->h);
		SDL_FreeSurface(temp_surf);
	}
}

void GLText::DrawGLText(GLfloat x, GLfloat y, GLfloat size, const char* text) {
	glPushMatrix();
	{
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		for (auto i = 0U; i < strlen(text); i++) {
			glBindTexture(GL_TEXTURE_2D, chars[text[i]].texture);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.f, 1.f); glVertex2f(0, 0);
				glTexCoord2f(1.f, 1.f); glVertex2f(size * chars[text[i]].ratio, 0);
				glTexCoord2f(1.f, 0.f); glVertex2f(size * chars[text[i]].ratio, size);
				glTexCoord2f(0.f, 0.f); glVertex2f(0, size);
			}
			glEnd();
			glTranslatef(size * chars[text[i]].ratio, 0.0f, 0.0f);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void GLText::DrawGLText(GLfloat x, GLfloat y, GLfloat size, const char* text, const GLclampf* color) {
	glPushMatrix();
	{
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		for (auto i = 0U; i < strlen(text); i++) {
			glBindTexture(GL_TEXTURE_2D, chars[text[i]].texture);
			glColor3fv(color);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.f, 1.f); glVertex2f(0, 0);
				glTexCoord2f(1.f, 1.f); glVertex2f(size * chars[text[i]].ratio, 0);
				glTexCoord2f(1.f, 0.f); glVertex2f(size * chars[text[i]].ratio, size);
				glTexCoord2f(0.f, 0.f); glVertex2f(0, size);
			}
			glEnd();
			glTranslatef(size * chars[text[i]].ratio, 0.0f, 0.0f);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void GLText::DrawGLTextInverted(GLfloat x, GLfloat y, GLfloat size, const char * text) {
	glPushMatrix();
	{
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		for (auto i = 0U; i < strlen(text); i++) {
			glBindTexture(GL_TEXTURE_2D, chars[text[i]].texture);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.f, 0.f); glVertex2f(0, 0);
				glTexCoord2f(1.f, 0.f); glVertex2f(size * chars[text[i]].ratio, 0);
				glTexCoord2f(1.f, 1.f); glVertex2f(size * chars[text[i]].ratio, size);
				glTexCoord2f(0.f, 1.f); glVertex2f(0, size);
			}
			glEnd();
			glTranslatef(size * chars[text[i]].ratio, 0.0f, 0.0f);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void GLText::DrawGLTextInverted(GLfloat x, GLfloat y, GLfloat size, const char * text, const GLclampf * color){
	glPushMatrix();
	{
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		for (auto i = 0U; i < strlen(text); i++) {
			glBindTexture(GL_TEXTURE_2D, chars[text[i]].texture);
			glColor3fv(color);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.f, 0.f); glVertex2f(0, 0);
				glTexCoord2f(1.f, 0.f); glVertex2f(size * chars[text[i]].ratio, 0);
				glTexCoord2f(1.f, 1.f); glVertex2f(size * chars[text[i]].ratio, size);
				glTexCoord2f(0.f, 1.f); glVertex2f(0, size);
			}
			glEnd();
			glTranslatef(size * chars[text[i]].ratio, 0.0f, 0.0f);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

GLfloat GLText::GetWidth(std::string str, GLfloat size)
{
	GLfloat width = 0.0f;
	for (auto i = 0U; i < str.length(); i++) {
		width += size * chars[str[i]].ratio;
	}
	return width;
}

void GLText::Uninit() {
	for (auto i = 0; i < 256; i++) {
		glDeleteTextures(1, &(chars[i].texture));
	}
	delete[] chars;
}


/* GL TEXT STATIC HERE */
GLTextStatic::GLTextStatic(const char * text, const GLclampf * color, GLfloat x, GLfloat y, GLfloat size) {
	this->x = x;
	this->y = y;
	this->size = size;
	Reset(text, color);
}

void GLTextStatic::Reset(const char * text, const GLclampf * color) {
	ResetText(text);
	ResetColor(color);
}

void GLTextStatic::ResetText(const char * text) {
	txt = text;
}

void GLTextStatic::ResetColor(const GLclampf * color) {
	std::copy_n(color, 3, this->color);
}

void GLTextStatic::Draw() {
	glPushMatrix();
	{
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		for (auto i = 0U; i < txt.length(); i++) {
			glBindTexture(GL_TEXTURE_2D, chars[txt[i]].texture);
			glColor3fv(color);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.f, 1.f); glVertex2f(0, 0);
				glTexCoord2f(1.f, 1.f); glVertex2f(size * chars[txt[i]].ratio, 0);
				glTexCoord2f(1.f, 0.f); glVertex2f(size * chars[txt[i]].ratio, size);
				glTexCoord2f(0.f, 0.f); glVertex2f(0, size);
			}
			glEnd();
			glTranslatef(size * chars[txt[i]].ratio, 0.0f, 0.0f);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void GLTextStatic::DrawInverted() {
	glPushMatrix();
	{
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		for (auto i = 0U; i < txt.length(); i++) {
			glBindTexture(GL_TEXTURE_2D, chars[txt[i]].texture);
			glColor3fv(color);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.f, 0.f); glVertex2f(0, 0);
				glTexCoord2f(1.f, 0.f); glVertex2f(size * chars[txt[i]].ratio, 0);
				glTexCoord2f(1.f, 1.f); glVertex2f(size * chars[txt[i]].ratio, size);
				glTexCoord2f(0.f, 1.f); glVertex2f(0, size);
			}
			glEnd();
			glTranslatef(size * chars[txt[i]].ratio, 0.0f, 0.0f);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

std::string GLTextStatic::GetText()
{
	return txt;
}
