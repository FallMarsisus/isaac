#include "game.h"

bool static_cam = false;
SDL_Rect cam = {
    0, 0, 1280, 720
};

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
    register_listener(EVENT_COLLISION, on_collision);
    
    return game;
}
void free_game(Game* game) {
    unregister_listener(EVENT_PLAYER_MOVED, on_player_move);
    unregister_listener(EVENT_CHEST_OPENED, on_chest_open);
    unregister_listener(EVENT_STATE_CHANGE, on_state_change);
    register_listener(EVENT_COLLISION, on_collision);

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

        for (int i = 0; i < ECS_GetManager()->st->dict->capacity; i++) {
            Node* current = ECS_GetManager()->st->dict->array[i];
            while (current) {
                if(current->key == game->player) {
                    current = current->next;
                    continue;
                }
                PositionComponent* position = ECS_GetComponent(current->key, POSITION);
                ChildComponent* child = ECS_GetComponent(current->key, CHILD);
                RigidbodyComponent* body = ECS_GetComponent(current->key, BODY);

                if (position == NULL) {
                    current = current->next;
                    continue;
                }

                if(position->camFixed || 
                ((int) floor(position->x / cam.w) == x && (int) floor(position->y / cam.h) == y)) {
                    if(!child) {
                        add_entity(r, current->key);
                    }

                    int pos_x = floor(position->x - get_x(r) * 1280) / 64;
                    int pos_y = floor(position->y - get_y(r) * 720) / 64;
                    if(pos_x < 0 || pos_y < 0 || pos_x >= get_grid_width(r) || pos_y >= get_grid_height(r)) {
                        current = current->next;
                        continue;
                    }

                    if(body && !body->is_dynamic) {
                        get_grid(r)[pos_y][pos_x] = 1;
                    }
                }
                current = current->next;
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
    if(nearest_enemy != -1 && is_colliding_with_enemy(game->player) && attacked == false) {
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
    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
        // Gestion des dégâts faits par le joueur en utilisant use_sword
        SwordComponent* sword = ECS_GetComponent(game->player, SWORD_C);
        use_sword(game->player, get_nearest_enemy(game->player));
    }
}

void update_game(Game* game, int win_width, int win_height, float delta) {
    call_events();

    SDL_Rect room_pos = {
        get_x(game->current_room) * cam.w,
        get_y(game->current_room) * cam.h,
        cam.w,
        cam.y
    };

    uint32_t* entities = get_entities(game->current_room);
    for(int i = 0; i < get_entity_amount(game->current_room); i++) {
        update_elt(
            entities[i],
            get_grid(game->current_room),
            get_entities(game->current_room),
            get_entity_amount(game->current_room),
            room_pos,
            delta
        );
    }

    update_elt(
        game->player,
        get_grid(game->current_room),
        get_entities(game->current_room),
        get_entity_amount(game->current_room),
        room_pos,
        delta
    );
    
    test_damage(game);
    // is_colliding_with_item(game->player);
    is_colliding_with_chest(game->player);
    
    PositionComponent* pos = ECS_GetComponent(game->player, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(game->player, SPRITE);

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

    render_systems(get_entities(game->current_room), get_entity_amount(game->current_room), cam, renderer);
    
    draw_inventory(game->player, renderer);
    display_health(game->player, renderer);

    SDL_Rect rec = {0, 0, 8, 8};
    int offsetX = get_x(game->current_room) * 1280 - cam.x + 28;
    int offsetY = get_y(game->current_room) * 720 - cam.y + 28;
    for(rec.x = offsetX; rec.x < offsetX + get_grid_width(game->current_room) * 64; rec.x += 64) {
        for(rec.y = offsetY; rec.y < offsetY + get_grid_height(game->current_room) * 64; rec.y += 64) {
            switch(get_grid(game->current_room)[(rec.y - offsetY) / 64][(rec.x - offsetX) / 64]) {
                case 1 : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
                default : SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
            }
            SDL_RenderFillRect(renderer, &rec);
        }
    }

    SDL_RenderPresent(renderer);
}