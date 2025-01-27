#include "game.h"

bool static_cam = true;
SDL_Rect cam = {
    32, 32, 1280, 720
};

int grid_width = (int) ceil(1280 / 64);
int grid_height = (int) ceil(720 / 64);

typedef struct game_s {
    Map* map;
    Room* current_room;
    
    uint32_t player;
} Game;

Game* create_game() {
    Game* game = malloc(sizeof(Game));

    init_event_system();

    ECS_CreateManager(30);
    game->player = initialize_game();

    game->map = create_map();
    change_room(game, 0, 0);

    register_listener(EVENT_PLAYER_MOVED, on_player_move);
    register_listener(EVENT_CHEST_OPENED, on_chest_open);
    register_listener(EVENT_STATE_CHANGE, on_state_change);

    return game;
}
void free_game(Game* game) {
    unregister_listener(EVENT_PLAYER_MOVED, on_player_move);
    unregister_listener(EVENT_CHEST_OPENED, on_chest_open);
    unregister_listener(EVENT_STATE_CHANGE, on_state_change);

    free_map(game->map);
    
    free_components();
    ECS_DestroyManager();

    free_event_system();

    free(game);
}

void change_room(Game* game, int x, int y) {
    Room* r = get_room(game->map, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(game->map, r);

        for(int i = 0; i < ECS_GetManager()->count; i++) {
            if(ECS_GetManager()->entity_ids[i] == game->player) continue;
            PositionComponent* position = ECS_GetComponent(ECS_GetManager()->entity_ids[i], POSITION);
            ChildComponent* child = ECS_GetComponent(ECS_GetManager()->entity_ids[i], CHILD);
            RigidbodyComponent* body = ECS_GetComponent(ECS_GetManager()->entity_ids[i], BODY);

            if (position == NULL) {
                continue;
            }

            if(position->camFixed || 
               ((int) floor(position->x / cam.w) == x && (int) floor(position->y / cam.h) == y)) {
                if(!child) {
                    add_entity(r, ECS_GetManager()->entity_ids[i]);
                }

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
    handle_input_system(event);
}
void test_damage(Game* game) {
    uint32_t nearest_enemy = get_nearest_enemy(game->player);
    static bool attacked = false;
    if(nearest_enemy != UINT32_MAX && is_colliding_with_enemy(game->player) && attacked == false) {
        attacked = true;
        if(apply_damage(nearest_enemy, game->player) == false) {
            printf("ERROR : Player not found\n");
        } else {
            printf("Player is taking damage from entity %d\n", nearest_enemy);
        }
    }
    else if (!is_colliding_with_enemy(game->player)) {
        attacked = false;
    }
    
}

void update_game(Game* game, int win_width, int win_height, float delta) {
    call_events();

    uint32_t* entities = get_entities(game->current_room);
    for(int i = 0; i < get_entity_amount(game->current_room); i++) {

        update_elt(
            entities[i],
            get_grid(game->current_room),
            cam,
            delta
        );

        // creating another to avoid polluting default function with too many args and return
        if (ECS_GetComponent(entities[i], ITEM) != NULL && update_item(entities[i], entities, get_entity_amount(game->current_room))) {
            free_one_entity(entities[i]);
            ECS_RemoveEntity(entities[i]);
            remove_entity(game->current_room, entities[i]);
            entities = get_entities(game->current_room);

            fflush(stdout);
            i--;
        }
    }

    update_elt(
        game->player,
        get_grid(game->current_room),
        cam,
        delta
    );
    
    test_damage(game);
    is_colliding_with_chest(game->player);

    PositionComponent* pos = ECS_GetComponent(game->player, POSITION);
    if(pos) {
        int changeX = floor(pos->x / cam.w);
        int changeY = floor(pos->y / cam.h);
        if(!static_cam) {
            cam.x = pos->x - cam.w / 2;
            cam.y = pos->y - cam.h / 2;
        }
        if(changeX != get_x(game->current_room) || changeY != get_y(game->current_room)) {
            if(static_cam) {
                cam.x = changeX * cam.w + 32;
                cam.y = changeY * cam.h + 32;
            }
            change_room(game, changeX, changeY);
            printf("Player To cam : %f/%d - %f/%d\nRoom nb : %d - %d\n", pos->x, cam.w, pos->y, cam.h, changeX, changeY);
        }
    }
}

void draw_game(SDL_Renderer* renderer, Game* game) {
    SDL_SetRenderDrawColor(renderer, 37, 37, 49, 255);
    SDL_RenderClear(renderer);

    render_systems(get_entities(game->current_room), get_entity_amount(game->current_room), cam, renderer);
    
    draw_inventory(game->player, renderer);
    display_health(game->player, renderer);

    SDL_RenderPresent(renderer);
}