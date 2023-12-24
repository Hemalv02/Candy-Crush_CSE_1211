#include "Texture.h"

#include "Log.h"

Texture::Texture() : texture(nullptr) {}

Texture::~Texture() { free(); }

bool Texture::loadFile(const std::string& path) {
  free();

  SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
  if (newTexture == nullptr) {
    logIMG("IMG_Load");
  }

  texture = newTexture;
  return texture != nullptr;
}

void Texture::renderTexture(SDL_Rect* rect) {
  SDL_RenderCopy(renderer, texture, nullptr, rect);
}

void Texture::free() {
  if (texture != nullptr) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
}