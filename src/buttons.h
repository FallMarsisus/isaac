#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL2/SDL.h>
#include "text.h"

typedef struct button_s* button;

button createButton(SDL_Renderer* renderer, text texte, SDL_Color background, SDL_Color hoverColor, unsigned int id);
void displayButton(SDL_Renderer* renderer, button bouton, bool isHovering);
bool mouseInButton(button bouton, int mouseX, int mouseY);
text getButtonText(button bouton);
SDL_Color getButtonBackground(button bouton);
SDL_Color getButtonHoverColor(button bouton);
unsigned int getButtonId(button bouton);

#endif // BUTTONS_H