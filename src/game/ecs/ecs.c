#include "ecs.h"
#include <string.h>
#include <stdio.h>

ECS_Manager* ecs;

void ECS_CreateManager(int component_count) {
    ecs = (ECS_Manager*)malloc(sizeof(ECS_Manager));
    if (!ecs) {
        fprintf(stderr, "Failed to allocate memory for ECS manager\n");
        exit(1);
    }

    ecs->st = create_structure();
    if (!ecs->st) {
        fprintf(stderr, "Failed to create structure\n");
        free(ecs);
        exit(1);
    }

    ecs->components = (ComponentArray*)malloc(component_count * sizeof(ComponentArray));
    if (!ecs->components) {
        fprintf(stderr, "Failed to allocate memory for component arrays\n");
        free_structure(ecs->st);
        free(ecs);
        exit(1);
    }
    ecs->component_count = component_count;

    for (int i = 0; i < component_count; ++i) {
        ecs->components[i].components = calloc(ecs->st->array->capacity, sizeof(void*));
        ecs->components[i].entity_mask = calloc(ecs->st->array->capacity, sizeof(uint8_t));

        if (!ecs->components[i].entity_mask) {
            fprintf(stderr, "Failed to allocate memory for entity mask\n");
            for (int j = 0; j < i; ++j) {
                free(ecs->components[j].entity_mask);
            }
            free(ecs->components);
            free_structure(ecs->st);
            free(ecs);
            exit(1);
        }
        ecs->components[i].component_size = 0; // Initialize to 0
    }

    register_listener(EVENT_ENTITY_REMOVED, onEntityRemove);
}

void ECS_DestroyManager() {
    unregister_listener(EVENT_ENTITY_REMOVED, onEntityRemove);

    for (int i = 0; i < ecs->component_count; ++i) {
        if (ecs->components[i].components) {
            for(int j = 0; j < ecs->st->array->len; j++) {
                if(ecs->components[i].entity_mask[j] == 1 && ecs->components[i].components[j]) {
                    free(ecs->components[i].components[j]);
                }
            }
            free(ecs->components[i].components);
        }
        free(ecs->components[i].entity_mask);
    }

    free_structure(ecs->st);

    free(ecs->components);
    free(ecs);
}

ECS_Manager* ECS_GetManager() {
    return ecs;
}

uint32_t ECS_CreateEntity() {
    int capacity = ecs->st->array->capacity;
    if (ecs->st->array->len >= capacity) {
        capacity *= 2;
        for (int i = 0; i < ecs->component_count; ++i) {
            ecs->components[i].components = realloc(ecs->components[i].components, capacity * sizeof(void*));
            ecs->components[i].entity_mask = realloc(ecs->components[i].entity_mask, capacity * sizeof(uint8_t));

            memset(ecs->components[i].entity_mask + ecs->st->array->len, 0, capacity - ecs->st->array->len);
        }
    }

    return create_element(ecs->st);
}

void ECS_RemoveEntity(uint32_t entity) {
    EntityRemovedEvent* event = malloc(sizeof(EntityRemovedEvent));
    event->entity = entity;
    trigger_event(EVENT_ENTITY_REMOVED, event, true);

    int index = get_index(ecs->st, entity);
    if (index == -1) {
        fprintf(stderr, "Invalid entity ID: %u\n", entity);
        return;
    }

    for(int i = 0; i < ecs->component_count; i++) {
        ecs->components[i].entity_mask[index] = 0;
    }
}

void onEntityRemove(Event event) {
    EntityRemovedEvent* e = (EntityRemovedEvent*)event.data;
    uint32_t entity = e->entity;

    int dest = get_index(ecs->st, entity);
    int src = remove_element(ecs->st, entity); // Returns index of the last element

    if (src != dest) { // Only move if necessary
        for (int i = 0; i < ecs->component_count; ++i) {
            ComponentArray* comp = &ecs->components[i];
            if (comp && comp->entity_mask && comp->entity_mask[src]) { // Move component data if it exists
                comp->components[dest] = comp->components[src];
                comp->entity_mask[dest] = 1;
            }
            comp->entity_mask[src] = 0; // Clear the source mask
        }
    }

    // Shrink storage if necessary
    int prev_len = ecs->st->array->len;
    int prev_capacity = ecs->st->array->capacity;
    if (prev_len * 4 < prev_capacity) {
        int new_capacity = prev_capacity / 2;
        for (int i = 0; i < ecs->component_count; ++i) {
            ecs->components[i].components = realloc(ecs->components[i].components, new_capacity * sizeof(void*));
            ecs->components[i].entity_mask = realloc(ecs->components[i].entity_mask, new_capacity * sizeof(uint8_t));
        }
    }
}

void* ECS_AddComponent(uint32_t entity, ComponentType component_type, int component_size) {
    if (component_type < 0 || component_type >= ecs->component_count) {
        fprintf(stderr, "Invalid component type: %d\n", component_type);
        return NULL;
    }

    if (component_size <= 0) {
        fprintf(stderr, "Invalid component size: %d\n", component_size);
        return NULL;
    }

    ComponentArray* array = &ecs->components[component_type];
    if(component_size > 0) {
        array->component_size = component_size; // Set the component size
        printf("Component type %d: component_size = %d\n", component_type, array->component_size);
    }

    int index = get_index(ecs->st, entity);
    if(index == -1) return NULL;
    array->entity_mask[index] = 1; // Mark the component as present for this entity

    array->components[index] = calloc(1, array->component_size);

    return array->components[index];
}

void* ECS_GetComponent(uint32_t entity, ComponentType component_type) {
    if (component_type < 0 || component_type >= ecs->component_count) {
        fprintf(stderr, "Invalid component type: %d\n", component_type);
        return NULL;
    }

    int index = get_index(ecs->st, entity);
    if (index == -1) {
        fprintf(stderr, "Invalid entity ID: %u\n", entity);
        return NULL;
    }

    ComponentArray* array = &ecs->components[component_type];
    if (!array->entity_mask[index]) {
        return NULL; // Component not present for this entity
    }

    return array->components[index];
}

bool ECS_HasComponent(uint32_t entity, ComponentType component_type) {
    if (component_type < 0 || component_type >= ecs->component_count) {
        fprintf(stderr, "Invalid component type: %d\n", component_type);
        return false;
    }

    int index = get_index(ecs->st, entity);
    if (index == -1) {
        fprintf(stderr, "Invalid entity ID: %u\n", entity);
        return false;
    }

    return ecs->components[component_type].entity_mask[index] == 1;
}

void ECS_ClearComponent(uint32_t entity, ComponentType component_type) {
    if (component_type < 0 || component_type >= ecs->component_count) {
        fprintf(stderr, "Invalid component type: %d\n", component_type);
        return;
    }

    int index = get_index(ecs->st, entity);
    if (index == -1) {
        fprintf(stderr, "Invalid entity ID: %u\n", entity);
        return;
    }

    ecs->components[component_type].entity_mask[index] = 0;
}