#include "ecs.h"
#include <string.h>

ECS_Manager* ecs;

void ECS_CreateManager(size_t component_count) {
    ecs = (ECS_Manager*)malloc(sizeof(ECS_Manager));
    ecs->capacity = 100;
    ecs->count = 0;
    ecs->entity_ids = (uint32_t*)malloc(ecs->capacity * sizeof(uint32_t));
    ecs->components = (ComponentArray*)malloc(component_count * sizeof(ComponentArray));
    ecs->component_count = component_count;

    for (size_t i = 0; i < component_count; ++i) {
        ecs->components[i].components = NULL;
        ecs->components[i].entity_mask = (uint8_t*)calloc(ecs->capacity, sizeof(uint8_t)); // Initialize all masks to 0
        ecs->components[i].component_size = 0;
    }
}

void ECS_DestroyManager() {
    free(ecs->entity_ids);
    for (size_t i = 0; i < ecs->component_count; ++i) {
        if (ecs->components[i].components) free(ecs->components[i].components);
        free(ecs->components[i].entity_mask);
    }
    free(ecs->components);
    free(ecs);
}
ECS_Manager* ECS_GetManager() {
    return ecs;
}
uint32_t ECS_CreateEntity() {
    if (ecs->count >= ecs->capacity) {
        ecs->capacity *= 2;
        ecs->entity_ids = (uint32_t*)realloc(ecs->entity_ids, ecs->capacity * sizeof(uint32_t));
        for (size_t i = 0; i < ecs->component_count; ++i) {
            ecs->components[i].components = realloc(ecs->components[i].components, ecs->capacity * ecs->components[i].component_size);
            ecs->components[i].entity_mask = (uint8_t*)realloc(ecs->components[i].entity_mask, ecs->capacity * sizeof(uint8_t));
            memset(ecs->components[i].entity_mask + ecs->count, 0, (ecs->capacity - ecs->count)); // Clear new mask entries
        }
    }
    uint32_t entity = ecs->count++;
    ecs->entity_ids[entity] = entity;
    return entity;
}

void* ECS_AddComponent(uint32_t entity, ComponentType component_type, size_t component_size) {
    ComponentArray* array = &ecs->components[component_type];
    if (!array->components) {
        array->components = malloc(ecs->capacity * component_size);
        array->component_size = component_size;
        memset(array->components, 0, ecs->capacity * component_size);
    }
    array->entity_mask[entity] = 1; // Mark the component as present for this entity
    return (char*)array->components + (entity * component_size);
}

void* ECS_GetComponent(uint32_t entity, ComponentType component_type) {
    ComponentArray* array = &ecs->components[component_type];
    if (!array->entity_mask[entity]) return NULL; // Return NULL if the component is not present
    return (char*)array->components + (entity * array->component_size);
}

int ECS_HasComponent(uint32_t entity, ComponentType component_type) {
    return ecs->components[component_type].entity_mask[entity]; // Check presence of the component
}

void ECS_RemoveEntity(uint32_t entity) {
    for (size_t i = 0; i < ecs->component_count; ++i) {
        ecs->components[i].entity_mask[entity] = 0; // Mark the component as absent
    }
}
