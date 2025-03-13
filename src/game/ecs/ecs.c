#include "ecs.h"

#define INITIAL_ENTITY_CAPACITY   1024
#define INITIAL_COMPONENT_CAPACITY 32
#define INITIAL_FREELIST_CAPACITY 512

typedef struct ECSManager {
    Entity nextEntity;
    int entity_capacity;
    int component_capacity;
    
    bool* active;
    bool* removal_flags;
    void*** components;  // components[entity][component_type]
    
    Entity* freeList;
    int freeList_size;
    int freeList_capacity;
} ECSManager;

static ECSManager* manager = NULL;

void expand_entities() {
    int new_cap = manager->entity_capacity * 2;
    
    bool* new_active = realloc(manager->active, new_cap * sizeof(bool));
    if (!new_active) {
        fprintf(stderr, "Failed to expand entity capacity\n");
        exit(1);
    }
    manager->active = new_active;
    
    bool* new_removal = realloc(manager->removal_flags, new_cap * sizeof(bool));
    if (!new_removal) {
        fprintf(stderr, "Failed to expand removal flags\n");
        exit(1);
    }
    manager->removal_flags = new_removal;
    
    void*** new_components = realloc(manager->components, new_cap * sizeof(void**));
    if (!new_components) {
        fprintf(stderr, "Failed to expand components array\n");
        exit(1);
    }
    manager->components = new_components;
    
    for (int e = manager->entity_capacity; e < new_cap; e++) {
        manager->active[e] = false;
        manager->removal_flags[e] = false;
        manager->components[e] = calloc(manager->component_capacity, sizeof(void*));
    }
    
    manager->entity_capacity = new_cap;
}
void expand_components(int new_capacity) {
    for (int e = 0; e < manager->entity_capacity; e++) {
        void** new_comps = realloc(manager->components[e], new_capacity * sizeof(void*));
        if (!new_comps) {
            fprintf(stderr, "Failed to expand component capacity\n");
            exit(1);
        }
        manager->components[e] = new_comps;
        
        for (int c = manager->component_capacity; c < new_capacity; c++) {
            manager->components[e][c] = NULL;
        }
    }
    manager->component_capacity = new_capacity;
}

void ECS_CreateManager() {
    manager = malloc(sizeof(ECSManager));
    if (!manager) {
        fprintf(stderr, "Failed to allocate ECS Manager\n");
        exit(1);
    }

    manager->entity_capacity = INITIAL_ENTITY_CAPACITY;
    manager->component_capacity = INITIAL_COMPONENT_CAPACITY;
    manager->nextEntity = 0;
    
    manager->active = calloc(manager->entity_capacity, sizeof(bool));
    manager->removal_flags = calloc(manager->entity_capacity, sizeof(bool));
    
    manager->components = malloc(manager->entity_capacity * sizeof(void**));
    for (int i = 0; i < manager->entity_capacity; i++) {
        manager->components[i] = calloc(manager->component_capacity, sizeof(void*));
    }
    
    manager->freeList_capacity = INITIAL_FREELIST_CAPACITY;
    manager->freeList = malloc(manager->freeList_capacity * sizeof(Entity));
    manager->freeList_size = 0;
}
void ECS_ClearManager() {
    // Free all components for active entities
    for (int e = 0; e < manager->entity_capacity; e++) {
        if (manager->active[e]) {
            for (int c = 0; c < manager->component_capacity; c++) {
                free(manager->components[e][c]);
                manager->components[e][c] = NULL;
            }
            manager->active[e] = false;
            manager->removal_flags[e] = false;
        }
    }

    // Reset freelist
    manager->freeList_size = 0;
    manager->nextEntity = 0;
}
void ECS_DestroyManager() {
    // First process any pending removals to avoid double-free issues
    ECS_ProcessRemovals();
    
    // Now free all remaining components and structures
    for (int e = 0; e < manager->entity_capacity; e++) {
        for (int c = 0; c < manager->component_capacity; c++) {
            free(manager->components[e][c]);
        }
        free(manager->components[e]);
    }
    free(manager->components);
    free(manager->active);
    free(manager->removal_flags);
    free(manager->freeList);
    free(manager);
    manager = NULL;
}


Entity ECS_CreateEntity() {
    if (manager->freeList_size > 0) {
        Entity e = manager->freeList[--manager->freeList_size];
        manager->active[e] = true;
        
        for (int c = 0; c < manager->component_capacity; c++) {
            free(manager->components[e][c]);
            manager->components[e][c] = NULL;
        }
        
        EntityCreatedEvent* event = malloc(sizeof(EntityCreatedEvent));
        event->entity = e;
        trigger_event(EVENT_ENTITY_CREATED, event, true);
        return e;
    }
    
    if (manager->nextEntity >= manager->entity_capacity) {
        expand_entities();
    }
    
    Entity e = manager->nextEntity++;
    manager->active[e] = true;
    
    EntityCreatedEvent* event = malloc(sizeof(EntityCreatedEvent));
    event->entity = e;
    trigger_event(EVENT_ENTITY_CREATED, event, true);
    return e;
}
void ECS_RemoveEntity(Entity entity) {
    if (entity >= manager->entity_capacity || !manager->active[entity]) {
        fprintf(stderr, "Attempted to remove invalid entity %zu\n", entity);
        return;
    }
    
    if (manager->removal_flags[entity]) {
        fprintf(stderr, "Entity %zu already marked for removal\n", entity);
        return;
    }
    
    EntityRemovedEvent* event = malloc(sizeof(EntityRemovedEvent));
    event->entity = entity;
    trigger_event(EVENT_ENTITY_REMOVED, event, true);
}
void ECS_ProcessRemovals() {
    for (Entity e = 0; e < manager->nextEntity; e++) {
        if (manager->removal_flags[e]) {
            // Free all components for this entity
            for (int c = 0; c < manager->component_capacity; c++) {
                free(manager->components[e][c]);
                manager->components[e][c] = NULL;
            }

            if (manager->freeList_size >= manager->freeList_capacity) {
                int new_cap = manager->freeList_capacity * 2;
                Entity* new_free = realloc(manager->freeList, new_cap * sizeof(Entity));
                if (!new_free) {
                    fprintf(stderr, "Failed to expand free list\n");
                    exit(1);
                }
                manager->freeList = new_free;
                manager->freeList_capacity = new_cap;
            }
            manager->freeList[manager->freeList_size++] = e;
            
            manager->active[e] = false;
            manager->removal_flags[e] = false;
        }
    }
}

void add_removal_flag(Entity entity) {
    if (entity >= manager->entity_capacity || !manager->active[entity]) {
        fprintf(stderr, "Invalid entity %zu\n", entity);
        return;
    }
    manager->removal_flags[entity] = true;
}
bool ECS_IsEntityActive(Entity entity) {
    return manager->active[entity] && !manager->removal_flags[entity];
}

void* ECS_AddComponent(Entity entity, ComponentType component_type, int component_size) {
    if (entity >= manager->entity_capacity || !manager->active[entity]) {
        fprintf(stderr, "Invalid entity %zu\n", entity);
        return NULL;
    }
    
    if (component_type >= manager->component_capacity) {
        int new_cap = component_type + 1;
        expand_components(new_cap);
    }
    
    if (manager->components[entity][component_type] != NULL) {
        fprintf(stderr, "Component %zu already exists for entity %zu\n", component_type, entity);
        return NULL;
    }
    
    void* component = calloc(1, component_size);
    if (!component) {
        fprintf(stderr, "Failed to allocate component\n");
        return NULL;
    }
    manager->components[entity][component_type] = component;
    return component;
}
void* ECS_GetComponent(Entity entity, ComponentType component_type) {
    if (entity >= manager->entity_capacity || 
        component_type >= manager->component_capacity ||
        !manager->active[entity]) {
        return NULL;
    }
    return manager->components[entity][component_type];
}
bool ECS_HasComponent(Entity entity, ComponentType component_type) {
    return ECS_GetComponent(entity, component_type) != NULL;
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
