#include "game.h"

typedef struct game_s {
    Map* map;
    Room* current_room;
    
    uint32_t player;
} Game;

Game* game;

bool static_cam = false;
SDL_Rect cam = {
    0, 0, 0, 0  // Initialize to 0, will be set in create_game
};

void create_game(int win_width, int win_height) {
    game = malloc(sizeof(Game));

    // Set camera dimensions based on window size
    cam.x = 32;
    cam.y = 32;
    cam.w = win_width;
    cam.h = win_height;

    init_event_system();

    init_player_positions(game->player);

    ECS_CreateManager(40);
    game->player = add_player(640, 360);

    game->map = create_map();
    change_room(0, 0);

    for(int i = 0; i < 30; i++) {
        float x1 = random_int(-5000, 5000), y1 = random_int(-5000, 5000), 
              x2 = random_int(-5000, 5000), y2 = random_int(-5000, 5000);
        add_teleporter(x1, y1, x2, y2);
        add_teleporter(x2, y2, x1, y1);
    }

    register_listener(EVENT_PLAYER_MOVED, on_player_move);
    register_listener(EVENT_CHEST_OPENED, on_chest_open);
    register_listener(EVENT_STATE_CHANGE, on_state_change);
    register_listener(EVENT_COLLISION, on_collision);
    register_listener(EVENT_ENTITY_CREATED, on_entity_created);
    register_listener(EVENT_ENTITY_REMOVED, on_entity_removed);
}

void free_game() {
    unregister_listener(EVENT_PLAYER_MOVED, on_player_move);
    unregister_listener(EVENT_CHEST_OPENED, on_chest_open);
    unregister_listener(EVENT_STATE_CHANGE, on_state_change);
    unregister_listener(EVENT_COLLISION, on_collision);
    unregister_listener(EVENT_ENTITY_CREATED, on_entity_created);
    unregister_listener(EVENT_ENTITY_REMOVED, on_entity_removed);

    free_map(game->map);
    
    free_components();
    ECS_DestroyManager();

    free_event_system();

    free(game);
}

void on_entity_created(Event event) {
    EntityCreatedEvent* e = (EntityCreatedEvent*) event.data;
    if(!e) return;

    PositionComponent* pos = ECS_GetComponent(e->entity, POSITION);
    ChildComponent* child = ECS_GetComponent(e->entity, CHILD);
    RigidbodyComponent* body = ECS_GetComponent(e->entity, BODY);
    if(!pos) return;

    int room_x = floor(pos->x / 1920); int room_y = floor(pos->y / 1080);

    Room* room = get_room(game->map, room_x, room_y);
    if(!room) return;

    int pos_x = (pos->x - get_x(room) * 1920) / 64;
    int pos_y = (pos->y - get_y(room) * 1080) / 64;

    add_entity(room, e->entity);
    
    if(body && !body->is_dynamic && (pos_x >= 0 && pos_y >= 0 && pos_x < get_grid_width(room) && pos_y < get_grid_height(room))) {
        get_grid(room)[pos_y][pos_x] = 1;
    }
}
void on_entity_removed(Event event) {
    EntityRemovedEvent* rEvent = event.data;

    remove_entity(game->current_room, rEvent->entity);
}

void change_room(int x, int y) {
    Room* r = get_room(game->map, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(game->map, r);

        for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
            PositionComponent* position = ECS_GetComponent(e, POSITION);
            ChildComponent* child = ECS_GetComponent(e, CHILD);
            RigidbodyComponent* body = ECS_GetComponent(e, BODY);

            if(x == 0 && y == 0 && e == game->player) {
                continue;
            }

            if (!position) continue;

            if(position->camFixed || 
            ((int) floor(position->x / 1920) == x && (int) floor(position->y / 1080) == y)) {
                if(!child) {
                    add_entity(r, e);
                }

                int pos_x = floor(position->x - get_x(r) * 1920) / 64;
                int pos_y = floor(position->y - get_y(r) * 1080) / 64;
                if(pos_x < 0 || pos_y < 0 || pos_x >= get_grid_width(r) || pos_y >= get_grid_height(r)) continue;

                if(body && !body->is_dynamic) {
                    get_grid(r)[pos_y][pos_x] = 1;
                }
            }
        }

        init_room(x, y, game->player);
    }
    game->current_room = r;
}

void get_keys(SDL_Event* event) {
    handle_input_system(event, game->player);
}
void test_damage(Game* game) {
    uint32_t nearest_enemy = get_nearest_enemy(game->player);
    static bool attacked = false;
    static bool sword_used = false;
    static uint32_t tempSword = SDL_MAX_UINT32;

    PositionComponent* player_pos = ECS_GetComponent(game->player, POSITION);
    SwordComponent* player_sword = ECS_GetComponent(game->player, SWORD_C);
    if(nearest_enemy != -1 && is_colliding_with_enemy(game->player) && attacked == false) {
        attacked = true;
        if(apply_damage(nearest_enemy, game->player) == false) {
            printf("ERROR : Player not found\n");
        } else {
            printf("Player is taking damage from entity %d\n", nearest_enemy);
        }
    } else if (!is_colliding_with_enemy(game->player)) {
        attacked = false;
    }
    static int sword_counter = 0;
    static float initial_direction_x = 0;
    static float initial_direction_y = 0;

    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
        if (!sword_used) {
            SwordComponent* sword = ECS_GetComponent(game->player, SWORD_C);
            tempSword = use_sword(game->player, get_nearest_enemy(game->player));
            sword_used = true;
            sword_counter = 0; // reset counter when sword is used
            initial_direction_x = player_pos->vx;
            initial_direction_y = player_pos->vy;
        } else {
            sword_counter++;
            if (sword_counter >= 35 || (player_pos->vx != initial_direction_x || player_pos->vy != initial_direction_y)) {
                sword_used = false;
                ECS_RemoveEntity(tempSword);
                sword_counter = 0;
            }
        }
    } else {
        sword_counter++;
        if (sword_counter >= 35 || (player_pos->vx != initial_direction_x || player_pos->vy != initial_direction_y)) {
            sword_used = false;
            ECS_RemoveEntity(tempSword);
            sword_counter = 0;
        }
    }

    // Update sword position relative to player as child system doesnt work with animations
    if (tempSword != SDL_MAX_UINT32) {
        PositionComponent* sword_pos = ECS_GetComponent(tempSword, POSITION);
        
        if (sword_pos && player_pos) {
            if (player_pos->vx >= 1 && player_pos->vy == 0) {
            sword_pos->x = player_pos->x + 50; // Right
            sword_pos->y = player_pos->y;
            } else if (player_pos->vx <= -1 && player_pos->vy == 0) {
            sword_pos->x = player_pos->x - 50; // Left
            sword_pos->y = player_pos->y;
            } else if (player_pos->vx == 0 && player_pos->vy >= 1) {
            sword_pos->x = player_pos->x;
            sword_pos->y = player_pos->y + 50; // Down
            } else if (player_pos->vx == 0 && player_pos->vy <= -1) {
            sword_pos->x = player_pos->x;
            sword_pos->y = player_pos->y - 50; // Up
            } else if (player_pos->vx >= 1 && player_pos->vy >= 1) {
            sword_pos->x = player_pos->x + 50;
            sword_pos->y = player_pos->y + 50; // Down-Right
            } else if (player_pos->vx <= -1 && player_pos->vy <= -1) {
            sword_pos->x = player_pos->x - 50;
            sword_pos->y = player_pos->y - 50; // Up-Left
            } else if (player_pos->vx >= 1 && player_pos->vy <= -1) {
            sword_pos->x = player_pos->x + 50;
            sword_pos->y = player_pos->y - 50; // Up-Right
            } else if (player_pos->vx <= -1 && player_pos->vy >= 1) {
            sword_pos->x = player_pos->x - 50;
            sword_pos->y = player_pos->y + 50; // Down-Left
            }
        }
    }
}

void update_game(int win_width, int win_height, float delta) {
    call_events();

    SDL_Rect room_pos = {
        get_x(game->current_room) * 1920,
        get_y(game->current_room) * 1080,
        1920,
        1080
    };

    update_player_positions(game->player);

    for (int i = 0; i < get_entity_amount(game->current_room); i++) {
        u_int32_t id = get_entities(game->current_room)[i];
        PositionComponent* position = ECS_GetComponent(id, POSITION);
        SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
        if(!position || !sprite) continue;
        
        update_elt(
            id,
            get_grid(game->current_room),
            get_entities(game->current_room),
            get_entity_amount(game->current_room),
            room_pos,
            delta
        );
    }
    
    test_damage(game);
    // is_colliding_with_item(game->player);
    is_colliding_with_chest(game->player);
    
    PositionComponent* pos = ECS_GetComponent(game->player, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(game->player, SPRITE);

    if(pos) {
        int changeX = floor(pos->x / 1920);
        int changeY = floor(pos->y / 1080);
        if(!static_cam) {
            cam.x = pos->x + (sprite->width - cam.w) / 2;
            cam.y = pos->y + (sprite->height - cam.h) / 2;

            // Cache room boundaries
            int room_x = get_x(game->current_room);
            int room_y = get_y(game->current_room);
            
            // Clamp camera position
            cam.x = fmax(room_x * 1920 + 32, fmin(cam.x, (room_x + 1) * 1920 - cam.w - 32));
            cam.y = fmax(room_y * 1080 + 32, fmin(cam.y, (room_y + 1) * 1080 - cam.h - 32));
        }

        if(changeX != get_x(game->current_room) || changeY != get_y(game->current_room)) {
            if(static_cam) {
                cam.x = changeX * 1920 + 32;
                cam.y = changeY * 1080 + 32;
            }
            change_room(changeX, changeY);
            printf("Player To cam : %f/%d - %f/%d\nRoom nb : %d - %d\n", pos->x, 1920, pos->y, 1080, changeX, changeY);
        }
    }
}

void draw_game(SDL_Renderer* renderer, int win_width, int win_height, int true_width , int true_height) {
    SDL_SetRenderDrawColor(renderer, 37, 37, 49, 255);
    SDL_RenderClear(renderer);

    render_systems(get_entities(game->current_room), get_entity_amount(game->current_room), cam, renderer);
    
    draw_inventory(game->player, renderer, win_width, win_height, true_width , true_height);
    display_health(game->player, renderer);

    SDL_Rect rec = {0, 0, 8, 8};
    int offsetX = get_x(game->current_room) * 1920 - cam.x + 28;
    int offsetY = get_y(game->current_room) * 1080 - cam.y + 28;
    for(rec.x = offsetX; rec.x < offsetX + get_grid_width(game->current_room) * 64; rec.x += 64) {
        for(rec.y = offsetY; rec.y < offsetY + get_grid_height(game->current_room) * 64; rec.y += 64) {
            switch(get_grid(game->current_room)[(rec.y - offsetY) / 64][(rec.x - offsetX) / 64]) {
                case 1 : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
                default : SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
            }
            SDL_RenderFillRect(renderer, &rec);
        }
    }

    extern Queue* player_positions;
    for(QueueNode* node = get_first_queue_node(player_positions); node; node = get_next_queue_node(node)) {
        Vector player_pos = *(Vector*) get_data_queue_node(node);
        SDL_Rect rec = {player_pos.x - cam.x, player_pos.y - cam.y, 10, 10};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rec);
    }

	// SDL_Color color = {255, 255, 255, 255};
	// displayText("abcdefg hijklmnopqrstuvwxyz", renderer, getFonts()->calibri, &color, 10, 50, 10);
	// displayText("lll", renderer, getFonts()->calibri, &color, 10, 70, 10);

    SDL_RenderPresent(renderer);
}