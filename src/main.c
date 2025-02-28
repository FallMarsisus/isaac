#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include "menu/menuManager.h"

#include <time.h>

#include "display.h"

int main() {
    srand(time(NULL));

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    // Create a window
	int true_width = 1280, true_height = 720;
    SDL_Window* win = SDL_CreateWindow("Hello SDL2", 100, 100, true_width, true_height, SDL_WINDOW_SHOWN);
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

    int win_width = 960, win_height = 540;
    SDL_RenderSetLogicalSize(ren, win_width, win_height);

    load_sprites(ren);
	load_fonts(ren);
    
    double t = 0.;
    double dt = 1/60.;

    double current_time = SDL_GetTicks() / 1000.;
    double accumulator = 0.;

    ECS_CreateManager();

    init_event_system();
	initDefaultItems();
    init_timer_system();

    init_menu_manager(win, ren);

    Mix_Music* music = Mix_LoadMUS("assets/AssetPack/Musics/21 - Dungeon.ogg");
    if (music == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }
    Mix_PlayMusic(music, -1); // Joue notre musique
    Mix_VolumeMusic(16); // Mets le volume a 0

    SDL_Event event;
    int running = 1;
    while (running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            handle_menu_manager_input(event);
        }

        double new_time = SDL_GetTicks() / 1000.;
        double frame_time = new_time - current_time;
        if(frame_time > 0.25) frame_time = 0.25;
        current_time = new_time;

        accumulator += frame_time;

        while(accumulator >= dt) {
            update_menu_manager(win_width, win_height, dt);
            t += dt;
            accumulator -= dt;
        }
        
        draw_menu_manager(ren, win_width, win_height, true_width, true_height);
        SDL_RenderPresent(ren);
    }

    Mix_FreeMusic(music); // Libére en mémoire notre musique

    free_menu_manager();

    ECS_DestroyManager();

    shutdown_timer_system();
    free_event_system();
	freeDefaultItems();
	free_fonts();

    Mix_CloseAudio();

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
