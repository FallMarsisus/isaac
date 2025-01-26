#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

int ipow(int base, int exp);
bool checkCollision(SDL_Rect* hitbox1, SDL_Rect* hitbox2);

void draw_rect(SDL_Rect* rect, SDL_Renderer* ren, int r, int g, int b, int a);

int random_int(int a, int b);
float random_float(float a, float b);
