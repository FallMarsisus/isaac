#include "mouse.h"

bool mouseInRect(int x, int y, SDL_Rect rect) {
	return (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h);
}