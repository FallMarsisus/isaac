#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include <stdint.h>

#include "../../utils/betterArrays/id_array.h"
#include "../../utils/betterArrays/dyn_arrays.h"
#include "../../utils/betterArrays/queue.h"
#include "../../utils/vector.h"
#include "./physics/forceSystem.h"
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
    SWORD_C,
    EFFECT,
    TILE
} ComponentType;

// Position and velocity component
typedef struct {
    float x, y;      // Position
    float vx, vy;    // Velocity
    float ax, ay;   // Acceleration
    bool camFixed;
} PositionComponent;

typedef struct RigidbodyComponent {
	SDL_Rect hitbox;
    bool is_dynamic;
    float friction;
    float restitution;
    float mass;
    float forceX;
    float forceY;
	dyn_array* forces;

    bool active;
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
    int tile_x;
    int tile_y;

    int tile_width;
    int tile_height;
} TileComponent;

typedef struct {
    int* path;          // Dynamic array to store the path (x, y pairs)
    int path_length;    // Number of steps in the path
    int current_step;   // Current step in the path
    uint32_t target;

    float speed;
    int last_update;
} PathfindingComponent;

typedef struct {
    int last;
    float time;
    bool playing;
} TimerComponent;

typedef struct {
    void* data;

    void (*update)(uint32_t entity, SDL_Rect cam);
} ScriptComponent;

typedef struct {
    ID_array* children;
} ParentComponent;

typedef struct {
    int offsetX; int offsetY;
    bool is_relative;

    uint32_t parent;
} ChildComponent;

typedef struct {
    uint32_t id;

    int time;
    int end_time;

    bool has_physics; // If the effect is removed when it collides with a object
} EffectComponent;