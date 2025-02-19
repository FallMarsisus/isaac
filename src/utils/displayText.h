#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
	SDL_Texture** calibri;
} fonts;

void load_fonts(SDL_Renderer* ren);
fonts* get_fonts();
void free_fonts();
void display_text(const char* text, SDL_Renderer* ren, SDL_Texture** font, SDL_Color* color, int x, int y, int fontSize);