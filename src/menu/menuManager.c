#include "menuManager.h"


void init_menu_manager(SDL_Window* win, SDL_Renderer* renderer) {
    create_game(win, renderer);
}

void free_menu_manager() {
    free_game();
}

void update_menu_manager(int win_width, int win_height, float delta) {
    update_game(win_width, win_height, delta);
}

void draw_menu_manager(SDL_Renderer* renderer, int win_width, int win_height, int true_width, int true_height) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_game(renderer, win_width, win_height, true_width, true_height);
}

void handle_menu_manager_input(SDL_Event event) {
    
}

void reset_game() {
    free_game();  // Free existing game resources
    
    SDL_Window* window = SDL_GetWindowFromID(1);
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    
    create_game(window, renderer);
}