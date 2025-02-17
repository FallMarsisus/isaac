#ifndef ROOM_GENERATION_H
#define ROOM_GENERATION_H

#include "../ecs.h"
#include "../components.h"
#include "../items/itemData.h"
#include "../items/itemList.h"
#include "../items/itemSystem.h"
#include "../../objects/enemies/enemies.h"
#include "../../objects/blocks/blocks.h"
#include <stdlib.h>
#include <time.h>
#include "../../../utils/pi.h"

void generate_room(int room_x, int room_y, uint32_t player_id);

#endif 