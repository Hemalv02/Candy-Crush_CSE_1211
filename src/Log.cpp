#include "Log.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

void logSDL2(const std::string& msg) {
  std::cerr << msg << " Error: " << SDL_GetError() << '\n';
}

void logIMG(const std::string& msg) {
  std::cerr << msg << " Error: " << IMG_GetError() << '\n';
}

void logTTF(const std::string& msg) {
  std::cerr << msg << " Error: " << TTF_GetError() << '\n';
}

void logMixer(const std::string& msg) {
  std::cerr << msg << " Error: " << Mix_GetError() << '\n';
}

void logError(const std::string& msg) { std::cerr << msg << '\n'; }