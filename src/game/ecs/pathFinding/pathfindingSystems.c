#include "pathfindingSystems.h"

void init_pathfinding_component(PathfindingComponent* component, u_int32_t target) {
    component->path = NULL;
    component->path_length = 0;
    component->current_step = 0;
    component->target = target;
    component->speed = 2;
    component->last_update = SDL_GetTicks();
}
void free_pathfinding_component(PathfindingComponent* component) {
    if(!component) return;
    if (component->path) {
        free(component->path);
    }
}

void update_pathfinding_system(uint32_t id, int** grid, SDL_Rect roomPos) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    PathfindingComponent* pathfinding = ECS_GetComponent(id, PATHFINDING);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    if (!position || !pathfinding || !sprite) return;

    PositionComponent* targetPos = ECS_GetComponent(pathfinding->target, POSITION);
    SpriteComponent* targetSprite = ECS_GetComponent(pathfinding->target, SPRITE);
    if(!targetPos || !targetSprite) return;

    float staticPosX = position->x - roomPos.x;
    float staticPosY = position->y - roomPos.y;

    if(pathfinding->last_update + 1000 < SDL_GetTicks()) {
        pathfinding->last_update = SDL_GetTicks();

        int pos_x = floor(staticPosX + sprite->width / 2) / 64;
        int pos_y = floor(staticPosY + sprite->height / 2) / 64;
        if(pos_x < 0) pos_x = 0; 
        if(pos_y < 0) pos_y = 0;
        if(pos_x >= GRID_WIDTH) pos_x = GRID_WIDTH - 1; 
        if(pos_y >= GRID_HEIGHT) pos_y = GRID_HEIGHT - 1;

        int target_x = floor(targetPos->x - roomPos.x + targetSprite->width / 2) / 64;
        int target_y = floor(targetPos->y - roomPos.y + targetSprite->height / 2) / 64;
        if(target_x < 0) target_x = 0; 
        if(target_y < 0) target_y = 0;
        if(target_x >= GRID_WIDTH) target_x = GRID_WIDTH - 1; 
        if(target_y >= GRID_HEIGHT) target_y = GRID_HEIGHT - 1;

        // Free the previous path
        if (pathfinding->path) {
            free(pathfinding->path);
            pathfinding->path = NULL;
        }

        // Compute new path using A* algorithm
        a_star(pos_x, pos_y, target_x, target_y, &pathfinding->path, &pathfinding->path_length, grid);
        if(pathfinding->path_length < 2) return;

        int prevX = pathfinding->path[0]; int prevY = pathfinding->path[1];
        int dx = 0; int dy = 0;

        int j = 0;
        for(int i = 0; i < pathfinding->path_length; i++) {
            int newX = pathfinding->path[2 * i];
            int newY = pathfinding->path[2 * i + 1];

            //if(newX - prevX != dx || newY - prevY != dy) {
                pathfinding->path[2 * j] = roomPos.x + newX * 64;
                pathfinding->path[2 * j + 1] = roomPos.y + newY * 64;
                dx = newX - prevX; dy = newY - prevY;
                j++;
            //}
            prevX = newX; prevY = newY;
        }
        pathfinding->path_length = j;
        
        pathfinding->current_step = 1;
    }

    // Follow the path
    if (pathfinding->path_length > 0 && pathfinding->current_step < pathfinding->path_length) {
        //if Reached the next step, increment the step
        SDL_Rect blockHitbox = {
            pathfinding->path[pathfinding->current_step * 2], 
            pathfinding->path[pathfinding->current_step * 2 + 1], 
            64, 64
        };
        SDL_Rect hitbox = {
            position->x,
            position->y,
            sprite->width,
            sprite->height
        };
        if(SDL_HasIntersection(&blockHitbox, &hitbox)) {
            pathfinding->current_step++;
        }
    }
}
