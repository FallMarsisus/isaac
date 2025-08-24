#include "menuComponent.h"
#include <math.h>

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

    // Draw circular background for smartwatch theme
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Semi-transparent circular background
    if (menu->type == MENU_PAUSE) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
    } else {
        SDL_SetRenderDrawColor(renderer, 20, 25, 40, 255);
    }
    SDL_RenderFillRect(renderer, NULL);
    
    // Draw circular border
    int centerX = win_width / 2;
    int centerY = win_height / 2;
    int radius = (win_width < win_height ? win_width : win_height) / 2 - 10;
    
    // Draw circle outline (approximate with rectangles)
    SDL_SetRenderDrawColor(renderer, 100, 120, 150, 255);
    for (int angle = 0; angle < 360; angle += 2) {
        int x = centerX + (int)(radius * cos(angle * M_PI / 180));
        int y = centerY + (int)(radius * sin(angle * M_PI / 180));
        SDL_Rect dot = {x-1, y-1, 2, 2};
        SDL_RenderFillRect(renderer, &dot);
    }

    // Smartwatch style circular menu items
    int itemHeight = 30;
    int spacing = 8;
    int totalHeight = menu->itemCount * (itemHeight + spacing);
    int startY = (win_height - totalHeight) / 2;

    for (int i = 0; i < menu->itemCount; i++) {
        // Create rounded rectangular items
        SDL_Rect itemRect = {
            20, // Left margin for round screen
            startY + i * (itemHeight + spacing),
            win_width - 40, // Right margin for round screen
            itemHeight
        };
        menu->items[i].bounds = itemRect;

        // Selected item gets special highlight
        if (i == menu->selectedItem) {
            SDL_SetRenderDrawColor(renderer, 70, 130, 200, 255);
            // Draw rounded corners effect with multiple rectangles
            SDL_RenderFillRect(renderer, &itemRect);
            
            // Glow effect for selected item
            SDL_Rect glowRect = {itemRect.x - 2, itemRect.y - 2, itemRect.w + 4, itemRect.h + 4};
            SDL_SetRenderDrawColor(renderer, 50, 100, 180, 100);
            SDL_RenderFillRect(renderer, &glowRect);
        } else {
            SDL_SetRenderDrawColor(renderer, 40, 50, 70, 200);
            SDL_RenderFillRect(renderer, &itemRect);
        }
        
        // Add subtle border
        SDL_SetRenderDrawColor(renderer, 80, 90, 110, 255);
        SDL_RenderDrawRect(renderer, &itemRect);

        // Center text in smartwatch style
        int textWidth = get_text_width(menu->items[i].text, get_fonts()->calibri, 16);
        int textX = (win_width - textWidth) / 2;
        int textY = startY + i * (itemHeight + spacing) + (itemHeight - 16) / 2;

        // Text color based on selection
        SDL_Color textColor = i == menu->selectedItem ? 
            (SDL_Color){255, 255, 255, 255} : 
            (SDL_Color){200, 220, 240, 255};

        display_text(
            menu->items[i].text,
            renderer, 
            get_fonts()->calibri,
            &textColor,
            textX,
            textY,
            16
        );
    }
}
