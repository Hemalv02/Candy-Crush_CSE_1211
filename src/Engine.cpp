#include "Engine.h"

#include <random>

#include "Log.h"

Engine::Engine() : WINDOW_WIDTH(800), WINDOW_HEIGHT(600), TITLE("Candy Crush") {
  savedBoard.resize(8, vector<int>(8));
  success = true;
  if (!initSDL2()) {
    logError("Unable to initialize SDL2 Engine!");
    exit();
  } else if (!textureInit()) {
    logError("Unable to load Textures!");
    exit();
  } else if (!fontInit()) {
    logError("Unable to load Fonts!");
    exit();
  } else if (!soundInit()) {
    logError("Unable to load Sounds!");
    exit();
  } else
    initSave();
}

Engine::~Engine() {
  if (!save()) {
    logError("Your data will not be saved!");
  }
  exit();
}

bool Engine::initSDL2() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
    logSDL2("SDL_Init");
    success = false;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    logIMG("IMG_Init");
    success = false;
  }

  if (TTF_Init() == -1) {
    logTTF("TTF_Init");
    success = false;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    logMixer("Mixer_OpenAudio");
    success = false;
  }

  window = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_SHOWN);
  if (window == NULL) {
    logSDL2("CreateWindow");
    success = false;
  }

  SDL_Surface* icon = IMG_Load("assets/gemBlue.png");
  if (icon == NULL) {
    logIMG("IMG_Load");
    success = false;
  }
  SDL_SetWindowIcon(window, icon);
  SDL_FreeSurface(icon);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    logSDL2("CreateRenderer");
    success = false;
  }

  SDL_Surface* cursorSurface = IMG_Load("assets/cursor.png");
  if (cursorSurface == NULL) {
    logIMG("IMG_Load");
    success = false;
  }
  cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
  if (cursor == NULL) {
    logSDL2("CreateColorCursor");
    success = false;
  }
  SDL_FreeSurface(cursorSurface);
  SDL_SetCursor(cursor);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    logError("Texture filtering not enabled!");
  }

  return success;
}

bool Engine::textureInit() {
  if (boardTexture.loadFile("assets/Background.png") &&
      candyTexture[Red].loadFile("assets/gemRed.png") &&
      candyTexture[Green].loadFile("assets/gemGreen.png") &&
      candyTexture[Blue].loadFile("assets/gemBlue.png") &&
      candyTexture[Orange].loadFile("assets/gemOrange.png") &&
      candyTexture[Yellow].loadFile("assets/gemYellow.png") &&
      candyTexture[Purple].loadFile("assets/gemPurple.png") &&
      candyTexture[White].loadFile("assets/gemWhite.png") &&
      selectorTexture.loadFile("assets/selector.png") &&
      hintTexture.loadFile("assets/hint.png") &&
      buttonTexture.loadFile("assets/button.png") &&
      highlightTexture.loadFile("assets/highlight.png") &&
      exitTexture.loadFile("assets/exit.png") &&
      startTexture.loadFile("assets/startBackground.png") &&
      endTexture.loadFile("assets/endBackground.png"))
    return true;

  else
    return false;
}

bool Engine::fontInit() {
  if (!continueText.openFont(30) || !newGameText.openFont(30) ||
      !gameModeText.openFont(25) || !timeModeText.openFont(25) ||
      !scoreText.openFont(30) || !highscoreText.openFont(30) ||
      !scores.openFont(35) || !highscores.openFont(35) || !times.openFont(75) ||
      !startNotice.openFont(100) || !soundText.openFont(30) ||
      !exitText.openFont(35))
    return false;

  else if (!continueText.loadText("Continue") ||
           !newGameText.loadText("New Game") || !scoreText.loadText("Score") ||
           !highscoreText.loadText("High Score") ||
           !startNotice.loadText("START") || !exitText.loadText("EXIT"))
    return false;

  else
    return true;
}

bool Engine::soundInit() {
  if (!music.loadMusic("assets/music.ogg") ||
      !matchSFX[0].loadSFX("assets/match1.ogg") ||
      !matchSFX[1].loadSFX("assets/match2.ogg") ||
      !matchSFX[2].loadSFX("assets/match3.ogg") ||
      !startSFX.loadSFX("assets/gamestart.ogg") ||
      !endSFX.loadSFX("assets/gameover.ogg"))
    return false;

  else
    return true;
}

void Engine::initSave() {
  SDL_RWops* save = SDL_RWFromFile("save.bin", "r+b");
  if (save == NULL) {
    logError("Warning: Unable to open save file!");
    for (int i = 0; i < Total_Mode; i++) {
      for (int j = 0; j < Total_Time; j++) {
        savedHighscore[i][j] = 0;
      }
    }
  } else {
    for (int i = 0; i < Total_Mode; i++) {
      for (int j = 0; j < Total_Time; j++) {
        SDL_RWread(save, &savedHighscore[i][j], sizeof(Sint32), 1);
      }
    }
    SDL_RWread(save, &forceQuit, sizeof(bool), 1);
    if (forceQuit) {
      SDL_RWread(save, &savedTime, sizeof(Uint32), 1);
      SDL_RWread(save, &savedScore, sizeof(Sint32), 1);
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          SDL_RWread(save, &savedBoard[i][j], sizeof(int), 1);
        }
      }
    }

    SDL_RWclose(save);
  }
}

bool Engine::save() {
  SDL_RWops* save = SDL_RWFromFile("save.bin", "r+b");
  if (save == NULL) {
    save = SDL_RWFromFile("save.bin", "w+b");
  }
  if (save != NULL) {
    for (int i = 0; i < Total_Mode; i++) {
      for (int j = 0; j < Total_Time; j++) {
        SDL_RWwrite(save, &savedHighscore[i][j], sizeof(Sint32), 1);
      }
    }
    SDL_RWwrite(save, &forceQuit, sizeof(bool), 1);
    if (forceQuit) {
      SDL_RWwrite(save, &savedTime, sizeof(Uint32), 1);
      SDL_RWwrite(save, &savedScore, sizeof(Sint32), 1);
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          SDL_RWwrite(save, &savedBoard[i][j], sizeof(int), 1);
        }
      }
    }

    SDL_RWclose(save);
  } else {
    logSDL2("Save");
    return false;
  }
  return true;
}

void Engine::exit() {
  SDL_FreeCursor(cursor);
  cursor = NULL;
  SDL_DestroyRenderer(renderer);
  renderer = NULL;
  SDL_DestroyWindow(window);
  window = NULL;
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

int Engine::getRandom() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, Total - 1);
  return dist(rng);
}

void Engine::render() { SDL_RenderPresent(renderer); }