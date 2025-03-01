#ifndef COMPONENTS_H
#define COMPONENTS_H

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
    TILE,
    STUN  // Nouvelle composante
} ComponentType;

// Position and velocity component
typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    bool camFixed;
} PositionComponent;

typedef struct {
	SDL_Rect hitbox;
    int layer; // 0 = tiles, 1 = ennemy, 2 = player; 3 = others
    ID_array* layer_that_collides_with;

    bool is_dynamic;
    float friction;
    float restitution;
    float mass;
    float forceX;
    float forceY;
	dyn_array* forces;

    bool colliding;
} RigidbodyComponent;

typedef struct {
    int width, height; // Dimensions of the entity on screen
    int layer;

    float angle;
    SDL_Point* center; // center of rotation
    SDL_RendererFlip flip;

    SDL_Texture* texture;
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
    int* path;
    int path_length;
    int current_step;
    uint32_t target;

    float speed;
    int last_update;
} PathfindingComponent;

typedef struct {
    void* data;

    void (*update)(uint32_t entity, SDL_Rect cam, uint32_t* entities, int amount);
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

    bool has_physics;
} EffectComponent;

typedef struct {
    int duration;  // Durée du stun en millisecondes
    int start_time;  // Temps de début du stun
} StunComponent;


#endif