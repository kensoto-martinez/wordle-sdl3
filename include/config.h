#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_image.h>

#define TITLE "Wordle"
#define VERSION = "0.1-alpha"
#define BACKGROUND_COLOR (SDL_Color){0, 0, 0, 255}
#define WIDTH 800
#define HEIGHT 600
#define MIN_WIDTH 400
#define MIN_HEIGHT 600

extern SDL_Window *Window;
extern SDL_Renderer *Renderer;

typedef enum {
    UpperLeft,
    UpperCenter,
    UpperRight,
    MiddleLeft,
    Center,
    MiddleRight,
    LowerLeft,
    LowerCenter,
    LowerRight,
    Default
} Alignment;

#endif