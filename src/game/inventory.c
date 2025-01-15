#include "inventory.h"

inv* create_inventory(int capacity) {
    inv *inventory = (inv*)malloc(sizeof(inv));
    inventory->items = (item*)malloc(sizeof(item) * capacity);
    inventory->size = 0;
    inventory->capacity = capacity;
    return inventory;
}

item* create_item(const char *name, int quantity) {
    item *it = (item*)malloc(sizeof(item));
    it->id = 0;
    strcpy(it->name, name);
    it->quantity = quantity;
    return it;
}

void add_item(inv *inventory, item* item) {
    if (inventory->size >= inventory->capacity) {
        printf("inv is full!\n");
        return;
    }
    inventory->items[inventory->size] = *item;
    inventory->size++;

}

void remove_item(inv *inventory, item* it, int count) {
    if (it->quantity < count) {
        printf("Not enough items to remove!\n");
        return;
    }
    if (it->quantity == count) {
        int index = it - inventory->items;
        for (int i = index; i < inventory->size - 1; i++) {
            inventory->items[i] = inventory->items[i + 1];
        }
        inventory->size--;
        return;
    }

    it->quantity -= count;

    if (it->quantity == 0) {
        int index = it - inventory->items;
        for (int i = index; i < inventory->size - 1; i++) {
            inventory->items[i] = inventory->items[i + 1];
        }
        inventory->size--;
    }
}

void free_inventory(inv *inventory) {
    free(inventory->items);
    free(inventory);
}

void display_inventory(inv *inventory, SDL_Renderer *ren) {
    int win_width = 640, win_height = 360;

    const SDL_Rect pos = (SDL_Rect) { (win_width - 388) / 2, (win_height - 196) / 2, 388, 196 };
    SDL_Rect rect = (SDL_Rect) { 0, 0, 388, 196 };
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 128);
    SDL_RenderFillRect(ren, &pos);
    SDL_RenderCopy(ren, load_sprites(ren)->inv_texture, &rect, &pos);
    
}