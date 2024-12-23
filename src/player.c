#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "display.h"

typedef struct player_s {
	SDL_Texture* texture;
	int x;
	int y;
	int sizeX;
	int sizeY;
}* player;

player createPlayer(int x, int y, int sizeX, int sizeY, const char* fileName, SDL_Renderer* renderer) {
	player joueur = malloc(sizeof(struct player_s));
	joueur->x = x;
	joueur->y = y;
	joueur->sizeX = sizeX;
	joueur->sizeY = sizeY;
	joueur->texture = loadBitMap(fileName, renderer);
	return joueur;
}

// placeholder si besoin d'ajouter des trucs à free
void freePlayer(player joueur) {
    SDL_DestroyTexture(joueur->texture);
	free(joueur);
}

void displayPlayer(player joueur, SDL_Renderer* renderer) {
	drawBitmap(joueur->texture, renderer, joueur->x, joueur->y, joueur->sizeX, joueur->sizeY);
}

void move(player joueur, int deltaX, int deltaY) {
	joueur->x += deltaX;
	joueur->y += deltaY;
}

void teleport(player joueur, int x, int y) {
	joueur->x = x;
	joueur->y = y;
}

void reSizePlayer(player joueur, int newSizeX, int newSizeY) {
	joueur->sizeX = newSizeX;
	joueur->sizeY = newSizeY;
}

void clipPlayer(player joueur, int maxX, int maxY) {
	if (joueur->x < 0) joueur->x = 0;
	if (joueur->y < 0) joueur->y = 0;
	if (joueur->x > maxX) joueur->x = maxX;
	if (joueur->y > maxY) joueur->y = maxY;
}
