#ifndef ECS_H
#define ECS_H

#include "components.h"

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    void* components;        // Array of components
    size_t component_size;   // Size of each component
    uint8_t* entity_mask;    // Array of flags indicating component presence
} ComponentArray;

typedef struct {
    uint32_t* entity_ids;    // Array of entity IDs
    size_t capacity;         // Total capacity of entities
    size_t count;            // Current number of entities
    ComponentArray* components; // Array of component arrays
    size_t component_count;  // Number of component types
} ECS_Manager;

ECS_Manager* ECS_CreateManager(size_t component_count);
void ECS_DestroyManager(ECS_Manager* ecs);
uint32_t ECS_CreateEntity(ECS_Manager* ecs);
void* ECS_AddComponent(ECS_Manager* ecs, uint32_t entity, ComponentType component_type, size_t component_size);
void* ECS_GetComponent(ECS_Manager* ecs, uint32_t entity, ComponentType component_type);
int ECS_HasComponent(ECS_Manager* ecs, uint32_t entity, ComponentType component_type);
void ECS_RemoveEntity(ECS_Manager* ecs, uint32_t entity);

#endif
