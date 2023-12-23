#ifndef CANDY_H
#define CANDY_H
#include "GameBoard.h"

class Candy : private GameBoard
{
    private:
        friend class Game;

        Candy(const int &nRows, const int &nCols);

        void renderCandy();

        void updateCandy();

        void randomize();

        bool match3Horizontal(const int &row, const int &col);
        bool match3Vertical(const int &row, const int &col);
        
        bool existMatch();
        
        bool noMatch();
        int nX, nY, nX_, nY_;

        void renderSelector(const int &selectedX, const int &selectedY, const int &x, const int &y);

        void updateGame();

        void saveState();
};

#endif