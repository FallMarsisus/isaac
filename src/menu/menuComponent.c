#include "menuComponent.h"

void init_menu_component(MenuComponent* menu, MenuType type) {
    menu->type = type;
    menu->items = NULL;
    menu->itemCount = 0;
    menu->selectedItem = 0;
    menu->isActive = true;
}

void free_menu_component(MenuComponent* menu) {
    if (menu->items) {
        free(menu->items);
    }
    menu->itemCount = 0;
}

void add_menu_item(MenuComponent* menu, const char* text, void (*callback)(void)) {
    menu->items = realloc(menu->items, (menu->itemCount + 1) * sizeof(MenuItem));
    menu->items[menu->itemCount].text = text;
    menu->items[menu->itemCount].callback = callback;
    menu->itemCount++;
}

void handle_menu_input(MenuComponent* menu, SDL_Event event) {
    if (!menu->isActive) return;

    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    menu->selectedItem = (menu->selectedItem - 1 + menu->itemCount) % menu->itemCount;
                    break;
                case SDLK_DOWN:
                    menu->selectedItem = (menu->selectedItem + 1) % menu->itemCount;
                    break;
                case SDLK_RETURN:
                    if (menu->items[menu->selectedItem].callback) {
                        menu->items[menu->selectedItem].callback();
                    }
                    break;
            }
            break;
    }
}

void draw_menu(MenuComponent* menu, SDL_Renderer* renderer, int win_width, int win_height) {
    if (!menu->isActive) return;

    // Add semi-transparent background for pause menu
    if (menu->type == MENU_PAUSE) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
        SDL_RenderFillRect(renderer, NULL);
    }

    int itemHeight = 50;
    int spacing = 20;
    int totalHeight = menu->itemCount * (itemHeight + spacing);
    int startY = (win_height - totalHeight) / 2;

    for (int i = 0; i < menu->itemCount; i++) {
        SDL_Rect itemRect = {
            (win_width - 200) / 2,
            startY + i * (itemHeight + spacing),
            200,
            itemHeight
        };
        menu->items[i].bounds = itemRect;

        SDL_SetRenderDrawColor(renderer, 
            i == menu->selectedItem ? 100 : 50,
            i == menu->selectedItem ? 100 : 50,
            i == menu->selectedItem ? 100 : 50,
            255);
        SDL_RenderFillRect(renderer, &itemRect);

        int textWidth = get_text_width(menu->items[i].text, get_fonts()->calibri, 20);

        display_text(
            menu->items[i].text,
            renderer, 
            get_fonts()->calibri,
            &(SDL_Color) {255, 255, 255, 255},
            (win_width - textWidth) / 2,
            startY + i * (itemHeight + spacing),
            20
        );
    }
}
