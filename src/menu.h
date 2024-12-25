#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "buttons.h"

button* createMainMenuButtons(SDL_Renderer* renderer, int winWidth, int winHeigth);
void displayMainMenu(SDL_Renderer* renderer, button* boutons);

#endif // MENU_H