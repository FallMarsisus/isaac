#include "ecs.h"

// The ECSManager holds entity states and component pointers.
typedef struct ECSManager {
    Entity nextEntity;                       // Next available entity ID
    bool active[MAX_ENTITIES];               // Whether an entity is active
    void* components[MAX_ENTITIES][MAX_COMPONENTS]; // Component storage
    Entity freeList[MAX_ENTITIES];           // Stores reusable entity IDs
    int freeCount;                           // Number of reusable IDs
} ECSManager;

static ECSManager* manager = NULL;
static bool removal_flags[MAX_ENTITIES] = { false };

void ECS_CreateManager() {
    manager = malloc(sizeof(ECSManager));
    if (!manager) {
        fprintf(stderr, "Failed to allocate ECS Manager\n");
        exit(1);
    }

    manager->nextEntity = 0;
    manager->freeCount = 0;
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        manager->active[e] = false;
        for (int comp = 0; comp < MAX_COMPONENTS; comp++) {
            manager->components[e][comp] = NULL;
        }
    }
}

void ECS_DestroyManager() {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (manager->active[e]) {
            for (int comp = 0; comp < MAX_COMPONENTS; comp++) {
                free(manager->components[e][comp]);
                manager->components[e][comp] = NULL;
            }
        }
    }
    free(manager);
    manager = NULL;
}

Entity ECS_CreateEntity() {
    Entity newEntity;

    // Reuse an old entity slot if available
    if (manager->freeCount > 0) {
        newEntity = manager->freeList[--manager->freeCount];
        // Ensure old components are cleared
        for (int comp = 0; comp < MAX_COMPONENTS; comp++) {
            free(manager->components[newEntity][comp]);
            manager->components[newEntity][comp] = NULL;
        }
    } else {
        if (manager->nextEntity >= MAX_ENTITIES) {
            fprintf(stderr, "Maximum number of entities reached.\n");
            return (Entity)-1;
        }
        newEntity = manager->nextEntity++;
    }

    manager->active[newEntity] = true;

    EntityCreatedEvent* event = malloc(sizeof(EntityCreatedEvent));
    event->entity = newEntity;
    trigger_event(EVENT_ENTITY_CREATED, event, true);

    return newEntity;
}

void ECS_RemoveEntity(Entity entity) {
    if (entity >= MAX_ENTITIES || !manager->active[entity]) {
        fprintf(stderr, "Attempted to remove invalid or already removed entity %d.\n", entity);
        return;
    }

    if (removal_flags[entity]) {
        fprintf(stderr, "Entity %d is already marked for removal.\n", entity);
        return;
    }

    removal_flags[entity] = true;
    
    EntityRemovedEvent* event = malloc(sizeof(EntityRemovedEvent));
    event->entity = entity;
    trigger_event(EVENT_ENTITY_REMOVED, event, true);
}

void ECS_ProcessRemovals() {
    for (Entity e = 0; e < manager->nextEntity; e++) {
        if (removal_flags[e]) {
            for (int comp = 0; comp < MAX_COMPONENTS; comp++) {
                free(manager->components[e][comp]);
                manager->components[e][comp] = NULL;
            }
            manager->active[e] = false;
            removal_flags[e] = false;

            // Add to free list for future reuse
            manager->freeList[manager->freeCount++] = e;
        }
    }
}

void* ECS_AddComponent(Entity entity, ComponentType component_type, int component_size) {
    if (entity >= MAX_ENTITIES || !manager->active[entity]) {
        fprintf(stderr, "Invalid entity %d for component addition.\n", entity);
        return NULL;
    }
    if (component_type < 0 || component_type >= MAX_COMPONENTS) {
        fprintf(stderr, "Invalid component type %d.\n", component_type);
        return NULL;
    }

    void* component = malloc(component_size);
    if (!component) {
        fprintf(stderr, "Failed to allocate component.\n");
        return NULL;
    }

    manager->components[entity][component_type] = component;
    return component;
}

void* ECS_GetComponent(Entity entity, ComponentType component_type) {
    if (!manager) {
        fprintf(stderr, "ECS Manager not initialized\n");
        return NULL;
    }

    if (entity >= MAX_ENTITIES || !manager->active[entity]) {
        return NULL;
    }
    if (component_type < 0 || component_type >= MAX_COMPONENTS) {
        return NULL;
    }
    return manager->components[entity][component_type];
}

bool ECS_HasComponent(Entity entity, ComponentType component_type) {
    if (entity >= MAX_ENTITIES || !manager->active[entity]) {
        return false;
    }
    if (component_type < 0 || component_type >= MAX_COMPONENTS) {
        return false;
    }
    return (manager->components[entity][component_type] != NULL);
}

void ECS_IterateEntities(void (*action)(Entity entity, void* user_data), void* user_data) {
    for (Entity e = 0; e < manager->nextEntity; e++) {
        if (manager->active[e])
            action(e, user_data);
    }
}

Entity ECS_GetFirstEntity() {
    for (Entity e = 0; e < manager->nextEntity; e++) {
        if (manager->active[e])
            return e;
    }
    return (Entity)-1;
}

Entity ECS_GetNextEntity(Entity current) {
    for (Entity e = current + 1; e < manager->nextEntity; e++) {
        if (manager->active[e])
            return e;
    }
    return (Entity)-1;
}
