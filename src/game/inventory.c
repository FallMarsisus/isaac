#include "inventory.h"

inv* create_inventory(int capacity) {
    inv *inventory = (inv*)malloc(sizeof(inv));
    inventory->items = (item*)malloc(sizeof(item) * capacity);
    inventory->size = 0;
    inventory->capacity = capacity;
    return inventory;
}

void add_item(inv *inventory, int id, const char *name, int quantity) {
    if (inventory->size >= inventory->capacity) {
        printf("inv is full!\n");
        return;
    }
    item *item = &inventory->items[inventory->size++];
    item->id = id;
    snprintf(item->name, sizeof(item->name), "%s", name);
    item->quantity = quantity;
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
