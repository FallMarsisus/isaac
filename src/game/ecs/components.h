#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../../utils/dyn_arrays.h"

typedef enum {
    POSITION,
    SPRITE,
    ANIMATION,
    PLAYER,
    TARGET
} ComponentType;

// Position and velocity component
typedef struct {
    float x, y;      // Position
    float vx, vy;    // Velocity
} PositionComponent;

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

// Player movement component
typedef struct {
    uint32_t entity;
    float speed;
} TargetMovementComponent;

typedef struct {
    bool is_rigid;

    SDL_FRect* hitbox;
} RigidbodyComponent;