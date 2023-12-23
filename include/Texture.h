#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL2/SDL_image.h>
#include "common.h"

class Texture
{
	private:
		SDL_Texture* texture;

		void free();

	public:
		Texture();
		~Texture();

		bool loadFile(const std::string &path);
		void renderTexture(SDL_Rect* rect = NULL);
};

#endif