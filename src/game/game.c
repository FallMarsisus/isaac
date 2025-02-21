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

    ECS_CreateManager();

    game->player = add_player(1920 / 2 - 32, 1280 - 64);
    init_player_positions(game->player);

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

void on_entity_created(Event event) {
    EntityCreatedEvent* e = (EntityCreatedEvent*)event.data;
    if (!e)
        return;

    PositionComponent* pos = ECS_GetComponent(e->entity, POSITION);
    if (!pos)
        return;

    int room_x = floor(pos->x / 1920);
    int room_y = floor(pos->y / 1280);

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

void change_room(int x, int y) {
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

            if(position->camFixed || 
            ((int) floor(position->x / 1920) == x && (int) floor(position->y / 1280) == y)) {
                add_entity(r, e);
            }
        }

        init_room(x, y, game->player);
    }
    game->current_room = r;
}

void test_damage(Game* game) {
}

void update_game(int win_width, int win_height, float delta)
{
    SDL_Rect room_pos = {
        get_x(game->current_room) * 1920,
        get_y(game->current_room) * 1280,
        1920,
        1280
    };
    update_timer_system(delta);
    call_events();

    update_player(game->player);
    update_player_positions(game->player);

    for (int i = 0; i < get_entity_amount(game->current_room); i++) {
        u_int32_t id = get_entities(game->current_room)[i];
        PositionComponent* position = ECS_GetComponent(id, POSITION);
        SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
        if (!position || !sprite) continue;

        update_elt(
            id,
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

    if (pos) {
        int changeX = floor(pos->x / 1920);
        int changeY = floor(pos->y / 1280);
        if(!static_cam) {
            cam.x = pos->x + (sprite->width - cam.w) / 2;
            cam.y = pos->y + (sprite->height - cam.h) / 2;

            // Cache room boundaries
            int room_x = get_x(game->current_room);
            int room_y = get_y(game->current_room);

            // Clamp camera position
            cam.x = fmax(room_x * 1920 + 32, fmin(cam.x, (room_x + 1) * 1920 - cam.w - 32));
            cam.y = fmax(room_y * 1280 + 32, fmin(cam.y, (room_y + 1) * 1280 - cam.h - 32));
        }

        if (changeX != get_x(game->current_room) || changeY != get_y(game->current_room)) {
            if (static_cam) {
                cam.x = changeX * 1920 + 32;
                cam.y = changeY * 1280 + 32;
            }
            change_room(changeX, changeY);
            printf("Player To cam : %f/%d - %f/%d\nRoom nb : %d - %d\n", pos->x, 1920, pos->y, 1280, changeX, changeY);
        }
    }

    ECS_ProcessRemovals();
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
	

    SDL_RenderPresent(renderer);
}