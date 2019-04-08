/* I use stack of runnable objects, controlled by SDL userevents */

#define COUNT_FPS

#include <iostream>
#include <map>
#include <stack>

#include "Menus.h"
#include "ScreenSettings.h"
#include "CatGame.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

#if __cplusplus >= 199711
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

using namespace std;

SDL_Window *window;
SDL_GLContext glcontext;

// init libraries
void Init() {
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
		cout << "Init error" << endl;
		exit(1);
	}
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
}
// uninit libraries
void Uninit() {
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
// make window and renderer
void MakeWindow(int w, int h, bool full_scr) {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	if (full_scr)
		window = SDL_CreateWindow("The Cat Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	else
		window = SDL_CreateWindow("The Cat Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (window == NULL) {
		exit(1);
	}
	SDL_Surface* icon = IMG_Load("icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);
	glcontext = SDL_GL_CreateContext(window);

}
// init menus
void InitRunnables(map<string, Runnable*> &runnables, ScreenSettings *scr_sets) {
	runnables["main_menu"] = new MainMenu(scr_sets->size);
	runnables["quit_dialog"] = new QuitDialog(scr_sets->size);
	runnables["options_menu"] = new OptionsMenu(scr_sets->size);
	runnables["video_options_menu"] = new VideoOptionsMenu(scr_sets);
	/* 10.0f is default gamefield height. Width is calculated according to screen aspect */
	runnables["cat_game"] = new CatGame_master(scr_sets->aspect * 6.0f, 6.0f);
	runnables["game_over_menu"] = new GameOverMenu(scr_sets->size);
}
// uninit menus
void UninitRunnables(map<string, Runnable*> &runnables) {
	for (auto m : runnables)
		delete m.second;
	runnables.clear();
}

// prints FPS at left top corner of screen
void PrintFPS(ScreenSettings *screen_sets, string fps) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glLoadIdentity();
		glOrtho(0, screen_sets->w, screen_sets->h, 0, 1, -1);
		GLText::DrawGLTextInverted(10.0f, 10.0f, 25.0f, ("FPS: " + fps).c_str());
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

int main(int argc, char **arg) {
	Init();
	srand(SDL_GetTicks());
	ScreenSettings *screen_sets;
	/* FLAG TO GO ON RESTART LOOP */
	bool restart = true;
	/* THIS IS RESTART LOOP */
	while (restart) {
		/* LOAD SCREEN SETTINGS FROM FILE */
		screen_sets = new ScreenSettings("screen.sets");
		/* INIT WINDOW */
		MakeWindow(screen_sets->w, screen_sets->h, screen_sets->full_screen);
		/* INIT TEXT DRAWER */
		/* IMPORTANT - IT SHOULD BE INITED AFTER GL INIT */
		GLText::Init("rainydays.ttf");
		/* ENABLE NEEDED GL FUNCS */
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glShadeModel(GL_SMOOTH);

		glClearColor(0, 0, 0, 0);

		glViewport(0, 0, screen_sets->w, screen_sets->h);

		/* INIT RESOURCES */
		map<string, Runnable*> runnables;
		InitRunnables(runnables, screen_sets);

		/* SOME VARS TO MAKE MAINLOOP */
		/* run_stack IS A STACK OF RUNNABLE OBJECTS,
		TOP OF STACK RUNS EVERY FRAME*/
		stack<Runnable*> run_stack;
		/* FLAG TO GO ON MAIN LOOP */
		bool go_on = true;

		/* TO GET EVENTS */
		SDL_Event e;

		/* MOUSE STATE AND COORDS */
		Uint32 m_state;
		int m_x, m_y;
		/* KEYBOARD STATE */
		const Uint8 *kb_state;

		/* VARIABLE TO HADLE TIME */
		Uint32 currTime, prevTime;
		prevTime = SDL_GetTicks();

		/* FIRST OF ALL RUN MAIN MENU */
		run_stack.push(runnables["main_menu"]);
		/* SET MIN FRAME TIME ACCORDING TO CURRENT RUNNABLE */
		Uint32 minTime = run_stack.top()->GetFrameTime();
		/* SET CAMERA */
		run_stack.top()->SetCamera();

		/* THIS IS A MIAN LOOP */
		while (go_on)
		{
			/* GET CURRENT TIME */
			currTime = SDL_GetTicks();
			/* CHECK IF MIN FRAME TIME HAS PASSED, ELSE JUST WAIT A BIT */
			if (currTime - prevTime >= minTime) {
				/* GET MOUSE STATE - INFORMATION ABOUT PRESSED BUTTONS
				AND WINDOW-RELATIVE COORDS*/
				m_state = SDL_GetMouseState(&m_x, &m_y);
				/* GET KEYBOARD STATE - INFO ABOUT PRESSED KEYS*/
				kb_state = SDL_GetKeyboardState(NULL);
				/* SEND THIS DATA TO CURRENT RUNNABLE */
				run_stack.top()->SetState(m_x, m_y, m_state, kb_state, currTime - prevTime);
				/* handle events */
				while (SDL_PollEvent(&e)) {
					/* IF EVENT IS ABOUT RUNSTACK, HANDLE IT HERE,
					ELSE SEND IT TO CURRENT RUNNABLE */
					if (e.type == Helpers::RUN_STACK_EVENT) {
						/* IF WE SHOULD PUSH NEW RUNNABLE TO RUNSTACK */
						if (e.user.code == Helpers::RS_PUSH) {
							/* PUSH NEW RUNNABLE BY NAME, THAT IS HELD IN user.data1 AS A VOID POINTER,\
							SO CAST IT TO char POINTER*/
							run_stack.push(runnables[(char*)e.user.data1]);
							/* SEND SOME SPECIAL DATA */
							/* IT IS DEFINED FOR EACH RUNNABLE, SO IT IS JUST A VOID POINTER */
							run_stack.top()->SetData(e.user.data2);
							/* GET MIN FRAME TIME */
							minTime = run_stack.top()->GetFrameTime();
							/* SET CAMERA BY NEW RUNNABLE */
							run_stack.top()->SetCamera();
							/* SEND INPUT STATE TO CURRENT RUNNABLE */
							run_stack.top()->SetState(m_x, m_y, m_state, kb_state, currTime - prevTime);
							/* WAIT A LITTLE BECAUSE KEY/BUTTON, THAN WE PREESED TO CHANGE RUNNABLE, IS STILL PRESSED
							SO IT MAY BE HANDLED INSTANTLY BY NEW RUNNABLE */
							SDL_Delay(20);
							/* BREAK FROM EVENT LOOP */
							break;
						}
						/* IF WE HAVE TO QUIT, WE SHOULD BREAK MAIN LOOP AND NOT RESTART IT FROM RESTART LOOP */
						else if (e.user.code == Helpers::RS_QUIT) {
							/* THIS MEANS WE'LL BREAK MAIN LOOP WILL */
							go_on = false;
							/* THIS MEANS WE'LL ALSO BREAK RESTART (OUTER) LOOP */
							restart = false;
							break;
						}
						/* IF SCREEN SETTINGS ARE CHANGED, WE SHOULD ONLY BREAK MAIN LOOP,
						SO IT WILL BE RESTARTED FROM RESTART (OUTER) LOOP */
						else if (e.user.code == Helpers::RS_SCREEN_OPTIONS_CHANGED) {
							/* SAVE SCREEN SETTIGS TO FILE */
							screen_sets->SaveFile("screen.sets");
							/* THIS MEANS WI WILL BREAK MAIN LOOP */
							go_on = false;
							break;
						}
						else if (e.user.code == Helpers::RS_POP) {
							/* SET CURRENT RUNNABLE TO DEFAULTS TO DISCARD ANY UNSAVED CHANGES */
							run_stack.top()->Reset();
							/* POP IT FROM RUNSTACK */
							run_stack.pop();
							/* SET MAINLOOP FRAMETIME ACCORDING TO CURRENT RUNNABLE */
							minTime = run_stack.top()->GetFrameTime();
							run_stack.top()->SetCamera();
						}
						else if (e.user.code == Helpers::RS_POP_AND_RESET) {
							/* SET CURRENT RUNNABLE TO DEFAULTS TO DISCARD ANY UNSAVED CHANGES */
							run_stack.top()->Reset();
							/* POP IT FROM RUNSTACK */
							run_stack.pop();
							/* SET MAINLOOP FRAMETIME ACCORDING TO CURRENT RUNNABLE */
							minTime = run_stack.top()->GetFrameTime();
							run_stack.top()->SetCamera();
							/* SET CURRENT RUNNABLE TO DEFAULTS */
							run_stack.top()->Reset();
						}
						else if (e.user.code == Helpers::RS_GO_TO_MAIN_MENU) {
							/* RESET TO DEFAULTS AND POP 'TILL THERE IS MAIN MENU ONLY IN RS */
							while (run_stack.size() > 1) {
								run_stack.top()->Reset();
								run_stack.pop();
							}
						}
						/* CLEAR EVENT QUEUE, BECAUSE THERE MAY BE SOME INPUT EVENTS FOR PREVIOUS RUNNABLE,
						THAT CAN DO UNFORSEEN STUFF IN NEW RUNNABLE*/
						while (SDL_PollEvent(&e));

					}
					else
						/* CALL EVENT HANDLE METHOD OF CURRENT RUNNABLE */
						run_stack.top()->HandleEvent(&e);
				}
				/* UPDATE CURRENT RUNNABLE */
				run_stack.top()->Update();
				/* DRAW CURRENT RUNNABLE */
				run_stack.top()->OnDraw();
				/*  */
#ifdef COUNT_FPS
				/* DRAW FPS */
				PrintFPS(screen_sets, to_string(1000/(currTime - prevTime)));
#endif // COUNT_FPS
				/* SWAP GL BUFFERS */
				SDL_GL_SwapWindow(window);

				prevTime = currTime;
			}
			else {
				/* DELAY IF RUNS TOO FAST */
				SDL_Delay(1);
			}
		}
		/* UNINIT EVERYTHING */
		UninitRunnables(runnables);
		GLText::Uninit();
		SDL_GL_DeleteContext(glcontext);
		SDL_DestroyWindow(window);
		delete screen_sets;
	}
	Uninit();
	return 0;
}