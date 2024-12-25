#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct text_s* text;

// creation functions
text createExmptyText();
text createText(SDL_Renderer* renderer, int x, int y, int red, int green, int blue, char* string, char* fontName, int fontSize);

// modification functions
void changeTextMessage(text texte, char* newString);
void sizeTextCoord(text texte, int x, int y);
void resizeText(SDL_Renderer* renderer, text texte, int newSize);
void changeTextFontSize(text texte, int newSize);
void forceTextNewWidth(text texte, int newSizeX);
void changeTextColor(SDL_Renderer* renderer, text texte, SDL_Color newColor);
void changeTextUnderline(text texte, bool underlining);

// display functions
void displayText(SDL_Renderer* renderer, text texte);

// getter functions
int getTextX(text texte);
int getTextY(text texte);
int getTextSizeX(text texte);
int getTextSizeY(text texte);
int getTextFontSize(text texte);
char* getTextString(text texte);
char* getTextFontName(text texte);
SDL_Color getTextColor(text texte);
bool messageIsNull(text texte);
bool getTextUnderlineState(text texte);