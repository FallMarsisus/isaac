#include "mouse.h"

bool mouse_in_rect(int x, int y, SDL_Rect rect) {
	return (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h);
}

bool mouse_in_rect_fix_drift(int x, int y, SDL_Rect* rect, int trueWidth, int renderWidth) {
 {
	float scaleFactor = (float)trueWidth / renderWidth;
	SDL_Rect scaledRect = {
		.x = rect->x * scaleFactor,
		.y = rect->y * scaleFactor,
		.w = rect->w * scaleFactor,
		.h = rect->h * scaleFactor
	};
	return mouse_in_rect(x, y, scaledRect);
}
}