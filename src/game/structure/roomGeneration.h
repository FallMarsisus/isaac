#ifndef ROOM_GENERATION_H
#define ROOM_GENERATION_H

#include <stdlib.h>
#include <time.h>

#include "../ecs/ecs.h"
#include "../ecs/components.h"
#include "../ecs/items/itemData.h"
#include "../ecs/items/itemList.h"
#include "../ecs/items/itemSystem.h"
#include "../objects/enemies/enemies.h"
#include "../objects/blocks/blocks.h"
#include "../../utils/pi.h"

void generate_room(int room_x, int room_y, uint32_t player_id);

#endif 