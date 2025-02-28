#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../event/eventList.h"
#include "../event/eventSystem.h"
#include "components.h"

typedef uint32_t Entity;

void ECS_CreateManager();
void ECS_ClearManager();
void ECS_DestroyManager();

Entity ECS_CreateEntity();
void ECS_RemoveEntity(Entity entity);
void ECS_ProcessRemovals();

void add_removal_flag(Entity entity);
bool ECS_IsEntityActive(Entity entity);

void* ECS_AddComponent(Entity entity, ComponentType component_type, size_t component_size);
void* ECS_GetComponent(Entity entity, ComponentType component_type);
bool ECS_HasComponent(Entity entity, ComponentType component_type);

void ECS_IterateEntities(void (*action)(Entity entity, void* user_data), void* user_data);

Entity ECS_GetFirstEntity();
Entity ECS_GetNextEntity(Entity current);
