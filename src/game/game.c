#include "game.h"

typedef struct game_s {
    Map* map;
    Room* current_room;

    uint32_t player;
} Game;

Game* game;

bool static_cam = false;
SDL_Rect cam = {
    0, 0, 0, 0 // Initialize to 0, will be set in create_game
};

void create_game(int win_width, int win_height)
{
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

    for (int i = 0; i < 30; i++) {
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

    init_timer_system();
}

void free_game()
{
    shutdown_timer_system();

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

void on_entity_created(Event event)
{
    EntityCreatedEvent* e = (EntityCreatedEvent*)event.data;
    if (!e)
        return;

    PositionComponent* pos = ECS_GetComponent(e->entity, POSITION);
    if (!pos)
        return;

    int room_x = floor(pos->x / 1920);
    int room_y = floor(pos->y / 1080);

    Room* room = get_room(game->map, room_x, room_y);
    if (!room)
        return;

    add_entity(room, e->entity);
}
void on_entity_removed(Event event)
{
    EntityRemovedEvent* rEvent = event.data;

    ChildComponent* child = ECS_GetComponent(rEvent->entity, CHILD);
    if (child) {
        uint32_t parent = child->parent;
        ParentComponent* parentComp = ECS_GetComponent(parent, PARENT);
        if (parentComp) {
            remove_child(parentComp, rEvent->entity);
        }
    }
    remove_entity(game->current_room, rEvent->entity);
}

void change_room(int x, int y)
{
    Room* r = get_room(game->map, x, y);
    if (r == NULL) {
        r = create_room(x, y);
        add_room(game->map, r);

        for (Entity e = ECS_GetFirstEntity(); e != -1; e = ECS_GetNextEntity(e)) {
            PositionComponent* position = ECS_GetComponent(e, POSITION);

            if (x == 0 && y == 0 && e == game->player) {
                continue;
            }

            if (!position)
                continue;

            if (position->camFixed || ((int)floor(position->x / 1920) == x && (int)floor(position->y / 1080) == y)) {
                add_entity(r, e);
            }
        }

        init_room(x, y, game->player);
    }
    game->current_room = r;
}

void get_keys(SDL_Event* event)
{
    handle_input_system(event, game->player);
}
void test_damage(Game* game)
{
    uint32_t nearest_enemy = get_nearest_enemy(game->player);
    static bool attacked = false;
    static bool sword_used = false;

    PositionComponent* player_pos = ECS_GetComponent(game->player, POSITION);
    SwordComponent* player_sword = ECS_GetComponent(game->player, SWORD_C);

    if (!player_sword || !player_pos)
        return;

    static int sword_counter = 0;

    if (sword_used) {
        sword_counter++;
        if (sword_counter >= 20) {
            sword_used = false;
            sword_counter = 0;
        }
    }

    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] && !sword_used) {
        SwordComponent* sword = ECS_GetComponent(game->player, SWORD_C);
        use_sword(game->player, get_nearest_enemy(game->player));
        sword_used = true;
        sword_counter = 0; // reset counter when sword is used

        if (nearest_enemy != -1 && is_colliding_with_enemy(game->player) && attacked == false) {
            attacked = true;
            if (apply_damage(nearest_enemy, game->player) == false) {
                printf("ERROR : Player not found\n");
            } else {
                printf("Player is taking damage from entity %d\n", nearest_enemy);
            }
        } else if (!is_colliding_with_enemy(game->player)) {
            attacked = false;
        }
    }
}

void update_game(int win_width, int win_height, float delta)
{
    call_events();

    update_timer_system(delta);

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
        if (!position || !sprite)
            continue;

        update_elt(
            id,
            get_entities(game->current_room),
            get_entity_amount(game->current_room),
            room_pos,
            delta);
    }

    test_damage(game);
    // is_colliding_with_item(game->player);
    is_colliding_with_chest(game->player);

    PositionComponent* pos = ECS_GetComponent(game->player, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(game->player, SPRITE);

    if (pos) {
        int changeX = floor(pos->x / 1920);
        int changeY = floor(pos->y / 1080);
        if (!static_cam) {
            cam.x = pos->x + (sprite->width - cam.w) / 2;
            cam.y = pos->y + (sprite->height - cam.h) / 2;

            // Cache room boundaries
            int room_x = get_x(game->current_room);
            int room_y = get_y(game->current_room);

            // Clamp camera position
            cam.x = fmax(room_x * 1920 + 32, fmin(cam.x, (room_x + 1) * 1920 - cam.w - 32));
            cam.y = fmax(room_y * 1080 + 32, fmin(cam.y, (room_y + 1) * 1080 - cam.h - 32));
        }

        if (changeX != get_x(game->current_room) || changeY != get_y(game->current_room)) {
            if (static_cam) {
                cam.x = changeX * 1920 + 32;
                cam.y = changeY * 1080 + 32;
            }
            change_room(changeX, changeY);
            printf("Player To cam : %f/%d - %f/%d\nRoom nb : %d - %d\n", pos->x, 1920, pos->y, 1080, changeX, changeY);
        }
    }
}

void draw_game(SDL_Renderer* renderer, int win_width, int win_height, int true_width, int true_height)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 49, 255);
    SDL_RenderClear(renderer);

    render_systems(get_entities(game->current_room), get_entity_amount(game->current_room), cam, renderer);

    draw_inventory(game->player, renderer, win_width, win_height, true_width, true_height);
    display_health(game->player, renderer);

    extern Queue* player_positions;
    for (QueueNode* node = get_first_queue_node(player_positions); node; node = get_next_queue_node(node)) {
        Vector player_pos = *(Vector*)get_data_queue_node(node);
        SDL_Rect rec = { player_pos.x - cam.x, player_pos.y - cam.y, 10, 10 };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rec);
    }

    SDL_Color color = {255, 255, 255, 255};

    SDL_RenderPresent(renderer);
}