#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game/map.h"

#include <time.h>

#include "display.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));

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

    sprite_list* sprites = load_sprites(ren);

    SDL_Event event;
    
    int running = 1;

    struct timeval* prev_update = malloc(sizeof(struct timeval));
    gettimeofday(prev_update, NULL);
    struct timeval* prev_draw = malloc(sizeof(struct timeval));
    gettimeofday(prev_draw, NULL);

    map* m = create_map(sprites);
    load_textures(m, ren);

    while (running) {
        struct timeval now;
        gettimeofday(&now, NULL);
        float last_update = (now.tv_sec - prev_update->tv_sec) + (now.tv_usec - prev_update->tv_usec) * 1e-6;
        float last_frame = (now.tv_sec - prev_draw->tv_sec) + (now.tv_usec - prev_draw->tv_usec) * 1e-6;
        

        /* Updating the game */
        // On vérifie qu'on quitte pas et on attend un appui de touche 
        if(last_update > 1 / 30) {
            gettimeofday(prev_update, NULL);

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = 0;
                }
            }
            
            update_map(m, win_width, win_height, last_update);
        }

        if(last_frame > 1/10) {
            gettimeofday(prev_draw, NULL);

            //Draw bg
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderClear(ren);

            draw_map(m, ren);
                    
            SDL_RenderPresent(ren);
        }
        //SDL_Delay(16); // Approximately 60 frames per second
    }

    //Free all the shit
    free_map(m);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}