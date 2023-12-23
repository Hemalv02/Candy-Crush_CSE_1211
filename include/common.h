#ifndef COMMON_H
#define COMMON_H
#include <SDL2/SDL.h>
#include <string>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern bool gameover;

extern bool pressed, selected;

extern bool forceQuit;

extern int selectChange;

extern int gameMode;
const extern std::string game_mode[];

extern int timeMode;
const extern int time_mode[];

extern Sint32* highscore;

#endif