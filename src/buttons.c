#include <stdbool.h>
#include <stdlib.h>
#include "text.h"
#include "display.h"

typedef struct button_s{
	text texte;
	SDL_Color backgroud;	
	SDL_Color hoverColor;
	unsigned int id;
}* button;

button createButton(SDL_Renderer* renderer, text texte, SDL_Color background, SDL_Color hoverColor, unsigned int id) {
	button bouton = malloc(sizeof(struct button_s));

	bouton->texte = texte;
	bouton->backgroud = background;
	bouton->hoverColor = hoverColor;
	bouton->id = id;

	return bouton;
}


void displayButton(SDL_Renderer* renderer, button bouton, bool isHovering) {
	int x = getTextX(bouton->texte);
	int y = getTextY(bouton->texte);
	int sizeX = getTextSizeX(bouton->texte);
	int sizeY = getTextSizeY(bouton->texte);
	SDL_Rect rect = {x-1, y-1, sizeX+2, sizeY+2};

	SDL_Color color = isHovering ? bouton->hoverColor : bouton->backgroud;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);

	displayText(renderer, bouton->texte);
}


bool mouseInButton(button bouton, int mouseX, int mouseY) {
	int x = getTextX(bouton->texte);
	int y = getTextY(bouton->texte);
	int sizeX = getTextSizeX(bouton->texte);
	int sizeY = getTextSizeY(bouton->texte);

	return x <= mouseX && mouseX < x + sizeX && y <= mouseY && mouseY < y + sizeY; 
}

text getButtonText(button bouton) {
	return bouton->texte;
}

SDL_Color getButtonBackground(button bouton) {
	return bouton->backgroud;
}

SDL_Color getButtonHoverColor(button bouton) {
	return bouton->hoverColor;
}

unsigned int getButtonId(button bouton) {
	return bouton->id;
}