#include "ecs.h"
#include <string.h>

ECS_Manager* ecs;

void ECS_CreateManager(int component_count) {
    ecs = (ECS_Manager*)malloc(sizeof(ECS_Manager));

    ecs->st = create_structure();
    
    ecs->components = (ComponentArray*)malloc(component_count * sizeof(ComponentArray));
    ecs->component_count = component_count;

    for (int i = 0; i < component_count; ++i) {
        ecs->components[i].components = NULL;
        ecs->components[i].entity_mask = calloc(ecs->st->array->capacity, sizeof(uint8_t)); // Initialize all masks to 0
        ecs->components[i].component_size = 0;
    }

    register_listener(EVENT_ENTITY_REMOVED, onEntityRemove);
}
void ECS_DestroyManager() {
    unregister_listener(EVENT_ENTITY_REMOVED, onEntityRemove);

    free_structure(ecs->st);

    for (int i = 0; i < ecs->component_count; ++i) {
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
    int capacity = ecs->st->array->capacity;
    if (ecs->st->array->len >= capacity) {
        capacity *= 2;
        for (int i = 0; i < ecs->component_count; ++i) {
            printf("Reallocating component %d to %d\n", i, capacity * ecs->components[i].component_size);
            fflush(stdout);

            ecs->components[i].components = realloc(
                ecs->components[i].components, 
                capacity * ecs->components[i].component_size
            );
            ecs->components[i].entity_mask = realloc(
                ecs->components[i].entity_mask, 
                capacity * sizeof(uint8_t)
            );
            memset(
                ecs->components[i].entity_mask + ecs->st->array->len, 0, 
                (capacity - ecs->st->array->len)
            );/**/ // Clear new mask entries
        }
    }

    uint32_t entity = create_element(ecs->st);
    return entity;
}

void* ECS_AddComponent(uint32_t entity, ComponentType component_type, int component_size) {
    ComponentArray* array = &ecs->components[component_type];
    if (!array->components) {
        array->components = malloc(ecs->st->array->capacity * component_size);
        array->component_size = component_size;
        memset(array->components, 0, ecs->st->array->capacity * component_size);
    }
    int index = get_index(ecs->st, entity);

    array->entity_mask[index] = 1; // Mark the component as present for this entity
    return (char*)array->components + (index * component_size);
}

void* ECS_GetComponent(uint32_t entity, ComponentType component_type) {
    ComponentArray* array = &ecs->components[component_type];
    int index = get_index(ecs->st, entity);
    if(index == -1) return NULL;

    if (!array->entity_mask[index]) return NULL; // Return NULL if the component is not present
    return (char*)array->components + (index * array->component_size);
}

bool ECS_HasComponent(uint32_t entity, ComponentType component_type) {
    int index = get_index(ecs->st, entity);
    if (index == -1) return false; // Entity does not exist
    return ecs->components[component_type].entity_mask[index] != NULL; // Check presence of the component
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


void ECS_RemoveEntity(uint32_t entity) {
    EntityRemovedEvent* event = malloc(sizeof(EntityRemovedEvent));
    event->entity = entity;
    trigger_event(EVENT_ENTITY_REMOVED, event, true);

    int index = get_index(ecs->st, entity);
    if(!index) return;

    for (int i = 0; i < ecs->component_count; ++i) {
        ComponentArray* comp = &ecs->components[i];
        comp->entity_mask[index] = 0;
    }
}

void onEntityRemove(Event event) {
    EntityRemovedEvent* e = (EntityRemovedEvent*)event.data;
    uint32_t entity = e->entity;

    int dest = get_index(ecs->st, entity);
    int src = remove_element(ecs->st, entity); // Returns index of the last element

    int prev_len = ecs->st->array->len;
    int prev_capacity = ecs->st->array->capacity;

    if (src != dest) { // Only move if necessary
        for (int i = 0; i < ecs->component_count; ++i) {
            ComponentArray* comp = &ecs->components[i];

            if (comp->entity_mask[src]) { // Move component data if it exists
                memcpy(
                    (char*)comp->components + (dest * comp->component_size),
                    (char*)comp->components + (src * comp->component_size),
                    comp->component_size
                );
                comp->entity_mask[dest] = 1;
            } else {
                comp->entity_mask[dest] = 0;
            }
        }
    }
    
    // Shrink storage if necessary
    if (prev_len * 4 < prev_capacity) {
        int new_capacity = prev_capacity / 2;
        for (int i = 0; i < ecs->component_count; ++i) {
            ecs->components[i].components = realloc(
                ecs->components[i].components, 
                new_capacity * ecs->components[i].component_size
            );
            ecs->components[i].entity_mask = realloc(
                ecs->components[i].entity_mask, 
                new_capacity * sizeof(uint8_t)
            );
        }
    }
}