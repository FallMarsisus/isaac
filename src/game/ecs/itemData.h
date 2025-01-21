#pragma once 

#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int id;
    char* name;
    char* description;
    int value;
} ItemData;

#endif