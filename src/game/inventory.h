#pragma once

#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include "../display.h"
#include <string.h>
#include <SDL2/SDL.h>

typedef struct {
    int id;
    char name[50];
    int quantity;
} item;

typedef struct {
    item *items;
    int size;
    int capacity;
} inv;

item* create_item(const char *name, int quantity);
inv* create_inventory(int capacity);
void add_item(inv *inventory, item* item);
void remove_item(inv *inventory, item* it, int count);
void free_inventory(inv *inventory);
void display_inventory(inv *inventory, SDL_Renderer *ren);

#endif // INVENTORY_H