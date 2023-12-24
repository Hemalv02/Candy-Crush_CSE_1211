#include "GameBoard.h"

GameBoard::GameBoard(const int &nRows, const int &nCols)
    : nRows(nRows), nCols(nCols) {
  score = 0;

  board.resize(nRows, vector<int>(nCols));

  square.resize(nRows, vector<SDL_Rect>(nCols));
  int startX = 240, startY = 35;
  for (int row = 0; row < nRows; row++) {
    for (int col = 0; col < nCols; col++) {
      square[row][col] =
          (SDL_Rect){col * 65 + startX, row * 65 + startY, 65, 65};
    }
  }

  removeFlagged.resize(nRows, vector<bool>(nCols));

  scoreBoard = (SDL_Rect){15, 200, 192, 43};
  highscoreBoard = (SDL_Rect){15, 100, 192, 43};
  timeBoard = (SDL_Rect){15, 400, 192, 71};
  exit = (SDL_Rect){10, 500, 194, 43};

  continueSelect = (SDL_Rect){325, 400, 160, 35};
  newGameSelect = (SDL_Rect){325, 440, 160, 35};
  timeSelect = (SDL_Rect){325, 480, 160, 35};
  soundSelect = (SDL_Rect){325, 520, 160, 35};
}

void GameBoard::scoreCalculate() {
  int count = 0;
  for (int row = 0; row < nRows; row++) {
    for (int col = 0; col < nCols; col++) {
      if (removeFlagged[row][col]) count++;
    }
  }
  score += (count / 3) * 750 + (count % 3) * (engine.getRandom() * 10 + 300);
}

void GameBoard::clear() {
  if (!randomized) scoreCalculate();
  *highscore = score > *highscore ? score : *highscore;

  for (int row = 0; row < nRows; row++) {
    for (int col = 0; col < nCols; col++) {
      if (removeFlagged[row][col]) {
        board[row][col] = Destroyed;
        removeFlagged[row][col] = false;
      }
    }
  }
}

void GameBoard::refill() {
  for (int col = 0; col < nCols; col++) {
    for (int row = nRows - 1; row >= 0; row--) {
      for (int _row = row - 1; _row >= 0; _row--) {
        if (board[row][col] == Destroyed && board[_row][col] != 0) {
          std::swap(board[row][col], board[_row][col]);
        }
      }
    }
  }

  for (int row = 0; row < nRows; row++) {
    for (int col = 0; col < nCols; col++) {
      if (board[row][col] == Destroyed) {
        board[row][col] = engine.getRandom();
      }
    }
  }
}

void GameBoard::renderStart() {
  engine.timer.stop();
  score = 0;
  engine.startTexture.renderTexture();

  if (forceQuit) {
    engine.buttonTexture.renderTexture(&continueSelect);
    engine.continueText.renderText(-1, -1, &continueSelect);
  }

  engine.buttonTexture.renderTexture(&newGameSelect);
  engine.newGameText.renderText(-1, -1, &newGameSelect);

  time = time_mode[timeMode];
  if (time <= 120 || time % 60 != 0)
    engine.timeModeText.loadText(std::to_string(time) + " seconds");
  else
    engine.timeModeText.loadText(std::to_string(time / 60) + " minutes");

  engine.buttonTexture.renderTexture(&timeSelect);
  engine.timeModeText.renderText(-1, -1, &timeSelect);

  engine.buttonTexture.renderTexture(&soundSelect);
  if (engine.musicState) {
    engine.soundText.loadText("Sound: ON");
  } else {
    engine.soundText.loadText("Sound: OFF");
  }
  engine.soundText.renderText(-1, -1, &soundSelect);
  engine.render();
}

void GameBoard::renderEnd() {
  engine.endTexture.renderTexture();
  engine.scores.renderText(425, 340);
  engine.render();
  score = 0;
  time = time_mode[timeMode];
}

void GameBoard::renderBoard() {
  engine.boardTexture.renderTexture();
  engine.exitTexture.renderTexture(&exit);
  engine.exitText.renderText(-1, -1, &exit);
  renderScore();
  renderHighScore();
  if (gameMode == Time) renderTimer();
}

void GameBoard::renderScore() {
  engine.scoreText.renderText(70, 170);
  engine.scores.loadText(std::to_string(score));
  engine.scores.renderText(25, -1, &scoreBoard);
}

void GameBoard::renderHighScore() {
  engine.highscoreText.renderText(50, 70);
  engine.highscores.loadText(std::to_string(*highscore));
  engine.highscores.renderText(25, -1, &highscoreBoard);
}

void GameBoard::renderTimer() {
  std::string minutes, seconds;
  if (gameover) {
    engine.timer.stop();
  } else if (!engine.timer.countdown(time * 1000)) {
    gameover = true;
  }
  minutes = std::to_string(engine.timer.time / 60);
  seconds = std::to_string(engine.timer.time % 60);

  if (std::stoi(minutes) < 10) {
    minutes = "0" + minutes;
  }
  if (std::stoi(seconds) < 10) {
    seconds = "0" + seconds;
  }

  engine.times.loadText(minutes + ":" + seconds);
  engine.times.renderText(-1, -1, &timeBoard);
}

void GameBoard::startNotice() {
  SDL_Delay(1000);
  renderBoard();
  engine.startNotice.renderText(-1, -1);
  engine.render();
  SDL_Delay(700);
}