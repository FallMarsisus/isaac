#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../event/eventList.h"
#include "../event/eventSystem.h"

#include "components.h"

#define MAX_ENTITIES 1000000
#define MAX_COMPONENT_TYPES 32

typedef uint32_t Entity;

typedef struct {
    void* components[MAX_COMPONENT_TYPES]; // Array of pointers to component data
    bool active;                          // Whether the entity is active
    Entity next;                          // Index of the next active entity
} EntityRecord;

typedef struct {
    EntityRecord* entities;               // Dynamically allocated array of entities
    int* component_sizes;                 // Dynamically allocated array of component sizes
    int entity_count;                     // Number of active entities
    int component_count;                  // Number of component types
} ECS_Manager;

void ECS_CreateManager(int component_count);
void ECS_DestroyManager();

uint32_t ECS_CreateEntity();
void ECS_RemoveEntity(uint32_t entity);

void* ECS_AddComponent(uint32_t entity, ComponentType component_type, int component_size);
void* ECS_GetComponent(uint32_t entity, ComponentType component_type);
bool ECS_HasComponent(uint32_t entity, ComponentType component_type);
void ECS_ClearComponent(uint32_t entity, ComponentType component_type);

void ECS_IterateEntities(void (*action)(Entity entity, void* user_data), void* user_data);

Entity ECS_GetFirstEntity();
Entity ECS_GetNextEntity(Entity current_entity);