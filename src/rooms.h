#ifndef ROOMS_H
#define ROOMS_H

#include "dict.h"
#include <stdbool.h>

typedef struct room_s* room;

bool roomExists(const int x, const int y, dict rooms);
room createRoom(const int x, const int y, void* entities, dict rooms);
bool destroyRoom(const int x, const int y, dict rooms);
bool changeRoom(const int x, const int y, void* entities, dict rooms);
void* getEntities(room salle);
void getCoords(room salle, int* x, int* y);
room getUp(room salle);
room getDown(room salle);
room getLeft(room salle);
room getRight(room salle);

#endif // ROOMS_H