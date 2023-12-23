#ifndef SOUND_H
#define SOUND_H
#include <SDL2/SDL_mixer.h>
#include "common.h"

class Sound
{
    private:
        Mix_Music* music;
        Mix_Chunk* sfx;

        void free();

    public:
        Sound();
        ~Sound();

        bool loadMusic(const std::string &path);
        bool loadSFX(const std::string &path);

        void playMusic();
        void stopMusic();
        void playSFX();
};

#endif