#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

typedef struct player_s {
	SDL_Texture* texture;
	int x;
	int y;
	int sizeX;
	int sizeY;
}* player;

player createPlayer(int x, int y, int sizeX, int sizeY, const char* fileName, SDL_Renderer* renderer);
void freePlayer(player joueur);
void displayPlayer(player joueur, SDL_Renderer* renderer);
void move(player joueur, int deltaX, int deltaY);
void teleport(player joueur, int x, int y);
void reSizePlayer(player joueur, int newSizeX, int newSizeY);
void clipPlayer(player joueur, int maxX, int maxY);