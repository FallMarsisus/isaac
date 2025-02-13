#include "mouse.h"

bool mouseInRect(int x, int y, SDL_Rect rect) {
	return (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h);
}

bool mouseInRectFixDrift(int x, int y, SDL_Rect* rect, int trueWidth, int renderWidth) {
 {
	float scaleFactor = (float)trueWidth / renderWidth;
	SDL_Rect scaledRect = {
		.x = rect->x * scaleFactor,
		.y = rect->y * scaleFactor,
		.w = rect->w * scaleFactor,
		.h = rect->h * scaleFactor
	};
	return mouseInRect(x, y, scaledRect);
}
}