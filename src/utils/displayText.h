#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	SDL_Texture** calibri;
	SDL_Texture** jetBainsMono;
} fonts;

void load_fonts(SDL_Renderer* ren);
fonts* get_fonts();
void free_fonts();

int get_text_width(const char* text, SDL_Texture** font, int fontSize);
int get_text_height(const char* text, SDL_Texture** font, int fontSize);

void display_text(const char* text, SDL_Renderer* ren, SDL_Texture** font, SDL_Color* color, int x, int y, int fontSize);