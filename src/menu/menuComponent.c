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
    
    // Draw circular border with gradient effect
    int centerX = win_width / 2;
    int centerY = win_height / 2;
    int radius = (win_width < win_height ? win_width : win_height) / 2 - 10;
    
    // Draw multiple circles for gradient border effect
    for (int r = radius; r > radius - 4; r--) {
        int alpha = 255 - (radius - r) * 60;
        SDL_SetRenderDrawColor(renderer, 100 + (radius - r) * 10, 120 + (radius - r) * 10, 150 + (radius - r) * 10, alpha);
        for (int angle = 0; angle < 360; angle += 3) {
            int x = centerX + (int)(r * cos(angle * M_PI / 180));
            int y = centerY + (int)(r * sin(angle * M_PI / 180));
            SDL_Rect dot = {x-1, y-1, 2, 2};
            SDL_RenderFillRect(renderer, &dot);
        }
    }

    // Add time display for main menu (smartwatch feature)
    if (menu->type == MENU_MAIN) {
        // Simple time display at top
        int timeY = 20;
        int timeWidth = get_text_width("12:34", get_fonts()->calibri, 12);
        display_text(
            "12:34", // Could be actual time in real implementation
            renderer,
            get_fonts()->calibri,
            &(SDL_Color){180, 200, 220, 255},
            (win_width - timeWidth) / 2,
            timeY,
            12
        );
    }

    // Add menu titles for better navigation
    const char* menuTitle = "Isaac";
    if (menu->type == MENU_SETTINGS) menuTitle = "Settings";
    else if (menu->type == MENU_PAUSE) menuTitle = "Paused";
    else if (menu->type == MENU_GAME_OVER) menuTitle = "Game Over";
    
    if (menu->type != MENU_MAIN) {
        int titleY = 30;
        int titleWidth = get_text_width(menuTitle, get_fonts()->calibri, 14);
        display_text(
            menuTitle,
            renderer,
            get_fonts()->calibri,
            &(SDL_Color){220, 240, 255, 255},
            (win_width - titleWidth) / 2,
            titleY,
            14
        );
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

        // Selected item gets special highlight with animation-like effect
        if (i == menu->selectedItem) {
            // Multiple glow layers for animated effect
            for (int layer = 0; layer < 3; layer++) {
                SDL_Rect glowRect = {
                    itemRect.x - (layer + 1) * 2, 
                    itemRect.y - (layer + 1) * 2, 
                    itemRect.w + (layer + 1) * 4, 
                    itemRect.h + (layer + 1) * 4
                };
                SDL_SetRenderDrawColor(renderer, 50 - layer * 10, 100 - layer * 20, 180 - layer * 30, 80 - layer * 25);
                SDL_RenderFillRect(renderer, &glowRect);
            }
            
            SDL_SetRenderDrawColor(renderer, 70, 130, 200, 255);
            SDL_RenderFillRect(renderer, &itemRect);
            
            // Add selection indicator on the left (smartwatch style)
            SDL_Rect indicator = {itemRect.x - 8, itemRect.y + itemRect.h / 2 - 2, 4, 4};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &indicator);
        } else {
            SDL_SetRenderDrawColor(renderer, 40, 50, 70, 200);
            SDL_RenderFillRect(renderer, &itemRect);
        }
        
        // Add subtle border
        SDL_SetRenderDrawColor(renderer, 80, 90, 110, 255);
        SDL_RenderDrawRect(renderer, &itemRect);

        // Draw simple icons for settings menu items (random icon for settings as requested)
        if (menu->type == MENU_SETTINGS) {
            int iconX = itemRect.x + 8;
            int iconY = itemRect.y + itemRect.h / 2;
            int iconSize = 8;
            
            SDL_SetRenderDrawColor(renderer, 180, 200, 220, 255);
            
            if (i == 0) { // Audio setting - draw speaker icon
                SDL_Rect speaker = {iconX, iconY - 3, 6, 6};
                SDL_RenderFillRect(renderer, &speaker);
                SDL_Rect wave1 = {iconX + 7, iconY - 1, 2, 2};
                SDL_RenderFillRect(renderer, &wave1);
            } else if (i == 1) { // Reset - draw circular arrow icon
                for (int angle = 45; angle < 315; angle += 15) {
                    int x = iconX + 4 + (int)(3 * cos(angle * M_PI / 180));
                    int y = iconY + (int)(3 * sin(angle * M_PI / 180));
                    SDL_Rect dot = {x, y, 2, 2};
                    SDL_RenderFillRect(renderer, &dot);
                }
            } else { // Back - draw arrow icon
                SDL_Rect arrow[] = {
                    {iconX, iconY, 2, 2},
                    {iconX + 2, iconY - 2, 2, 2},
                    {iconX + 2, iconY + 2, 2, 2},
                    {iconX + 4, iconY, 4, 2}
                };
                for (int j = 0; j < 4; j++) {
                    SDL_RenderFillRect(renderer, &arrow[j]);
                }
            }
        }

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
