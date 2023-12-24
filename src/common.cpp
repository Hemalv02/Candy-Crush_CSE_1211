#include "common.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool gameover = false;
bool pressed = false;
bool forceQuit = false;
bool selected = false;
int selectChange = 1;

int gameMode = 0;
const std::string game_mode[] = {"Time", "Zen", "Endless"};
int timeMode = 1;
const int time_mode[] = {60, 120, 300};

Sint32* highscore = 0;