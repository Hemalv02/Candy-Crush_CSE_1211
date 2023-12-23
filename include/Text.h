#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL_ttf.h>
#include "common.h"

class Text {
private:
    TTF_Font* font;
    SDL_Texture* texture;
    int width;
    int height;

    void free();

public:
    Text();
    bool openFont(int size);
    bool loadText(const std::string &text);
    void renderText(int x, int y, SDL_Rect* rect = NULL);
};

#endif
