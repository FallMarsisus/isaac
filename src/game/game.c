#include "game.h"

typedef struct game_s {
    Map* map;
    Room* current_room;
    
    uint32_t player;
} Game;

Game* game;

bool static_cam = false;
SDL_Rect cam = {
    0, 0, 1280, 720
};

void create_game() {
    game = malloc(sizeof(Game));

    init_event_system();

    ECS_CreateManager(40);
    game->player = add_player(640, 360);

    game->map = create_map();
    change_room(0, 0);

    register_listener(EVENT_PLAYER_MOVED, on_player_move);
    register_listener(EVENT_CHEST_OPENED, on_chest_open);
    register_listener(EVENT_STATE_CHANGE, on_state_change);
    register_listener(EVENT_COLLISION, on_collision);
    register_listener(EVENT_ENTITY_CREATED, on_entity_created);
    //register_listener(EVENT_ENTITY_REMOVED, on_entity_removed);
}

void free_game() {
    unregister_listener(EVENT_PLAYER_MOVED, on_player_move);
    unregister_listener(EVENT_CHEST_OPENED, on_chest_open);
    unregister_listener(EVENT_STATE_CHANGE, on_state_change);
    unregister_listener(EVENT_COLLISION, on_collision);
    unregister_listener(EVENT_ENTITY_CREATED, on_entity_created);
    //unregister_listener(EVENT_ENTITY_REMOVED, on_entity_removed);

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

    if(pos->camFixed || 
    ((int) floor(pos->x / cam.w) == get_x(game->current_room) && (int) floor(pos->y / cam.h) == get_y(game->current_room))) {
        if(!child) {
            add_entity(game->current_room, e->entity);
        }

        int pos_x = floor(pos->x - get_x(game->current_room) * 1280) / 64;
        int pos_y = floor(pos->y - get_y(game->current_room) * 720) / 64;
        if(pos_x < 0 || pos_y < 0 || pos_x >= get_grid_width(game->current_room) || pos_y >= get_grid_height(game->current_room)) {
            return;
        }

        if(body && !body->is_dynamic) {
            get_grid(game->current_room)[pos_y][pos_x] = 1;
        }
    }
}
void on_entity_removed(Event event) {
    EntityRemovedEvent* rEvent = event.data;

    //remove_entity(game->current_room, rEvent->entity);
    //onEntityRemove(event);
}

void change_room(int x, int y) {
    Room* r = get_room(game->map, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(game->map, r);
        init_room(x, y, game->player);

        if(x != 0 || y != 0) {
            add_entity(r, game->player);
        }
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

    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
        if (!sword_used) {
            SwordComponent* sword = ECS_GetComponent(game->player, SWORD_C);
            tempSword = use_sword(game->player, get_nearest_enemy(game->player));
            sword_used = true;
            sword_counter = 0; // reset counter when sword is used
        } else {
            sword_counter++;
            if (sword_counter >= 0.1) {
                sword_used = false;
                sword_counter = 0;
            }
        }
    } else {
        sword_used = false;
        sword_counter = 0;
    }
}

void update_game(int win_width, int win_height, float delta) {
    call_events();

    SDL_Rect room_pos = {
        get_x(game->current_room) * cam.w,
        get_y(game->current_room) * cam.h,
        cam.w,
        cam.y
    };

    for (int i = 0; i < get_entity_amount(game->current_room); i++) {
        u_int32_t id = get_entities(game->current_room)[i];
        PositionComponent* position = ECS_GetComponent(id, POSITION);
        SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
        if(!position || !sprite) continue;

        if(!(position->x + sprite->width >= cam.x &&
        position->x <= cam.x + cam.w &&
        position->y + sprite->height >= cam.y &&
        position->y <= cam.y + cam.h)) continue;

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
            change_room(changeX, changeY);
            printf("Player To cam : %f/%d - %f/%d\nRoom nb : %d - %d\n", pos->x, cam.w, pos->y, cam.h, changeX, changeY);
        }
    }
}

void draw_game(SDL_Renderer* renderer) {
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