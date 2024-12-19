#pragma once
#include <SDL2/SDL_image.h>

SDL_Texture* loadBitMap(const char* fileName, SDL_Renderer* renderer);

void displayBitmap(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int sizeX, int sizeY);