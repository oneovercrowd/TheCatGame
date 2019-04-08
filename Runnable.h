#pragma once
#include <SDL.h>
#include <Windows.h>
#include "Helpers.h"
#include <SDL_opengl.h>

using namespace std;

class Runnable
/*
an abstract class of runnable objects
has basic abstract methods for rendering and event hadling
has its onw loop parameters such as min per_frame time
*/
{
protected:
	void *data;
	int ms_per_frame; // min frame time in miliseconds
	int m_x, m_y; // mouse position
	Uint32 mstate; // mouse state
	const Uint8 *kbstate; // keyboard state
	Uint32 dt; // time difference
public:
	virtual void SetCamera() = 0;  // set opengl camera
	virtual void OnDraw() = 0; // will be called from mainloop to draw
	virtual void HandleEvent(SDL_Event *e) = 0; // will be called from mainloop to handle event
	int GetFrameTime() { // returns min frame time (to control mainloop)
		return ms_per_frame;
	}
	Runnable(int ms_per_frame) {
		this->ms_per_frame = ms_per_frame;
	}
	virtual ~Runnable() {
	}
	virtual void SetState(int m_x, int m_y, Uint32 mstate, const Uint8 *kbstate, Uint32 dt) {
		this->m_x = m_x;
		this->m_y = m_y;
		this->mstate = mstate;
		this->kbstate = kbstate;
		this->dt = dt;
	}
	virtual void Reset() = 0;
	virtual void Update() = 0;
	virtual void SetData(void *new_data) {
		data = new_data;
	}
};

