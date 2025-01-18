#include "ecs.h"
#include <string.h>

ECS_Manager* ECS_CreateManager() {
    ECS_Manager* ecs = (ECS_Manager*)malloc(sizeof(ECS_Manager));
    ecs->capacity = 100;
    ecs->count = 0;
    ecs->entity_ids = (uint32_t*)malloc(ecs->capacity * sizeof(uint32_t));
    ecs->components = (ComponentArray*)malloc(ecs->capacity * sizeof(ComponentArray));
    memset(ecs->components, 0, ecs->capacity * sizeof(ComponentArray));
    return ecs;
}

void ECS_DestroyManager(ECS_Manager* ecs) {
    free(ecs->entity_ids);
    for (size_t i = 0; i < ecs->capacity; ++i) {
        if (ecs->components[i].components) {
            free(ecs->components[i].components);
        }
    }
    free(ecs->components);
    free(ecs);
}

uint32_t ECS_CreateEntity(ECS_Manager* ecs) {
    if (ecs->count >= ecs->capacity) {
        ecs->capacity *= 2;
        ecs->entity_ids = (uint32_t*)realloc(ecs->entity_ids, ecs->capacity * sizeof(uint32_t));
        ecs->components = (ComponentArray*)realloc(ecs->components, ecs->capacity * sizeof(ComponentArray));
    }
    uint32_t entity = ecs->count++;
    ecs->entity_ids[entity] = entity;
    return entity;
}

void* ECS_AddComponent(ECS_Manager* ecs, uint32_t entity, size_t component_type, size_t component_size) {
    if (!ecs->components[component_type].components) {
        ecs->components[component_type].component_size = component_size;
        ecs->components[component_type].components = malloc(ecs->capacity * component_size);
    }
    void* component = (char*)ecs->components[component_type].components + (entity * component_size);
    memset(component, 0, component_size);
    return component;
}

void* ECS_GetComponent(ECS_Manager* ecs, uint32_t entity, size_t component_type) {
    if (!ecs->components[component_type].components) return NULL;
    return (char*)ecs->components[component_type].components + (entity * ecs->components[component_type].component_size);
}

void ECS_RemoveEntity(ECS_Manager* ecs, uint32_t entity) {
    for (size_t i = 0; i < ecs->capacity; ++i) {
        if (ecs->components[i].components) {
            void* component = (char*)ecs->components[i].components + (entity * ecs->components[i].component_size);
            memset(component, 0, ecs->components[i].component_size);
        }
    }
}

