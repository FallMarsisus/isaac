#include <SDL2/SDL.h>
#include "display.h"
#include "text.h"
#include "buttons.h"

const int numberOfButtons = 3;

button* createMainMenuButtons(SDL_Renderer* renderer, int winWidth, int winHeigth) {

	printf("Allocating memory for buttons\n");
	fflush(stdout);
	button* boutons = malloc(sizeof(button)*numberOfButtons);
	char* font = "./assets/fonts/FRESHFACE.ttf";

	printf("Creating first text\n");
	fflush(stdout);
	text firstText = createText(renderer, 0, 50, 255, 255, 255, "Play the game !", font, 24);
	sizeTextCoord(firstText, (winWidth-getTextSizeX(firstText))/2, 50);

	printf("Creating first button\n");
	fflush(stdout);
	button firstButton = createButton(renderer, firstText, (SDL_Color){0, 255, 0}, (SDL_Color){0, 150, 0}, 0);

	printf("Creating second text\n");
	fflush(stdout);
	text secondText = createText(renderer, 0, 50, 255, 255, 255, "Settings", font, 24);
	sizeTextCoord(secondText, (winWidth-getTextSizeX(secondText))/2, 150);

	printf("Creating second button\n");
	fflush(stdout);
	button secondButton = createButton(renderer, secondText, (SDL_Color){0, 255, 0}, (SDL_Color){0, 150, 0}, 1);

	printf("Creating third text\n");
	fflush(stdout);
	text thirdText = createText(renderer, 0, 50, 255, 255, 255, "Exit", font, 24);
	sizeTextCoord(thirdText, (winWidth-getTextSizeX(thirdText))/2, 250);

	printf("Creating third button\n");
	fflush(stdout);
	button thirdButton = createButton(renderer, thirdText, (SDL_Color){0, 255, 0}, (SDL_Color){0, 150, 0}, 2);

	printf("Assigning buttons to array\n");
	fflush(stdout);
	boutons[0] = firstButton;
	boutons[1] = secondButton;
	boutons[2] = thirdButton;

	printf("Returning buttons array\n");
	fflush(stdout);
	return boutons;
}


void displayMainMenu(SDL_Renderer* renderer, button* boutons) {

	for (int i = 0; i < numberOfButtons; i++) {
		displayButton(renderer, boutons[i], false);
	}
}