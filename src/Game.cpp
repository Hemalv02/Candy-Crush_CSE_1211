#include "Game.h"

Game::Game(const int& nRows, const int& nCols)
    : candy(nRows, nCols), nRows(nRows), nCols(nCols) {
  running = true;
  startGame();

  x = y = 0;
  run();
}

void Game::startGame() {
  while (SDL_WaitEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
      break;
    } else {
      candy.renderStart();
      if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        if (forceQuit) {
          if (SDL_PointInRect(&mousePos, &candy.continueSelect)) {
            selectChange = ContinueSelection;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
              start();
              break;
            }
          }
        }
        if (SDL_PointInRect(&mousePos, &candy.newGameSelect)) {
          selectChange = NewGameSelection;
          if (e.type == SDL_MOUSEBUTTONDOWN) {
            forceQuit = false;
            start();
            break;
          }
        } else if (SDL_PointInRect(&mousePos, &candy.timeSelect)) {
          selectChange = TimeSelection;
          if (e.type == SDL_MOUSEBUTTONDOWN)
            timeMode = (timeMode + 1) % Total_Time;
        } else if (SDL_PointInRect(&mousePos, &candy.soundSelect)) {
          if (e.type == SDL_MOUSEBUTTONDOWN) {
            candy.engine.musicState = !(candy.engine.musicState);
          }
        }
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_s:
          case SDLK_DOWN:
            selectChange = (selectChange + 1) % Total_Selection;
            break;

          case SDLK_w:
          case SDLK_UP:
            selectChange =
                (Total_Selection + (selectChange - 1)) % Total_Selection;
            break;

          case SDLK_RIGHT:
          case SDLK_d:
            if (selectChange == TimeSelection)
              timeMode = (timeMode + 1) % Total_Time;
            break;

          case SDLK_LEFT:
          case SDLK_a:
            if (selectChange == TimeSelection)
              timeMode = (Total_Time + (timeMode - 1)) % Total_Time;
            break;

          case SDLK_RETURN:
            start();
            break;
        }
      }
    }
  }
}

void Game::endGame() {
  if (candy.engine.musicState) {
    candy.engine.music.stopMusic();
  }
  if (forceQuit) {
    startGame();
    return;
  } else {
    candy.renderEnd();
    if (candy.engine.musicState) {
      candy.engine.endSFX.playSFX();
    }
  }
  while (SDL_WaitEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
      ;
      break;
    }
    if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.sym == SDLK_RETURN) {
        startGame();
        break;
      }
    }
  }
}

void Game::start() {
  gameover = false;
  highscore = &candy.engine.savedHighscore[Time][timeMode];

  if (candy.engine.musicState) {
    candy.engine.startSFX.playSFX();
  }
  candy.startNotice();
  if (candy.engine.musicState) {
    candy.engine.music.playMusic();
  }
  timerID = SDL_AddTimer(1000, callback, NULL);
  candy.randomize();
  candy.updateCandy();
}

void Game::run() {
  while (running && SDL_WaitEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
      forceQuit = true;
      candy.saveState();
    }
    if (gameover) {
      SDL_RemoveTimer(timerID);
      if (!candy.existMatch()) {
        SDL_Delay(400);
      }
      endGame();
    } else if (!candy.noMatch()) {
      candy.randomize();
    } else {
      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE)
          gameover = true;
        else if (!pressed) {
          pressed = true;
        } else
          keyControl();
        candy.renderSelector(selectedX, selectedY, x, y);
        candy.updateGame();
      }
      if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN ||
          e.type == SDL_MOUSEBUTTONUP) {
        pressed = true;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        if (e.type == SDL_MOUSEBUTTONDOWN &&
            SDL_PointInRect(&mousePos, &candy.exit)) {
          gameover = forceQuit = true;
          candy.saveState();
        }
        for (int x_ = 0; x_ < nRows; x_++) {
          for (int y_ = 0; y_ < nCols; y_++) {
            if (SDL_PointInRect(&mousePos, &candy.square[x_][y_])) {
              x = x_;
              y = y_;
              mouseControl();
              candy.renderSelector(selectedX, selectedY, x, y);
              candy.updateGame();
            }
          }
        }
      } else
        candy.renderSelector(selectedX, selectedY, x, y);
    }
  }
  SDL_RemoveTimer(timerID);
}

void Game::keyControl() {
  switch (e.key.keysym.sym) {
    case SDLK_UP:
    case SDLK_w:
      x--;
      if (selected) {
        y = selectedY;
        if (x < 0) x = selectedX;
        if (x != selectedX) x = selectedX - 1;
      } else
        x = (nRows + x) % nRows;
      break;

    case SDLK_DOWN:
    case SDLK_s:
      x++;
      if (selected) {
        y = selectedY;
        if (x < 0) x = selectedX;
        if (x != selectedX) x = selectedX + 1;
      } else
        x = x % nRows;
      break;

    case SDLK_LEFT:
    case SDLK_a:
      y--;
      if (selected) {
        x = selectedX;
        if (y < 0) y = selectedY;
        if (y != selectedY) y = selectedY - 1;
      } else
        y = (nCols + y) % nCols;
      break;

    case SDLK_RIGHT:
    case SDLK_d:
      y++;
      if (selected) {
        x = selectedX;
        if (y < 0) y = selectedY;
        if (y != selectedY) y = selectedY + 1;
      } else
        y = y % nCols;
      break;

    case SDLK_RETURN:
    case SDLK_SPACE:
      swapCandys();
      break;
  }
}

void Game::mouseControl() {
  switch (e.type) {
    case SDL_MOUSEMOTION:
      if (selected) {
        if (!swapCheck()) pressed = false;
        if (click)
          drag = true;
        else
          drag = false;
      }
      break;

    case SDL_MOUSEBUTTONDOWN:
      click = true;
      if (drag) {
        selectedX = x;
        selectedY = y;
        selected = true;
      } else
        swapCandys();
      break;

    case SDL_MOUSEBUTTONUP:
      if (drag) {
        swapCandys();
      } else
        click = false;
      drag = false;
      break;
  }
}

void Game::swapCandys() {
  if (!selected) {
    selectedX = x;
    selectedY = y;
    selected = true;
  } else {
    if (swapCheck()) {
      std::swap(candy.board[selectedX][selectedY], candy.board[x][y]);
      candy.updateCandy();
      if (!candy.existMatch()) {
        std::swap(candy.board[selectedX][selectedY], candy.board[x][y]);
        candy.updateCandy();
      }
      pressed = false;
    } else {
      x = selectedX;
      y = selectedY;
      pressed = true;
    }
    selected = false;
  }
}

bool Game::swapCheck() {
  if (x > selectedX + 1 || x < selectedX - 1 || y > selectedY + 1 ||
      y < selectedY - 1 || (x > selectedX && y > selectedY) ||
      (x < selectedX && y < selectedY) || (x > selectedX && y < selectedY) ||
      (x < selectedX && y > selectedY))
    return false;
  else
    return true;
}

Uint32 Game::callback(Uint32 interval, void* param) {
  SDL_Event event;
  SDL_UserEvent userevent;

  userevent.type = SDL_USEREVENT;
  userevent.code = 0;
  userevent.data1 = NULL;
  userevent.data2 = NULL;

  event.type = SDL_USEREVENT;
  event.user = userevent;

  SDL_PushEvent(&event);
  return (interval);
}