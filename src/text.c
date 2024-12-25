#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "display.h"

typedef struct text_s {
    SDL_Texture* texture;
    SDL_Color color;
    TTF_Font* font;
    bool underlined;
    char* text;
    char* fontName;
    int fontSize;
    int x;
    int y;
    int sizeX;
    int sizeY;
}* text;

// private
void renderText(SDL_Renderer* renderer, text texte) {
    if (texte->font == NULL) {
        fprintf(stderr, "Font not loaded: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    if (texte->text == NULL) {
        fprintf(stderr, "Text is NULL\n");
        exit(EXIT_FAILURE);
    }

    if (texte->texture != NULL) {
        SDL_DestroyTexture(texte->texture);
        texte->texture = NULL;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(texte->font, texte->text, texte->color);

    if (surface == NULL) {
        fprintf(stderr, "Failed to load surface\n");
        exit(EXIT_FAILURE);
    }

    texte->texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texte->texture == NULL) {
        fprintf(stderr, "Failed to load texture\n");
        exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(surface);
}

int max(int a, int b) {
    return a < b ? b : a;
}



// fonctions de création
text createExmptyText() {
    text texte = malloc(sizeof(struct text_s));
    texte->x = -1;
    texte->y = -1;
    texte->sizeX = -1;
    texte->sizeY = -1;
    texte->fontSize = -1;
    texte->underlined = false;
    texte->font = NULL;
    texte->texture = NULL;
    texte->color = (SDL_Color){0, 0, 0};
    texte->fontName = malloc(sizeof(char)*2);
    texte->text = malloc(sizeof(char)*2);
    strcpy(texte->fontName, "");
    strcpy(texte->text, "");

    return texte;
}

text createText(SDL_Renderer* renderer, int x, int y, int red, int green, int blue, char* string, char* fontName, int fontSize) {
    text texte = malloc(sizeof(struct text_s));
    texte->color = (SDL_Color){red, green, blue};
    texte->x = x;
    texte->y = y;
    texte->fontSize = fontSize;
    texte->underlined = false;

    texte->font = TTF_OpenFont(fontName, fontSize);
    int textWidth, textHeight;
    TTF_SizeText(texte->font, string, &textWidth, &textHeight);

    texte->sizeX = textWidth;
    texte->sizeY = textHeight;

    texte->text = malloc(strlen(string) + 1);
    strcpy(texte->text, string);

    texte->fontName = malloc(strlen(fontName) + 1);
    strcpy(texte->fontName, fontName);

    renderText(renderer, texte);

    return texte;
}



// fonctions transformateurs
void changeTextMessage(SDL_Renderer* renderer, text texte, char* newString) {
    free(texte->text);

    texte->text = malloc(strlen(newString) + 1);
    strcpy(texte->text, newString);
    
    renderText(renderer, texte);
}

void sizeTextCoord(text texte, int x, int y) {
    texte->x = x;
    texte->y = y;
}

void resizeText(SDL_Renderer* renderer, text texte, int newSize) {
    texte->fontSize = newSize;
    texte->font = TTF_OpenFont(texte->fontName, newSize);

    renderText(renderer, texte);
}

void changeTextFontSize(text texte, int newSize) {
    int textWidth, textHeight;
    texte->font = TTF_OpenFont(texte->fontName, newSize);
    TTF_SizeText(texte->font, texte->text, &textWidth, &textHeight);

    texte->sizeX = textWidth;
    texte->sizeY = textHeight;
    texte->fontSize = newSize;
}

void forceTextNewWidth(text texte, int newSizeX) {
    texte->sizeY = (texte->sizeY * newSizeX) / texte->sizeX;
    texte->sizeX = newSizeX;
    texte->fontSize = texte->sizeY;
    texte->font = TTF_OpenFont(texte->fontName, texte->fontSize);
}

void changeTextColor(SDL_Renderer* renderer, text texte, SDL_Color newColor) {
	texte->color = newColor;
	renderText(renderer, texte);
}

void changeTextUnderline(text texte, bool underlining) {
    texte->underlined = underlining;
}



// display functions
void displayText(SDL_Renderer* renderer, text texte) {
    SDL_Rect messageRect = {texte->x, texte->y, texte->sizeX, texte->sizeY};

    if (texte->underlined) {
        drawBox(renderer, texte->x, texte->y + texte->sizeY - (texte->fontSize / 10), texte->sizeX, max(texte->fontSize / 15, 1), texte->color.r, texte->color.g, texte->color.b);
    }

    SDL_RenderCopy(renderer, texte->texture, NULL, &messageRect);
}



// getter functions
int getTextX(text texte) {
    return texte->x;
}

int getTextY(text texte) {
    return texte->y;
}

int getTextSizeX(text texte) {
    return texte->sizeX;
}

int getTextSizeY(text texte) {
    return texte->underlined ? texte->sizeY + max(texte->fontSize / 15, 1) : texte->sizeY;
}

int getTextFontSize(text texte) {
    return texte->fontSize;
}

char* getTextString(text texte) {
    return texte->text;
}

char* getTextFontName(text texte) {
    return texte->fontName;
}

SDL_Color getTextColor(text texte) {
    return texte->color;
}

bool messageIsNull(text texte) {
    return texte->texture == NULL;
}

bool getTextUnderlineState(text texte) {
    return texte->underlined;
}