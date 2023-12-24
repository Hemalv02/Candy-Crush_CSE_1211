#include "Candy.h"

Candy::Candy(const int& nRows, const int& nCols) : GameBoard(nRows, nCols) {
  selected = false;
  pressed = false;
}

void Candy::randomize() {
  if (forceQuit && selectChange == ContinueSelection) {
    score = engine.savedScore;
    board = engine.savedBoard;
    time = engine.savedTime;
    forceQuit = false;
  } else {
    randomized = true;
    for (int i = 0; i < nRows; i++) {
      for (int j = 0; j < nCols; j++) {
        board[i][j] = engine.getRandom();
      }
    }
    while (existMatch()) {
      clear();
      refill();
    }
    randomized = false;
  }
}

void Candy::renderCandy() {
  renderBoard();
  for (int x = 0; x < nRows; x++) {
    for (int y = 0; y < nCols; y++) {
      int COLOR = board[x][y];
      engine.candyTexture[COLOR].renderTexture(&square[x][y]);
    }
  }
}

void Candy::updateCandy() {
  renderCandy();
  engine.render();
  SDL_Delay(400);
}

bool Candy::match3Horizontal(const int& row, const int& col) {
  const int start = board[row][col];
  for (int i = 1; i < 3; i++) {
    if (board[row][col + i] != start) {
      return false;
    }
  }

  for (int i = 0; i < 3; i++) {
    removeFlagged[row][col + i] = true;
  }
  return true;
}

bool Candy::match3Vertical(const int& row, const int& col) {
  const int start = board[row][col];
  for (int i = 1; i < 3; i++) {
    if (board[row + i][col] != start) {
      return false;
    }
  }

  // Mark candys to be removed
  for (int i = 0; i < 3; i++) {
    removeFlagged[row + i][col] = true;
  }
  return true;
}

bool Candy::existMatch() {
  bool exist = false;

  // Horizontal match
  for (int row = 0; row < nRows; row++) {
    for (int col = 0; col < nCols - 2; col++) {
      if (match3Horizontal(row, col)) {
        exist = true;
      }
    }
  }

  // Vertical match
  for (int row = 0; row < nRows - 2; row++) {
    for (int col = 0; col < nCols; col++) {
      if (match3Vertical(row, col)) {
        exist = true;
      }
    }
  }
  return exist;
}

bool Candy::noMatch() {
  vector<vector<int> > tempBoard = board;
  vector<vector<bool> > tempPending = removeFlagged;
  for (int x = 0; x < nRows - 1; x++) {
    for (int y = 0; y < nCols - 1; y++) {
      // Horizontal check
      std::swap(board[x][y], board[x + 1][y]);
      if (existMatch()) {
        nX = x;
        nY = y;
        nX_ = x + 1;
        nY_ = y;
        board = tempBoard;
        removeFlagged = tempPending;
        return true;
      } else
        board = tempBoard;

      // Vertical check
      std::swap(board[x][y], board[x][y + 1]);
      if (existMatch()) {
        nX = x;
        nY = y;
        nX_ = x;
        nY_ = y + 1;
        board = tempBoard;
        removeFlagged = tempPending;
        return true;
      } else
        board = tempBoard;
    }
  }
  return false;
}

void Candy::renderSelector(const int& selectedX, const int& selectedY,
                           const int& x, const int& y) {
  renderCandy();
  if (selected) {
    engine.selectorTexture.renderTexture(&square[selectedX][selectedY]);
  }
  if (pressed) {
    engine.selectorTexture.renderTexture(&square[x][y]);
  }
  engine.render();
}

void Candy::updateGame() {
  int count = 0;
  while (existMatch()) {
    if (engine.musicState) {
      count++;
      if (count == 1) {
        engine.matchSFX[0].playSFX();
      } else if (count == 2) {
        engine.matchSFX[1].playSFX();
      } else
        engine.matchSFX[2].playSFX();
    }
    clear();
    updateCandy();
    refill();
    updateCandy();
  }
}

void Candy::saveState() {
  engine.savedScore = score;
  engine.savedTime = engine.timer.time;
  engine.savedBoard = board;
}