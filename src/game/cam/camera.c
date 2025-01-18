#include "camera.h"

typedef struct camera_s {
    int posx; int posy;
    int width; int height;

    chained_list** layers;
    int nb_layers;
} Camera;

Camera* create_camera(int width, int height) {
    Camera* cam = malloc(sizeof(Camera));
    cam->width = width; cam->height = height;
    cam->posx = 0;
    cam->posy = 0;

    cam->nb_layers = 2;
    cam->layers = malloc(sizeof(chained_list*) * cam->nb_layers);

    return cam;
}
void free_camera(Camera* cam);

void load_in_cam(Camera* cam, chained_list* objects);

void update_cam(Camera* cam);

void draw_cam(Camera* cam, SDL_Renderer* ren) {
    for(int i = 0; i < cam->nb_layers; i++) {
        for(cell* c = get_first(cam->layers[i]); c != NULL; c = get_next(c)) {
            /*
            Object* obj = get_data(c);
            if(obj == NULL) continue;

            if(obj->self_draw_object != NULL) obj->self_draw_object(obj, cam->posx, cam->posy, ren);
            else draw_object(obj, cam->posx, cam->posy, ren);
            */
        }
    }
}