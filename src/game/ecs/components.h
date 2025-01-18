#pragma once

#include <SDL2/SDL.h>

typedef enum {
    POSITION,
    RENDER,
    SPRITE,
    PLAYER,
    TARGET
} ComponentType;

// Position and velocity component
typedef struct {
    float x, y;      // Position
    float vx, vy;    // Velocity
} PositionComponent;

// Rendering size component
typedef struct {
    int width, height; // Dimensions of the entity
} RenderComponent;

// Sprite component (for rendering a texture)
typedef struct {
    SDL_Texture* texture;  // Texture for the sprite
} SpriteComponent;

// Player movement component
typedef struct {
    float speed;  // Movement speed of the player
} PlayerMovementComponent;

// Player movement component
typedef struct {
    uint32_t entity;
    float speed;
} TargetMovementComponent;