#include "systems.h"

// Initialize the game with entities and components
void init_room(int rX, int rY, uint32_t player_id) {
    generate_room(rX, rY, player_id);
}

void free_components() {
    for(Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
        free_one_entity(e);
    }
}

void free_one_entity(uint32_t entity) {
    StateMachineComponent* sm = ECS_GetComponent(entity, STATE_MACHINE);
    if(sm) {
        free_state_machine(sm);
    }

    InventoryComponent* invent = ECS_GetComponent(entity, INVENT);
    if(invent) {
        free_inventory(invent);
    }

    free_pathfinding_component(ECS_GetComponent(entity, PATHFINDING));
    free_all_other_components(entity);
    free_all_render_components(entity);

    free_rigidbody_component(entity);

    add_removal_flag(entity);
}

void update_elt(uint32_t elt, uint32_t* entities, int amount, SDL_Rect cam, float delta) {
    StateMachineComponent* sm = ECS_GetComponent(elt, STATE_MACHINE);
    ParentComponent* parent = ECS_GetComponent(elt, PARENT);

    if(sm) {
        update_state_machine(sm);
    }
    
    update_others(elt, cam, entities, amount);
    //update_pathfinding_system(elt, roomPos);
    update_item(elt);
    update_physics(elt, entities, amount, delta);
    
    update_anim(elt);
}

// Render all entities
void render_entity(uint32_t id, SDL_Rect cam, SDL_Renderer* renderer) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    if(!position || !sprite) return;

    if(!(position->x + sprite->width >= cam.x &&
    position->x <= cam.x + cam.w &&
    position->y + sprite->height >= cam.y &&
    position->y <= cam.y + cam.h)) return;

    PathfindingComponent* targetComp = ECS_GetComponent(id, PATHFINDING);
    if(targetComp) {
        PositionComponent* targetPos = ECS_GetComponent(targetComp->target, POSITION);
        if(targetPos) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for(int j = 0; j < targetComp->path_length - 1; j++) {
                SDL_RenderDrawRect(renderer, &(SDL_Rect) {
                    targetComp->path[2 * j] - cam.x + 16,
                    targetComp->path[2 * j + 1] - cam.y + 16,
                    32, 32
                });
            }
        }
    }
    
    render_component(id, cam, renderer);
}
