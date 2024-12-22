#ifndef ROOMS_H_
#define ROOMS_H_

#include "list_entities.h"

#include <stdlib.h>
#include <assert.h>
#include "../utils/utils.h"

typedef struct room room;

room* create_room(int posx, int posy);
void free_room(room* r);

#endif