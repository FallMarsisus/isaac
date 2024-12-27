#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "game/map.h"

#include "game/player.h"

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

    //Calibre la taille de la fenetre
    //SDL_GetWindowSize(win, &win_width, &win_height);

    SDL_Event event;
    
    int running = 1;

    map* m = create_map();
    
    room* current = create_room(0, 0);
    add_room(m, current);

    player* p = create_player(320, 180);

    //=========Loads textures===========
    load_player_textures(
        p, 
        ren,
        "assets/player/sprite_sheet.bmp"
    );

    while (running) {
        // On vérifie qu'on quitte pas et on attend un appui de touche 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        
        get_inputs(p);

        current = update(p, win_width, win_height, m, current);
        
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255 );
        SDL_RenderClear(ren);
        
        draw_room(ren, current);
        draw_player(ren, p);
                
        SDL_RenderPresent(ren);
        SDL_Delay(16); // Approximately 60 frames per second
    }

    //Free all the shit
    free_player(p);
    free_map(m);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}