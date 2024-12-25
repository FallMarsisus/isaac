#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct room_s {
	int x;
	int y;
	struct room_s* up;
	struct room_s* down;
	struct room_s* right;
	struct room_s* left;
	void* entities;
}* room;

bool roomExists(const int x, const int y, dict rooms) {
	return mem(x, y, rooms);
}

room createRoom(const int x, const int y, void* entities, dict rooms) {
	if (roomExists(x, y, rooms)) {
		fprintf(stderr, "The room already exists");
		exit(EXIT_FAILURE);
	}

	room salle = malloc(sizeof(struct room_s));
	salle->x = x;
	salle->y = y;
	salle->entities = entities;

	room up = getValue(x, y-1, rooms);
	room down = getValue(x, y+1, rooms);
	room left = getValue(x-1, y, rooms);
	room right = getValue(x+1, y, rooms);

	salle->up = up;
	salle->down = down;
	salle->left = left;
	salle->right = right;

	if (up != NULL) up->down = salle;
	if (down != NULL) down->up = salle;
	if (left != NULL) left->right = salle;
	if (right != NULL) right->left = salle;

	if (!add(x, y, salle, rooms)) {
		fprintf(stderr, "failed to create the room\n");
		exit(EXIT_FAILURE);
	}

	return salle;
}

bool destroyRoom(const int x, const int y, dict rooms) {
	if (!roomExists(x, y, rooms)) {
		return false;
	}

	room salle = getValue(x, y, rooms);

	if (salle->up != NULL) salle->up->down = NULL;
	if (salle->down != NULL) salle->down->up = NULL;
	if (salle->left != NULL) salle->left->right = NULL;
	if (salle->right != NULL) salle->right->left = NULL;

	free(salle);
	if (!removeValue(x, y, rooms)) {
		fprintf(stderr, "failed to delete the room");
		exit(EXIT_FAILURE);
	}

	return true;
}

bool changeRoom(const int x, const int y, void* entities, dict rooms) {
	if (!roomExists(x, y, rooms)) {
		return false;
	}
	
	room salle = getValue(x, y, rooms);

	salle->entities = entities;
	return true;
}

void* getEntities(room salle) {return salle->entities;}

void getCoords(room salle, int* x, int* y) {
	*x = salle->x;
	*y = salle->y;
}

room getUp(room salle) {return salle->up;}
room getDown(room salle) {return salle->down;}
room getLeft(room salle) {return salle->left;}
room getRight(room salle) {return salle->right;}