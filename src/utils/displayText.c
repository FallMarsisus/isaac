#include "displayText.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

fonts* allFonts;

SDL_Texture* load_char(const int chara, const char* fontName, SDL_Renderer* renderer) {
	char* path = malloc(sizeof(char)*256);
	sprintf(path, "assets/fonts/%s/%d.bmp", fontName, chara);
	// printf("Loading character from path: %s\n", path);
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
		// fprintf(stderr, "\033[0;31mFailed to load character : %c\033[0m\n", (char)chara);
		// fprintf(stderr, "\033[0;31mError : %s\033[0m\n", SDL_GetError());
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

fonts* get_fonts() {
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

int get_text_width(char* text, SDL_Texture** font, int fontSize) {
	int width = 0;
	while (*text != '\0') {
		SDL_Texture* texture = font[(unsigned char)*text];
		if (texture) {
			int texW, texH;
			SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
			float scale = 2 * (float)fontSize / texH;  // Calculate scale based on desired height
			
			width += (int)(texW * scale);
		}
		text++;
	}
	return width;
}

int get_text_height(char* text, SDL_Texture** font, int fontSize) {
	int height = 0;
	while (*text != '\0') {
		SDL_Texture* texture = font[(unsigned char)*text];
		if (texture) {
			int texW, texH;
			SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
			float scale = 2 * (float)fontSize / texH;  // Calculate scale based on desired height
			
			height += (int)(texH * scale);
		}
		text++;
	}
	return height;
}

void display_text(const char* text, SDL_Renderer* ren, SDL_Texture** font, SDL_Color* color, int x, int y, int fontSize) {
	while (*text != '\0') {
		SDL_Texture* texture = font[(unsigned char)*text];
		if (texture) {
			int texW, texH;
			SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
			SDL_SetTextureColorMod(texture, color->r, color->g, color->b);  // Sets RGB values
			SDL_SetTextureAlphaMod(texture, color->a);  // Sets alpha transparency
			float scale = 2 * (float)fontSize / texH;  // Calculate scale based on desired height
			SDL_Rect dstrect = { x, y, (int)(texW * scale), 2 * fontSize }; 
			
			SDL_RenderCopy(ren, texture, NULL, &dstrect);
			x += (int)(texW * scale);
		}
		text++;
	}

}