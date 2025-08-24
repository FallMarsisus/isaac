#include "menuManager.h"

static MenuComponent mainMenu;
static MenuComponent gameOverMenu;
static MenuComponent pauseMenu;
static MenuComponent settingsMenu;
static MenuComponent* currentMenu = NULL;

static SDL_Window* window;
static SDL_Renderer* rendererRef;

void start_game() {
    currentMenu = NULL;
    create_game(window, rendererRef);
}

void resume_game() {
    switch_to_menu(MENU_NONE);
}

void quit_game() {    
    trigger_event(EVENT_QUIT, NULL, false);
}

void return_to_main_menu() {
    switch_to_menu(MENU_MAIN);
}

void open_settings() {
    switch_to_menu(MENU_SETTINGS);
}

void toggle_audio() {
    // Placeholder for audio toggle functionality
    printf("Audio settings toggled\n");
}

void reset_game() {
    // Placeholder for reset functionality  
    printf("Game reset\n");
}

void init_menu_manager(SDL_Window* win, SDL_Renderer* renderer) {
    window = win;
    rendererRef = renderer;

    init_menu_component(&mainMenu, MENU_MAIN);
    add_menu_item(&mainMenu, "🎮 Game", start_game);
    add_menu_item(&mainMenu, "⚙️ Settings", open_settings);
    add_menu_item(&mainMenu, "❌ Quit", quit_game);

    init_menu_component(&pauseMenu, MENU_PAUSE);
    add_menu_item(&pauseMenu, "▶️ Resume", resume_game);
    add_menu_item(&pauseMenu, "🏠 Home", return_to_main_menu);
    add_menu_item(&pauseMenu, "❌ Quit", quit_game);

    init_menu_component(&gameOverMenu, MENU_GAME_OVER);
    add_menu_item(&gameOverMenu, "🔄 Retry", start_game);
    add_menu_item(&gameOverMenu, "🏠 Home", return_to_main_menu);
    add_menu_item(&gameOverMenu, "❌ Quit", quit_game);

    init_menu_component(&settingsMenu, MENU_SETTINGS);
    add_menu_item(&settingsMenu, "🔊 Audio", toggle_audio);
    add_menu_item(&settingsMenu, "🔄 Reset", reset_game);
    add_menu_item(&settingsMenu, "🔙 Back", return_to_main_menu);

    currentMenu = &mainMenu;

    register_listener(EVENT_GAME_OVER, on_game_over);
}

void free_menu_manager() {
    unregister_listener(EVENT_GAME_OVER, on_game_over);

    free_game();

    free_menu_component(&mainMenu);
    free_menu_component(&pauseMenu);
    free_menu_component(&gameOverMenu);
    free_menu_component(&settingsMenu);
}

void update_menu_manager(float delta) {
    call_events();

    int win_width, win_height;
    SDL_RenderGetLogicalSize(get_renderer(), &win_width, &win_height);

    if (!currentMenu) {
        update_game(win_width, win_height, delta);
    }

    ECS_ProcessRemovals();
}

void draw_menu_manager(SDL_Renderer* renderer, int true_width, int true_height) {
    int win_width, win_height;
    SDL_RenderGetLogicalSize(renderer, &win_width, &win_height);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (!currentMenu || currentMenu->type == MENU_PAUSE) {
        draw_game(renderer, win_width, win_height, true_width, true_height);
    }
    if (currentMenu) {
        draw_menu(currentMenu, renderer, win_width, win_height);
    }
}

void handle_menu_manager_input(SDL_Event event) {
    if (currentMenu) {
        handle_menu_input(currentMenu, event);
    }
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_k) {
        explode_room(1, 1);
    }
}

void switch_to_menu(MenuType menuType) {
    if(!currentMenu && menuType != MENU_PAUSE) {
        free_game();
    }
    switch (menuType) {
        case MENU_MAIN:
            if(currentMenu == &pauseMenu) {
                free_game();
            }
            currentMenu = &mainMenu;
            break;
        case MENU_PAUSE:
            currentMenu = &pauseMenu;
            break;
        case MENU_GAME_OVER:
            currentMenu = &gameOverMenu;
            break;
        case MENU_SETTINGS:
            currentMenu = &settingsMenu;
            break;
        case MENU_NONE:
            currentMenu = NULL;
            break;
    }
}

void on_game_over(Event event) {
    switch_to_menu(MENU_GAME_OVER);
}
