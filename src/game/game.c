#include "game.h"

bool static_cam = true;
SDL_Rect cam = {
    0, 0, 1280, 720
};

typedef struct game_s {
    Map* map;
    Room* current_room;

    ECS_Manager* ecs;
    uint32_t player;
} Game;

Game* create_game() {
    Game* game = malloc(sizeof(Game));
    game->ecs = ECS_CreateManager(20);
    game->player = initialize_game(game->ecs);

    game->map = create_map();
    change_room(game, 0, 0);

    return game;
}
void free_game(Game* game) {
    free_map(game->map);
    
    free_components(game->ecs);
    ECS_DestroyManager(game->ecs);

    free(game);
}

void load_assets(Game* game) {

}

void add_children_to_room(Game* game, Room* r, uint32_t elt) {
    ParentComponent* parent = ECS_GetComponent(game->ecs, elt, PARENT);
    if(parent) {
        for(int i = 0; i < get_len(parent->children); i++) {
            uint32_t id = *((u_int32_t*) get_elt(parent->children, i));
            add_entity(r, &game->ecs->entity_ids[id]);

            add_children_to_room(game, r, id);
        }
    }
}

void change_room(Game* game, int x, int y) {
    Room* r = get_room(game->map, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(game->map, r);
        add_entity(r, &game->ecs->entity_ids[game->player]);
        add_children_to_room(game, r, game->player);

        for(int i = 0; i < game->ecs->count; i++) {
            if(game->ecs->entity_ids[i] == game->player) continue;
            PositionComponent* position = ECS_GetComponent(game->ecs, game->ecs->entity_ids[i], POSITION);
            ChildComponent* child = ECS_GetComponent(game->ecs, game->ecs->entity_ids[i], CHILD);
            if(child) continue;

            if(position->camFixed || 
               ((int) floor(position->x / cam.w) == x && (int) floor(position->y / cam.h) == y)) {
                add_entity(r, &game->ecs->entity_ids[i]);
                add_children_to_room(game, r, game->ecs->entity_ids[i]);
                
                printf("%f - %f\n", position->x, position->y);
            }
        }
    }
    game->current_room = r;
}

void get_keys(Game* game, SDL_Event* event) {
    handle_input_system(game->ecs, event);
}

void update_game(Game* game, int win_width, int win_height, float delta) {
    update_systems(game->ecs, get_entities(game->current_room), cam);

    PositionComponent* pos = ECS_GetComponent(game->ecs, game->player, POSITION);
    if(pos) {
        int changeX = floor(pos->x / cam.w);
        int changeY = floor(pos->y / cam.h);
        if(!static_cam) {
            cam.x = pos->x - cam.w / 2;
            cam.y = pos->y - cam.h / 2;
        }
        if(changeX != get_x(game->current_room) || changeY != get_y(game->current_room)) {
            change_room(game, changeX, changeY);
            if(static_cam) {
                cam.x = changeX * cam.w;
                cam.y = changeY * cam.h;
            }
            printf("Player To cam : %f/%d - %f/%d\nRoom nb : %d - %d\n", pos->x, cam.w, pos->y, cam.h, changeX, changeY);
        }
    }
}

void draw_game(SDL_Renderer* renderer, Game* game) {
    SDL_SetRenderDrawColor(renderer, 37, 37, 49, 255);
    SDL_RenderClear(renderer);

    render_systems(game->ecs, get_entities(game->current_room), cam, renderer);

    draw_inventory(game->ecs, game->player, renderer);


    SDL_RenderPresent(renderer);
}