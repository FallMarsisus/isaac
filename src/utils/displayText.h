#pragma once

#include <SDL2/SDL.h>

typedef struct {
	SDL_Texture** calibri;
} fonts;

void load_fonts(SDL_Renderer* ren);
fonts* getFonts();
void free_fonts();
void displayText(const char* text, SDL_Renderer* ren, SDL_Texture** font, int x, int y, int fontSize);