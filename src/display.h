#pragma once
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* loadBitMap(const char* fileName, SDL_Renderer* renderer);

void drawBitmap(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int sizeX, int sizeY);
void drawText(SDL_Renderer *renderer, char* text, int x, int y, int sizeX, int sizeY, TTF_Font* font, int red, int green, int blue);
void drawBox(SDL_Renderer* renderer, int x, int y, int sizeX, int sizeY, int red, int green, int blue);
void newFrame(SDL_Renderer* renderer, int red, int green, int blue);