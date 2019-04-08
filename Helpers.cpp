#include "Helpers.h"

/* CHECKS IF TWO RECTS COLLIDE */
bool Helpers::CollideRR(SDL_Rect &rect1, SDL_Rect &rect2) {
	if (rect1.x + rect1.w <= rect2.x || rect2.x + rect2.w <= rect1.x)
		return false;
	if (rect1.y + rect1.h <= rect2.y || rect2.y + rect2.h <= rect1.y)
		return false;
	return true;
}

/* CHECKS IF TWO RECTS COLLIDE */
bool Helpers::CollideRR(FRect &rect1, FRect &rect2)
{
	if (rect1.x + rect1.w <= rect2.x || rect2.x + rect2.w <= rect1.x)
		return false;
	if (rect1.y + rect1.h <= rect2.y || rect2.y + rect2.h <= rect1.y)
		return false;
	return true;
}

/* CHECKS IF TWO CIRCLES COLLIDE */
bool Helpers::CollideCC(double x1, double y1, double r1, double x2, double y2, double r2) {
	return hypot(x1 - x2, y1 - y2) < (r1 + r2);
}
/* CHECKS IF RECT COLLIDES WITH CIRCLE */
bool Helpers::CollideRC(SDL_Rect &rect, double x, double y, double r) {
	// check if off
	double rect_r = rect.x + rect.w,
		rect_b = rect.y + rect.h;
	if (rect_r <= x - r || x + r <= rect.x)
		return false;
	if (rect_b <= y - r || y + r <= rect.y)
		return false;
	// else check if corner is off
	double dxr = x - rect_r,
		dxl = x - rect.x,
		dyb = y - rect_b,
		dyt = y - rect.y;
	if (dxr > 0)
		if (dyb > 0) {
			// left bottom
			if (hypot(dxr, dyb) >= r)
				return false;
		}
		else if (dyt < 0) {
			// left top
			if (hypot(dxr, dyt) >= r)
				return false;
		}
		else if (dxl < 0)
			if (dyb > 0) {
				// right bottom
				if (hypot(dxl, dyb) >= r)
					return false;
			}
			else if (dyt < 0) {
				// right top
				if (hypot(dxl, dyt) >= r)
					return false;
			}
	return true;
}
/* CHECKS IF RECT COLLIDES WITH CIRCLE */
bool Helpers::CollideRC(FRect &rect, double x, double y, double r)
{
	/* CHECK IF CIRCLE IS OFF RECT EITHER BY X OR Y */
	double rect_r = rect.x + rect.w,
		rect_b = rect.y + rect.h;
	if (rect_r <= x - r || x + r <= rect.x)
		return false;
	if (rect_b <= y - r || y + r <= rect.y)
		return false;
	/* IF NOT - CHECK IF NOT COLLIDES IN CORNER */
	double dxr = x - rect_r,
		dxl = x - rect.x,
		dyb = y - rect_b,
		dyt = y - rect.y;
	if (dxr > 0)
		if (dyb > 0) {
			// left bottom
			if (hypot(dxr, dyb) >= r)
				return false;
		}
		else if (dyt < 0) {
			// left top
			if (hypot(dxr, dyt) >= r)
				return false;
		}
		else if (dxl < 0)
			if (dyb > 0) {
				// right bottom
				if (hypot(dxl, dyb) >= r)
					return false;
			}
			else if (dyt < 0) {
				// right top
				if (hypot(dxl, dyt) >= r)
					return false;
			}
	return true;
}

SDL_Texture *Helpers::LoadTexture(SDL_Renderer *renderer, std::string name) {
	SDL_Surface *sur = IMG_Load(name.c_str());
	if (!sur) {
		printf("IMG_Load: %s\n", IMG_GetError());
	}
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
	if (!tex) {
		printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
	}
	SDL_FreeSurface(sur);
	return tex;
}

GLuint Helpers::LoadGLTexture(std::string name)
{
	GLuint TextureID = 0;
	SDL_Surface *sur = IMG_Load(name.c_str());
	if (!sur) {
		printf("IMG_Load: %s\n", IMG_GetError());
		return 0;
	}
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	int Mode = GL_RGB;

	if (sur->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, sur->w, sur->h, 0, Mode, GL_UNSIGNED_BYTE, sur->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	SDL_FreeSurface(sur);
	return TextureID;
}

GLuint Helpers::GLTextureFromSurface(SDL_Surface *sur)
{
	GLuint TextureID = 0;
	if (!sur) {
		printf("IMG_Load: %s\n", IMG_GetError());
		return 0;
	}
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	if (TextureID == 0) {
		glGetError();
	}
	int Mode = GL_RGB;

	if (sur->format->BitsPerPixel == 32) {
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, sur->w, sur->h, 0, Mode, GL_UNSIGNED_BYTE, sur->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return TextureID;
}

GLuint Helpers::GLTextureText(GLfloat w, GLfloat h, const std::string text, GLclampf * color, const std::string fontname, int ptsize)
{
	// make temp SDL_Suface with text and make an opengl's texture from it
	SDL_Surface *t_text, *t_box;
	// init label surface
	t_box = SDL_CreateRGBSurfaceWithFormat(0, (int)w, (int)h, 32, SDL_PIXELFORMAT_RGBA32);
	if (!t_box) {
		printf("SDL_CreateRGBSurfaceWithFormat: %s\n", SDL_GetError());
	}
	SDL_Rect temp_rect;
	// load font
	TTF_Font *font = TTF_OpenFont(fontname.c_str(), ptsize);
	if (!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	t_text = TTF_RenderText_Blended(font, text.c_str(), Helpers::Color(int(255 * color[0]), int(255 * color[1]), int(255 * color[2]), 1));
	SDL_FillRect(t_box, NULL, 0);
	temp_rect.w = t_text->w;
	temp_rect.h = t_text->h;
	temp_rect.x = 0;
	// calc position
	temp_rect.y = int((h - temp_rect.h) / 2);
	// blit text to label
	SDL_BlitSurface(t_text, NULL, t_box, &temp_rect);
	GLuint texture = Helpers::GLTextureFromSurface(t_box);
	// free temp resources
	TTF_CloseFont(font);
	SDL_FreeSurface(t_text);
	SDL_FreeSurface(t_box);
	return texture;
}

double Helpers::Degree(double rad) {
	return rad / M_PI * 180;
}
SDL_Color Helpers::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Color const c = { r, g, b, a };
	return c;
}
Uint32 Helpers::callback1(Uint32 interval, void * param){
	SDL_Event e;
	e.type = SDL_USEREVENT;
	e.user.code = Helpers::WEAP_CD;
	e.user.data1 = param;
	SDL_PushEvent(&e);
	return 0;
}

void Helpers::PushUserEvent(Uint32 event_type, Sint32 user_type, void* data1, void* data2) {
	SDL_Event e;
	e.type = event_type;
	e.user.code = user_type;
	e.user.data1 = data1;
	e.user.data2 = data2;
	SDL_PushEvent(&e);
	return;
}