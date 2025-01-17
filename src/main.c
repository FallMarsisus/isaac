#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game/game.h"

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

    double t = 0.;
    double dt = 1/60.;

    double current_time = SDL_GetTicks() / 1000.;
    double accumulator = 0.;

    game* 

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        double new_time = SDL_GetTicks() / 1000.;
        double frame_time = new_time - current_time;
        if(frame_time > 0.25) frame_time = 0.25;
        current_time = new_time;

        accumulator += frame_time;

        while(accumulator >= dt) {
            update_map(m, win_width, win_height, dt);
            t += dt;
            accumulator -= dt;
        }

        printf("FPS : %f\n", 1 / frame_time);

        //Draw bg
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        draw_map(m, ren);
                
        SDL_RenderPresent(ren);
        //SDL_Delay(16); // Approximately 60 frames per second
    }

    //Free all the shit
    free_map(m);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}