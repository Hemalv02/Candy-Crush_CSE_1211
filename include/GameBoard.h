#ifndef GAME_BOARD_H
#define GAME_BOARD_H
#include "Engine.h"

class GameBoard
{
    protected:
        Engine engine;

        const int nRows, nCols;

        vector<vector<int> > board;
        vector<vector<SDL_Rect> > square;

        vector<vector<bool> > removeFlagged;

        bool randomized;

        GameBoard(const int &nRows, const int &nCols);

        void renderStart();
        void renderEnd();

        void renderBoard();

        void startNotice();

        SDL_Rect timeBoard;
        Uint32 time;
        void renderTimer();

        SDL_Rect scoreBoard;
        SDL_Rect highscoreBoard;
        void renderScore();
        void renderHighScore();
        Sint32 score;
        void scoreCalculate();

        SDL_Rect continueSelect;
        SDL_Rect newGameSelect;
        SDL_Rect timeSelect;
        SDL_Rect soundSelect;

        SDL_Rect exit;

        void clear();

        void refill();
};

#endif