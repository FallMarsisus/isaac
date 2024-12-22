#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>


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

void displayBitmap(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int sizeX, int sizeY) {

	if (renderer == NULL) {
		fprintf(stderr, "Renderer should not be NULL.");
	}
	
	//Crée l'objet contenant la texture
	SDL_Rect spriteRect = {x, y, sizeX, sizeY};
	
	SDL_RenderCopy(renderer, texture, NULL, &spriteRect);

}