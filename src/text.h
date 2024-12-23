#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct text_s* text;

text createExmptyText();
text createText(SDL_Renderer* renderer, int x, int y, int red, int green, int blue, char* string, char* fontName, int fontSize);
void sizeTextCoordAndSize(text texte, int x, int y, int sizeX, int sizeY);
void changeTextMessage(text texte, char* newString);
void resizeText(SDL_Renderer* renderer, text texte, int newSize);
void displayText(SDL_Renderer* renderer, text texte);
bool messageIsNull(text texte);
void changeTextFontSize(text texte, int newSize);
void forceTextNewWidth(text texte, int newSizeX);