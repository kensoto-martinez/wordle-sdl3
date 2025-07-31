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
#define TPS 120
#define MIN_WIDTH 500
#define MIN_HEIGHT 700
#define DEFAULT_WIDTH 550
#define DEFAULT_HEIGHT 700

#define FOCUS_TILE_EXTRA_SIZE_PIXELS 8

//window stuff
extern SDL_Window *Window;
extern SDL_Renderer *Renderer;

//game configuring variables
extern int Word_Length;
extern int Max_Attempts;
extern int Focus_Letter_Index[2];

typedef struct {
    SDL_Color White;
    SDL_Color Black;
    SDL_Color LightGray;
    SDL_Color Gray;
    SDL_Color DarkGray;
    SDL_Color Green;
    SDL_Color Yellow;
    SDL_Color Red;
} Colors;
extern Colors Color;

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

typedef struct {
    SDL_FRect rect;
    SDL_Color color;
    char letter;
} Grid_Letter;
extern Grid_Letter **Grid;

#endif