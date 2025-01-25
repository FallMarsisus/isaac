#include "game.h"

bool static_cam = true;
SDL_Rect cam = {
    0, 0, 1280, 720
};

int grid_width = (int) ceil(1280 / 64);
int grid_height = (int) ceil(720 / 64);

typedef struct game_s {
    Map* map;
    Room* current_room;

    ECS_Manager* ecs;
    uint32_t player;
} Game;

Game* create_game() {
    Game* game = malloc(sizeof(Game));
    game->ecs = ECS_CreateManager(30);
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
        for(int i = 0; i < get_ids_len(parent->children); i++) {
            uint32_t id = get_ids(parent->children)[i];
            add_entity(r, game->ecs->entity_ids[id]);

            add_children_to_room(game, r, id);
        }
    }
}

void change_room(Game* game, int x, int y) {
    Room* r = get_room(game->map, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(game->map, r);
        add_entity(r, game->player);
        add_children_to_room(game, r, game->player);

        for(int i = 0; i < game->ecs->count; i++) {
            if(game->ecs->entity_ids[i] == game->player) continue;
            PositionComponent* position = ECS_GetComponent(game->ecs, game->ecs->entity_ids[i], POSITION);
            ChildComponent* child = ECS_GetComponent(game->ecs, game->ecs->entity_ids[i], CHILD);
            RigidbodyComponent* body = ECS_GetComponent(game->ecs, game->ecs->entity_ids[i], BODY);
            if(child) continue;

            if(position->camFixed || 
               ((int) floor(position->x / cam.w) == x && (int) floor(position->y / cam.h) == y)) {
                add_entity(r, game->ecs->entity_ids[i]);
                add_children_to_room(game, r, game->ecs->entity_ids[i]);

                int pos_x = (int) (position->x - cam.x) / 64;
                int pos_y = (int) (position->y - cam.y) / 64;
                if(pos_x < 0) pos_x = 0;
                if(pos_y < 0) pos_y = 0;
                if(pos_x >= get_grid_width(r)) pos_x = get_grid_width(r) - 1;
                if(pos_y >= get_grid_height(r)) pos_y = get_grid_height(r) - 1;

                if(body && !body->is_dynamic) {
                    get_grid(r)[pos_y][pos_x] = 1;
                }
            }
        }
    }
    game->current_room = r;
}

void get_keys(Game* game, SDL_Event* event) {
    handle_input_system(game->ecs, event);
}

void test_damage(Game* game) {
    uint32_t nearest_enemy = get_nearest_enemy(game->ecs, game->player);
    static bool attacked = false;
    if(nearest_enemy != UINT32_MAX && is_colliding_with_enemy(game->ecs, game->player) && attacked == false) {
        attacked = true;
        if(apply_damage(game->ecs, nearest_enemy, game->player) == false) {
            printf("ERROR : Player not found\n");
        } else {
            printf("Player is taking damage from entity %d\n", nearest_enemy);
        }
    }
    else if (!is_colliding_with_enemy(game->ecs, game->player)) {
        attacked = false;
    }
    
}

void update_game(Game* game, int win_width, int win_height, float delta) {
    update_systems(
        game->ecs, 
        get_entities(game->current_room), 
        get_entity_amount(game->current_room), 
        get_grid(game->current_room),
        cam
    );

    

    test_damage(game);
    is_colliding_with_chest(game->ecs, game->player);

    PositionComponent* pos = ECS_GetComponent(game->ecs, game->player, POSITION);
    if(pos) {
        int changeX = floor(pos->x / cam.w);
        int changeY = floor(pos->y / cam.h);
        if(!static_cam) {
            cam.x = pos->x - cam.w / 2;
            cam.y = pos->y - cam.h / 2;
        }
        if(changeX != get_x(game->current_room) || changeY != get_y(game->current_room)) {
            if(static_cam) {
                cam.x = changeX * cam.w;
                cam.y = changeY * cam.h;
            }
            change_room(game, changeX, changeY);
            printf("Player To cam : %f/%d - %f/%d\nRoom nb : %d - %d\n", pos->x, cam.w, pos->y, cam.h, changeX, changeY);
        }
    }
}

void draw_game(SDL_Renderer* renderer, Game* game) {
    SDL_SetRenderDrawColor(renderer, 37, 37, 49, 255);
    SDL_RenderClear(renderer);

    render_systems(game->ecs, get_entities(game->current_room), get_entity_amount(game->current_room), cam, renderer);

    draw_inventory(game->ecs, game->player, renderer);
    display_health(game->ecs, game->player, renderer);


    SDL_RenderPresent(renderer);
}