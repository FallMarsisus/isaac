#include "utils.h"

int ipow(int base, int exp) {
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

bool checkCollision(SDL_Rect *rect1, SDL_Rect *rect2) {
    return !(rect1->x > rect2->x + rect2->w ||  // rect1 is to the right of rect2
             rect1->x + rect1->w < rect2->x || // rect1 is to the left of rect2
             rect1->y > rect2->y + rect2->h || // rect1 is below rect2
             rect1->y + rect1->h < rect2->y);  // rect1 is above rect2
}

void draw_rect(SDL_Rect* rect, SDL_Renderer* ren, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(ren, r, g, b, a);
    SDL_RenderFillRect(ren, rect);
}

int random_int(int a, int b) {
    return (rand() % (b - a)) + a;
}