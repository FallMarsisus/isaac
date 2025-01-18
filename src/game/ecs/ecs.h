#ifndef ECS_H
#define ECS_H

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    void* components;
    size_t component_size;
} ComponentArray;

typedef struct {
    uint32_t* entity_ids;
    size_t capacity;
    size_t count;
    ComponentArray* components;
} ECS_Manager;

ECS_Manager* ECS_CreateManager();
void ECS_DestroyManager(ECS_Manager* ecs);
uint32_t ECS_CreateEntity(ECS_Manager* ecs);
void* ECS_AddComponent(ECS_Manager* ecs, uint32_t entity, size_t component_type, size_t component_size);
void* ECS_GetComponent(ECS_Manager* ecs, uint32_t entity, size_t component_type);
void ECS_RemoveEntity(ECS_Manager* ecs, uint32_t entity);

#endif
