#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

int ipow(int base, int exp);
bool checkCollision(SDL_Rect* hitbox1, SDL_Rect* hitbox2);