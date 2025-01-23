#pragma once

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "../components.h"
#include "../ecs.h"
#include "a_star.h" // Assuming you have the A* implementation in a separate header

void init_pathfinding_component(PathfindingComponent* targetComp, u_int32_t target);
void free_pathfinding_component(PathfindingComponent* targetComp);

void update_pathfinding_system(uint32_t id, ECS_Manager* ecs, int** grid, SDL_Rect cam);