#pragma once
#include <map>
#include <vector>
#include <SDL.h>

using namespace std;

class KeyMap
{
public:
	/*map<Uint16, string> key_names;
	vector<string> action_names;*/

	int KM_PAUSE,
		KM_FIRST_LEFT,
		KM_FIRST_RIGHT,
		KM_FIRST_JUMP,
		KM_SECOND_LEFT,
		KM_SECOND_RIGHT,
		KM_SECOND_JUMP;


	KeyMap() {
		SetDefault();
	}
	~KeyMap() {

	};
	void SetDefault() {
		KM_PAUSE = SDLK_p;
		KM_FIRST_LEFT = SDLK_a;
		KM_FIRST_RIGHT = SDLK_d;
		KM_FIRST_JUMP = SDLK_w;
		KM_SECOND_LEFT = SDLK_LEFT;
		KM_SECOND_RIGHT = SDLK_RIGHT;
		KM_SECOND_JUMP = SDLK_UP;
	}
};
