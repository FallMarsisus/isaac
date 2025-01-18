#include "game.h"

typedef struct game_s {
    Map* map;
    Room* current_room;
    int coord_x; int coord_y;

    ECS_Manager* ecs;

    chained_list* current_entities;
} Game;

Game* create_game() {
    Game* game = malloc(sizeof(Game));
    game->map = create_map();

    game->coord_x = 0; game->coord_y = 0;
    change_room(game, game->coord_x, game->coord_y);

    game->ecs = ECS_CreateManager();
    initialize_game(game->ecs);

    return game;
}
void free_game(Game* game) {
    free_map(game->map);

    ECS_DestroyManager(game->ecs);

    free(game);
}

void load_assets(Game* game) {
}

void change_room(Game* game, int x, int y) {
    Room* r = get_room(game->map, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(game->map, r);
    }
    game->current_room = r;
    game->current_entities = get_entities(r);
    game->coord_x = x;
    game->coord_y = y;
}

void get_keys(Game* game, SDL_Event* event) {
    handle_input_system(game->ecs, event);
}

void update_game(Game* game, int win_width, int win_height, float delta) {
    update_systems(game->ecs);
}

void draw_game(SDL_Renderer* ren, Game* game) {
    render_systems(game->ecs, ren);
}