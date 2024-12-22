#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "game/map.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* win = SDL_CreateWindow("Hello SDL2", 100, 100, 640, 360, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* image = SDL_LoadBMP("image.bmp");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, image);


    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Clear the window
    SDL_RenderClear(ren);

    // On affiche un sprite dans le terminal
    int win_width, win_height;
    SDL_GetWindowSize(win, &win_width, &win_height);

    SDL_Event event;
    int x = 0, y = 0;
    int running = 1;

    map* m = create_map();
    room* current = create_room(0, 0);
    add_room(m, current);

    int mx = 0;
    int my = 0;

    while (running) {

        // On vérifie qu'on quitte pas et on attend un appui de touche 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
            running = 0;
            }
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT]) x -= 5;
        if (state[SDL_SCANCODE_RIGHT]) x += 5;
        if (state[SDL_SCANCODE_UP]) y -= 5;
        if (state[SDL_SCANCODE_DOWN]) y += 5;

        if (x < 0) {
            x = win_width - 20;
            mx--;
            current = get_room(m, mx, my);
            if(current == NULL) {
                room* current = create_room(mx, my);
                add_room(m, current);
            }
        }
        if (y < 0) {
            y = win_height - 20;
            my--;
            current = get_room(m, mx, my);
            if(current == NULL) {
                room* current = create_room(mx, my);
                add_room(m, current);
            }
        }
        if (x > win_width - 20) {
            x = 0;
            mx++;
            current = get_room(m, mx, my);
            if(current == NULL) {
                room* current = create_room(mx, my);
                add_room(m, current);
            }
        }
        if (y > win_height - 20) {
            y = 0;
            my++;
            current = get_room(m, mx, my);
            if(current == NULL) {
                room* current = create_room(mx, my);
                add_room(m, current);
            }
        }
        
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255 );
        SDL_RenderClear(ren);
        
        SDL_Rect dstrect = { x, y, 20, 20 };
        SDL_SetRenderDrawColor(ren , 0, 0, 255, 255 );
        SDL_RenderDrawRect(ren, &dstrect);

        SDL_RenderPresent(ren);
    }
    

    // Wait for 0.1 second
    SDL_Delay(100);

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}