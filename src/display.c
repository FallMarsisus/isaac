#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>
#include "display.h"

SDL_Texture* loadBitMap(const char* fileName, SDL_Renderer* renderer) {

	// load the image and free it direcly to save ram space
	SDL_Surface* image = SDL_LoadBMP(fileName);

	if (image == NULL) {
		fprintf(stderr, "SDL_LoadBMP Error: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	if (texture == NULL) {
		fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
		return NULL;
	}

	return texture;
}

void drawBitmap(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int sizeX, int sizeY) {

	if (renderer == NULL) {
		fprintf(stderr, "Renderer should not be NULL.");
	}
	
	//Crée l'objet contenant la texture
	SDL_Rect spriteRect = {x, y, sizeX, sizeY};
	
	SDL_RenderCopy(renderer, texture, NULL, &spriteRect);
}


void drawText(SDL_Renderer *renderer, char* text, int x, int y, int sizeX, int sizeY, TTF_Font* font, int red, int green, int blue) {
	SDL_Color color = {red, green, blue};

	SDL_Surface* messageSurface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
	SDL_FreeSurface(messageSurface);

	SDL_Rect messageRect = {x, y, sizeX, sizeY};

	SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

}