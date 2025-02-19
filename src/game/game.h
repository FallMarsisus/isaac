#pragma once

#include "../utils/betterArrays/id_array.h"

#include "structure/map.h"

#include "event/eventList.h"
#include "event/eventSystem.h"
#include "event/events/listeners.h"

#include "ecs/ecs.h"
#include "ecs/components.h"
#include "ecs/systems.h"

typedef struct game_s Game;

void create_game(int win_width, int win_height);
void free_game();

void on_entity_created(Event event);
void on_entity_removed(Event event);

void change_room(int x, int y);

void update_game(int win_width, int win_height, float delta);

void draw_game(SDL_Renderer* renderer, int win_width, int win_height, int true_width , int true_height);