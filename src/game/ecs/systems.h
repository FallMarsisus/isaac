#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "../../display.h"

#include "ecs.h"
#include "components.h"
#include <SDL2/SDL.h>

void initialize_game(ECS_Manager* ecs);
void handle_input_system(ECS_Manager* ecs, SDL_Event* event);
void update_systems(ECS_Manager* ecs);
void render_systems(ECS_Manager* ecs, SDL_Renderer* renderer);

#endif
