#include "ecs.h"

static ECS_Manager* ecs_manager = NULL;
static Entity active_entity_head = -1;    // Index of the first active entity

void ECS_CreateManager(int component_count) {
    // Allocate memory for the ECS manager
    ecs_manager = (ECS_Manager*)malloc(sizeof(ECS_Manager));
    if (!ecs_manager) {
        fprintf(stderr, "Failed to allocate memory for ECS manager\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for entities
    ecs_manager->entities = (EntityRecord*)calloc(MAX_ENTITIES, sizeof(EntityRecord));
    if (!ecs_manager->entities) {
        fprintf(stderr, "Failed to allocate memory for entities\n");
        free(ecs_manager);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for component sizes
    ecs_manager->component_sizes = (int*)calloc(MAX_COMPONENT_TYPES, sizeof(int));
    if (!ecs_manager->component_sizes) {
        fprintf(stderr, "Failed to allocate memory for component sizes\n");
        free(ecs_manager->entities);
        free(ecs_manager);
        exit(EXIT_FAILURE);
    }

    // Initialize manager state
    ecs_manager->entity_count = 0;
    ecs_manager->component_count = component_count;
    active_entity_head = -1; // Initialize the linked list
}

void ECS_DestroyManager() {
    if (!ecs_manager) return;

    // Free all component data
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        for (int j = 0; j < MAX_COMPONENT_TYPES; ++j) {
            if (ecs_manager->entities[i].components[j]) {
                free(ecs_manager->entities[i].components[j]);
            }
        }
    }

    // Free allocated memory
    free(ecs_manager->entities);
    free(ecs_manager->component_sizes);
    free(ecs_manager);
    ecs_manager = NULL;
}

Entity ECS_CreateEntity() {
    if (!ecs_manager) return -1;

    for (Entity i = 0; i < MAX_ENTITIES; ++i) {
        if (!ecs_manager->entities[i].active) {
            ecs_manager->entities[i].active = true;
            ecs_manager->entity_count++;

            // Add the new entity to the linked list
            ecs_manager->entities[i].next = active_entity_head;
            active_entity_head = i;


            EntityCreatedEvent* event = malloc(sizeof(EntityCreatedEvent));
            event->entity = i;
            trigger_event(EVENT_ENTITY_CREATED, event, true);

            return i;
        }
    }

    return -1; // No more entities available
}

void ECS_RemoveEntity(Entity entity) {
    if (!ecs_manager || entity >= MAX_ENTITIES || !ecs_manager->entities[entity].active) {
        return;
    }
    
    // Mark the entity as inactive
    ecs_manager->entities[entity].active = false;
    ecs_manager->entity_count--;

    // Free all components associated with the entity
    for (int i = 0; i < MAX_COMPONENT_TYPES; ++i) {
        if (ecs_manager->entities[entity].components[i]) {
            free(ecs_manager->entities[entity].components[i]);
            ecs_manager->entities[entity].components[i] = NULL;
        }
    }

    // Remove the entity from the linked list
    if (entity == active_entity_head) {
        // If the entity is the head, update the head
        active_entity_head = ecs_manager->entities[entity].next;
    } else {
        // Otherwise, find the previous entity in the linked list
        Entity prev = active_entity_head;
        while (prev != -1 && ecs_manager->entities[prev].next != entity) {
            prev = ecs_manager->entities[prev].next;
        }
        if (prev != -1) {
            ecs_manager->entities[prev].next = ecs_manager->entities[entity].next;
        }
    }

    EntityRemovedEvent* event = malloc(sizeof(EntityRemovedEvent));
    event->entity = entity;
    trigger_event(EVENT_ENTITY_REMOVED, event, true);
}

void* ECS_AddComponent(Entity entity, ComponentType component_type, int component_size) {
    if (!ecs_manager || entity >= MAX_ENTITIES || !ecs_manager->entities[entity].active || component_type >= MAX_COMPONENT_TYPES) {
        return NULL;
    }

    // Allocate memory for the component if it doesn't already exist
    if (!ecs_manager->entities[entity].components[component_type]) {
        ecs_manager->entities[entity].components[component_type] = malloc(component_size);
        if (!ecs_manager->entities[entity].components[component_type]) {
            fprintf(stderr, "Failed to allocate memory for component\n");
            return NULL;
        }
        ecs_manager->component_sizes[component_type] = component_size;
    }

    return ecs_manager->entities[entity].components[component_type];
}

void* ECS_GetComponent(Entity entity, ComponentType component_type) {
    if (!ecs_manager || entity >= MAX_ENTITIES || !ecs_manager->entities[entity].active || component_type >= MAX_COMPONENT_TYPES) {
        return NULL;
    }

    return ecs_manager->entities[entity].components[component_type];
}

bool ECS_HasComponent(Entity entity, ComponentType component_type) {
    if (!ecs_manager || entity >= MAX_ENTITIES || !ecs_manager->entities[entity].active || component_type >= MAX_COMPONENT_TYPES) {
        return false;
    }

    return ecs_manager->entities[entity].components[component_type] != NULL;
}

void ECS_ClearComponent(Entity entity, ComponentType component_type) {
    if (!ecs_manager || entity >= MAX_ENTITIES || !ecs_manager->entities[entity].active || component_type >= MAX_COMPONENT_TYPES) {
        return;
    }

    if (ecs_manager->entities[entity].components[component_type]) {
        free(ecs_manager->entities[entity].components[component_type]);
        ecs_manager->entities[entity].components[component_type] = NULL;
    }
}

void ECS_IterateEntities(void (*action)(Entity entity, void* user_data), void* user_data) {
    if (!ecs_manager) return;

    for (Entity i = 0; i < MAX_ENTITIES; ++i) {
        if (ecs_manager->entities[i].active) {
            action(i, user_data); // Call the action function for each active entity
        }
    }
}

Entity ECS_GetFirstEntity() {
    return active_entity_head; // Directly return the head of the linked list
}

Entity ECS_GetNextEntity(Entity current_entity) {
    if (!ecs_manager || current_entity >= MAX_ENTITIES) return -1;
    return ecs_manager->entities[current_entity].next; // Directly return the next entity
}

/*
int main() {
    ECS_CreateManager(10);

    Entity entity1 = ECS_CreateEntity();
    Entity entity2 = ECS_CreateEntity();
    Entity entity3 = ECS_CreateEntity();

    // Iterate over all entities using a for loop
    for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
        printf("Entity %d is active\n", e);
    }

    ECS_DestroyManager();
    return 0;
}
    */