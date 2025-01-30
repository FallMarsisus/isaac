#ifndef MOUSE_H
#define MOUSE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool mouseInRect(int x, int y, SDL_Rect rect);

#endif