#include "Sound.h"

#include "Log.h"

Sound::Sound() {
  music = NULL;
  sfx = NULL;
}

Sound::~Sound() { free(); }

bool Sound::loadMusic(const std::string& path) {
  free();

  music = Mix_LoadMUS(path.c_str());
  if (music == NULL) {
    logMixer("Mix_LoadMUS");
    return false;
  } else
    return true;
}

bool Sound::loadSFX(const std::string& path) {
  free();

  sfx = Mix_LoadWAV(path.c_str());
  if (sfx == NULL) {
    logMixer("Mix_LoadWAV");
    return false;
  } else
    return true;
}

void Sound::playMusic() {
  if (!Mix_PlayingMusic()) {
    Mix_PlayMusic(music, -1);
  } else {
    if (Mix_PausedMusic()) {
      Mix_ResumeMusic();
    } else {
      Mix_PauseMusic();
    }
  }
}

void Sound::stopMusic() { Mix_HaltMusic(); }

void Sound::playSFX() { Mix_PlayChannel(-1, sfx, 0); }

void Sound::free() {
  if (music != NULL) {
    Mix_FreeMusic(music);
    music = NULL;
  }
  if (sfx != NULL) {
    Mix_FreeChunk(sfx);
    sfx = NULL;
  }
}