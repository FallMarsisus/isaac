#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../utils/displayText.h"
#include "../game/event/eventSystem.h"

typedef enum {
    MENU_MAIN,
    MENU_PAUSE,
    MENU_GAME_OVER,
    MENU_SETTINGS,
    MENU_NONE
} MenuType;

typedef struct {
    const char* text;
    SDL_Rect bounds;
    void (*callback)(void);
} MenuItem;

typedef struct {
    MenuType type;
    MenuItem* items;
    int itemCount;
    int selectedItem;
    bool isActive;
} MenuComponent;

void init_menu_component(MenuComponent* menu, MenuType type);
void free_menu_component(MenuComponent* menu);
void add_menu_item(MenuComponent* menu, const char* text, void (*callback)(void));
void handle_menu_input(MenuComponent* menu, SDL_Event event);
void draw_menu(MenuComponent* menu, SDL_Renderer* renderer, int win_width, int win_height);