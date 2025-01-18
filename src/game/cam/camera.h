#pragma once

#include <SDL2/SDL.h>

#include "../../utils/chained_list.h"

typedef struct camera_s Camera;

Camera* create_camera(int width, int height);
void free_camera(Camera* cam);

void load_in_cam(Camera* cam, chained_list* objects);

void update_cam(Camera* cam);

void draw_cam(Camera* cam, SDL_Renderer* ren);