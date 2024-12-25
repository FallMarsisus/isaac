#include <stdbool.h>
#include <stdlib.h>
#include "text.h"
#include "display.h"

typedef struct button_s{
	text texte;
	SDL_Color backgroud;	
	SDL_Color hoverColor;
	int margin;
	unsigned int id;
}* button;

button createButton(SDL_Renderer* renderer, text texte, SDL_Color background, SDL_Color hoverColor, unsigned int id, int margin) {
	button bouton = malloc(sizeof(struct button_s));

	bouton->texte = texte;
	bouton->backgroud = background;
	bouton->hoverColor = hoverColor;
	bouton->id = id;
	bouton->margin = margin;

	return bouton;
}


void displayButton(SDL_Renderer* renderer, button bouton, bool isHovering) {
	int x = getTextX(bouton->texte);
	int y = getTextY(bouton->texte);
	int sizeX = getTextSizeX(bouton->texte);
	int sizeY = getTextSizeY(bouton->texte);
	SDL_Rect rect = {x - bouton->margin - 1, y - bouton->margin - 1, sizeX + 2 * bouton->margin, sizeY + 2 * bouton->margin};

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

	return x - bouton->margin <= mouseX && mouseX < x + sizeX + bouton->margin && y - bouton->margin <= mouseY && mouseY < y + sizeY + bouton->margin;
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