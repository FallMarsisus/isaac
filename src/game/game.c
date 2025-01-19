#include "game.h"

SDL_Rect cam = {
    0, 0, 640, 360
};

typedef struct game_s {
    Map* map;
    Room* current_room;

    ECS_Manager* ecs;
    uint32_t player;
} Game;

Game* create_game() {
    Game* game = malloc(sizeof(Game));
    game->map = create_map();

    change_room(game, 0, 0);

    game->ecs = ECS_CreateManager(10);
    game->player = initialize_game(game->ecs);

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

void change_room(Game* game, int x, int y) {
    Room* r = get_room(game->map, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(game->map, r);

        
    }
    game->current_room = r;
}

void get_keys(Game* game, SDL_Event* event) {
    handle_input_system(game->ecs, event);
}

void update_game(Game* game, int win_width, int win_height, float delta) {
    update_systems(game->ecs, cam);

    PositionComponent* pos = ECS_GetComponent(game->ecs, game->player, POSITION);
    if(pos) {
        int changeX = floor(pos->x / cam.w);
        int changeY = floor(pos->y / cam.h);
        if(changeX != get_x(game->current_room) || changeY != get_y(game->current_room)) {
            change_room(game, changeX, changeY);
            cam.x = changeX * cam.w;
            cam.y = changeY * cam.h;
            printf("Player To cam : %f/%d - %f/%d\nRoom nb : %d - %d\n", pos->x, cam.w, pos->y, cam.h, changeX, changeY);
        }
    }
}

void draw_game(SDL_Renderer* ren, Game* game) {
    render_systems(game->ecs, cam, ren);
}