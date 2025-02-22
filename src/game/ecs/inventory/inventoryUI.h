#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "./inventoryComponent.h"

bool mouse_in_slot(int x, int y, int row, int col);
bool mouse_in_any_slot(uint32_t entity, int x, int y);
bool mouse_in_constant_slots(int nth, int mouseX, int mouseY);

int on_clic(uint32_t entity, int mouseX, int mouseY);
int get_slot_of_mouse(uint32_t entity, int mouseX, int mouseY);
int get_clicked_acion(InventoryComponent* invent, int mouseX, int mouseY);

void draw_inventory(uint32_t entity, SDL_Renderer* renderer, int win_width, int win_height, int true_width , int true_height);
void print_inv_debug(InventoryComponent* invent);