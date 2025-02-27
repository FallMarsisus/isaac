#include "menuManager.h"

static MenuComponent mainMenu;
static MenuComponent gameOverMenu;
static MenuComponent* currentMenu = NULL;
static SDL_Window* window;
static SDL_Renderer* rendererRef;

void start_game() {
    currentMenu = NULL;
    
    free_game();
    create_game(window, rendererRef);
}

void quit_game() {
    SDL_Event quit;
    quit.type = SDL_QUIT;
    SDL_PushEvent(&quit);
}

void return_to_main_menu() {
    switch_to_menu(MENU_MAIN);
}

void init_menu_manager(SDL_Window* win, SDL_Renderer* renderer) {
    window = win;
    rendererRef = renderer;

    init_menu_component(&mainMenu, MENU_MAIN);
    add_menu_item(&mainMenu, "Start Game", start_game);
    add_menu_item(&mainMenu, "Quit", quit_game);

    init_menu_component(&gameOverMenu, MENU_GAME_OVER);
    add_menu_item(&gameOverMenu, "Try Again", start_game);
    add_menu_item(&gameOverMenu, "Main Menu", return_to_main_menu);
    add_menu_item(&gameOverMenu, "Quit", quit_game);

    currentMenu = &mainMenu;

    register_listener(EVENT_GAME_OVER, on_game_over);
}

void free_menu_manager() {
    unregister_listener(EVENT_GAME_OVER, on_game_over);

    free_game();

    free_menu_component(&mainMenu);
    free_menu_component(&gameOverMenu);
}

void update_menu_manager(int win_width, int win_height, float delta) {
    if (!currentMenu) {
        update_game(win_width, win_height, delta);
    }
}

void draw_menu_manager(SDL_Renderer* renderer, int win_width, int win_height, int true_width, int true_height) {
    if (currentMenu) {
        draw_menu(currentMenu, renderer, win_width, win_height);
    } else {
        draw_game(renderer, win_width, win_height, true_width, true_height);
    }
}

void handle_menu_manager_input(SDL_Event event) {
    if (currentMenu) {
        handle_menu_input(currentMenu, event);
    }
}

void switch_to_menu(MenuType menuType) {
    switch (menuType) {
        case MENU_MAIN:
            currentMenu = &mainMenu;
            break;
        case MENU_GAME_OVER:
            currentMenu = &gameOverMenu;
            break;
        case MENU_NONE:
            currentMenu = NULL;
            break;
    }
}

void on_game_over(Event event) {
    switch_to_menu(MENU_GAME_OVER);
}