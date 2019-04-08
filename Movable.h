#pragma once
#include "Sprite.h"
class Movable :
	public Sprite
{
protected:
	double vx, vy; // velocity
	double ax, ay; // acceleration
	double friction; // constant direction-independent friction. NOT USED
public:
	Movable(SDL_Renderer *renderer, SDL_Texture *texture, string name, double x, double y, int r_x, int r_y, int r_w, int r_h, double friction, bool visible = true);
	~Movable();
	void Update(Uint32 dt);
	void Stop();
};



Movable::Movable(SDL_Renderer *renderer, SDL_Texture *texture, string name, double x, double y, int r_x, int r_y, int r_w, int r_h, double friction, bool visible = true) :
	Sprite(renderer, texture, name, x, y, r_x, r_y, r_w, r_h)
{
	vx = vy = ax = ay = 0;
	this->friction - friction;
}


Movable::~Movable()
{
}

inline void Movable::Update(Uint32 dt) {
	// move
	x += vx * dt;
	y += vy * dt;
	// change velocity
	vx += ax * dt;
	vy += ay * dt;
}

inline void Movable::Stop() {
	vx = vy = ax = ay = 0;
}
