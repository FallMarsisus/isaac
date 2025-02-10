#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include <stdint.h>

#include "../../utils/betterArrays/id_array.h"
#include "../../utils/betterArrays/dyn_arrays.h"
#include "../../utils/betterArrays/queue.h"
#include "../../utils/vector.h"
#include "items/itemData.h"

typedef enum {
    POSITION,
    BODY,
    SPRITE,
    ANIMATION,
    PLAYER,
    PATHFINDING,
    TIMER,
    SCRIPT,
    TELEPORT,
    PARENT,
    CHILD,
    INVENT,
    HEALTH,
    ITEM,
    DAMAGER,
    STATE_MACHINE,
    SWORD_C
} ComponentType;

// Position and velocity component
typedef struct {
    float x, y;      // Position
    float vx, vy;    // Velocity
    bool camFixed;
} PositionComponent;

typedef struct {
    bool is_dynamic;       // True if the object is dynamic; false if static
    float mass;            // Mass of the object
    SDL_Rect hitbox;       // Collision boundary
    float restitution;     // Bounciness (0.0 - 1.0)
    float friction;        // Friction coefficient
} RigidbodyComponent;

// Sprite component (for rendering a texture)
typedef struct {
    int width, height; // Dimensions of the entity

    float angle; // set the angle.
    SDL_Point* center; // the center where the texture will be rotated.
    SDL_RendererFlip flip; // the flip of the texture.

    SDL_Texture* texture;  // Texture for the sprite
} SpriteComponent;

typedef struct {
    SDL_Rect* current_img;

    dyn_array* animations;
    int anim_index;
    int counter;
    bool playing;

    int last_change;
} AnimationComponent;

typedef struct {
    float interval;
    int amount;
} Anim;

// Player movement component
typedef struct {
    float speed;  // Movement speed of the player
} PlayerMovementComponent;

typedef struct {
    int* path;          // Dynamic array to store the path (x, y pairs)
    int path_length;    // Number of steps in the path
    int current_step;   // Current step in the path
    uint32_t target;

    int speed;
    int last_update;
} PathfindingComponent;

typedef struct {
    int last;
    float time;
    bool playing;
} TimerComponent;

typedef struct {
    void (*update)(uint32_t entity, SDL_Rect cam);
} ScriptComponent;

typedef struct {
    int posX; int posY;
} TeleporterComponent;

typedef struct {
    ID_array* children;
} ParentComponent;

typedef struct {
    int offsetX; int offsetY;
    bool is_relative;

    uint32_t parent;
} ChildComponent;


