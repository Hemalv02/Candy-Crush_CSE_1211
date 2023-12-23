#ifndef GAME_H
#define GAME_H

#include "Candy.h"

class Game {
private:
    SDL_Event e;
    SDL_TimerID timerID;
    SDL_Point mousePos;
    Candy candy;
    const int nRows, nCols;
    int x, y;
    int selectedX, selectedY;

    void keyControl();
    void mouseControl();
    bool click;
    bool drag;

    bool swapCheck();
    void swapCandys();
    void run();
    void startGame();
    void endGame();
    void start();
    bool running;

    static Uint32 callback(Uint32 interval, void* param);

public:
    Game(const int &nRows, const int &nCols);
};

#endif
