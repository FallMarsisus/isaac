#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../event/eventList.h"
#include "../event/eventSystem.h"
#include "components.h"

// Limits – adjust as needed
#define MAX_ENTITIES   1024
#define MAX_COMPONENTS 32

typedef uint32_t Entity;

// Core ECS functions
void ECS_CreateManager();
void ECS_DestroyManager();

Entity ECS_CreateEntity();
void ECS_RemoveEntity(Entity entity);
void ECS_ProcessRemovals();

void* ECS_AddComponent(Entity entity, ComponentType component_type, int component_size);
void* ECS_GetComponent(Entity entity, ComponentType component_type);
bool ECS_HasComponent(Entity entity, ComponentType component_type);

// Entity iteration
void ECS_IterateEntities(void (*action)(Entity entity, void* user_data), void* user_data);

// Entity iteration helpers
Entity ECS_GetFirstEntity();
Entity ECS_GetNextEntity(Entity current);
