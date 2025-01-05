#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
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
    SDL_Window* win = SDL_CreateWindow("Hello SDL2", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int win_width = 640, win_height = 360;
    SDL_RenderSetLogicalSize(ren, win_width, win_height);

    // Load ttf
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    
    int running = 1;

    map* m = create_map();
    load_textures(m, ren);

    while (running) {
        // On vérifie qu'on quitte pas et on attend un appui de touche 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        
        update_map(m, win_width, win_height);
        
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255 );
        SDL_RenderClear(ren);

        draw_map(m, ren);
                
        SDL_RenderPresent(ren);
        SDL_Delay(16); // Approximately 60 frames per second
    }

    //Free all the shit
    free_map(m);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}