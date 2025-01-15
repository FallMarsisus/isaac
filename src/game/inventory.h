#pragma once

#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

inv* create_inventory(int capacity);
void add_item(inv *inventory, int id, const char *name, int quantity);
void remove_item(inv *inventory, item* it, int count);
void free_inventory(inv *inventory);

#endif // INVENTORY_H