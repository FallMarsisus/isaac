#ifndef MOUSE_H
#define MOUSE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool mouse_in_rect(int x, int y, SDL_Rect rect);

bool mouse_in_rect_fix_drift(int x, int y, SDL_Rect* rect, int trueWidth, int renderWidth);

#endif