#include "displayText.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

fonts* allFonts;

SDL_Texture* load_char(const int chara, const char* fontName, SDL_Renderer* renderer) {
	char* path = malloc(sizeof(char)*256);
	sprintf(path, "assets/fonts/%s/%d.bmp", fontName, chara);
	printf("Loading character from path: %s\n", path);
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
        fprintf(stderr, "Failed to load character : %c\n", (char)chara);
		fprintf(stderr, "Error : %s\n", SDL_GetError());
		free(path);
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
	free(path);
    return texture;
}

SDL_Texture** loadOneFont(const char* fontName, SDL_Renderer* ren) {
	SDL_Texture** textures = malloc(sizeof(SDL_Texture*) * 256);
	for (int i = 0; i < 256; i++) {
		textures[i] = load_char(i, fontName, ren);
	}
	return textures;
}

void load_fonts(SDL_Renderer* ren) {

	allFonts = malloc(sizeof(fonts));
	
	allFonts->calibri = loadOneFont("calibri", ren);
}

fonts* getFonts() {
	return allFonts;
}

void freeTextures(SDL_Texture** textures) {
	for (int i = 0; i < 256; i++) {
		if (textures[i] != NULL) {
			SDL_DestroyTexture(allFonts->calibri[i]);
		}
	}
}

void free_fonts() {
	freeTextures(allFonts->calibri);
	free(allFonts->calibri);
	free(allFonts);
}

void displayText(const char* text, SDL_Renderer* ren, SDL_Texture** font, int x, int y, int fontSize) {

	while (*text != '\0') {
		SDL_Texture* texture = font[(unsigned char)*text];
		if (texture) {
			printf("Displaying character: %c\n", *text);
			int texW = 20;
			int texH = 20;
			SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
			SDL_Rect dstrect = { x, y, texW * fontSize, texH * fontSize };
			SDL_RenderCopy(ren, texture, NULL, &dstrect);
			x += texW * fontSize;
		}

		text = &text[1];
	}

}