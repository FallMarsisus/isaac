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

typedef struct ecs_manager_s {
    uint32_t* entity_ids;    // Array of entity IDs
    size_t capacity;         // Total capacity of entities
    size_t count;            // Current number of entities
    ComponentArray* components; // Array of component arrays
    size_t component_count;  // Number of component types
} ECS_Manager;
/**
 * @brief Creates a new ECS manager.
 *
 * This function allocates and initializes a new ECS manager with the specified number of component types.
 *
 * @param component_count The number of different component types that the manager will handle.
 * @return A pointer to the newly created ECS manager.
 */
ECS_Manager* ECS_CreateManager(size_t component_count);

/**
 * @brief Destroys an ECS manager.
 *
 * This function deallocates all memory associated with the ECS manager, including all entities and components.
 *
 * @param ecs A pointer to the ECS manager to be destroyed.
 */
void ECS_DestroyManager(ECS_Manager* ecs);

/**
 * @brief Creates a new entity.
 *
 * This function creates a new entity within the ECS manager and returns its unique identifier.
 *
 * @param ecs A pointer to the ECS manager.
 * @return The unique identifier of the newly created entity.
 */
uint32_t ECS_CreateEntity(ECS_Manager* ecs);

/**
 * @brief Adds a component to an entity.
 *
 * This function adds a component of the specified type and size to the given entity.
 *
 * @param ecs A pointer to the ECS manager.
 * @param entity The unique identifier of the entity to which the component will be added.
 * @param component_type The type of the component to be added.
 * @param component_size The size of the component to be added.
 * @return A pointer to the newly added component.
 */
void* ECS_AddComponent(ECS_Manager* ecs, uint32_t entity, ComponentType component_type, size_t component_size);

/**
 * @brief Retrieves a component from an entity.
 *
 * This function retrieves a component of the specified type from the given entity.
 *
 * @param ecs A pointer to the ECS manager.
 * @param entity The unique identifier of the entity from which the component will be retrieved.
 * @param component_type The type of the component to be retrieved.
 * @return A pointer to the component, or NULL if the component is not present.
 */
void* ECS_GetComponent(ECS_Manager* ecs, uint32_t entity, ComponentType component_type);

/**
 * @brief Checks if an entity has a specific component.
 *
 * This function checks whether the given entity has a component of the specified type.
 *
 * @param ecs A pointer to the ECS manager.
 * @param entity The unique identifier of the entity to be checked.
 * @param component_type The type of the component to check for.
 * @return 1 if the entity has the component, 0 otherwise.
 */
int ECS_HasComponent(ECS_Manager* ecs, uint32_t entity, ComponentType component_type);

/**
 * @brief Removes an entity.
 *
 * This function removes the given entity from the ECS manager, marking all its components as absent.
 *
 * @param ecs A pointer to the ECS manager.
 * @param entity The unique identifier of the entity to be removed.
 */
void ECS_RemoveEntity(ECS_Manager* ecs, uint32_t entity);

#endif // ECS_H