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

extern int Word_Length;
extern int Max_Attempts;

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
    SDL_FRect parent;
    SDL_FRect *children;
    int child_count;
    int capacity;
} Container;

// typedef struct {
//     SDL_FRect rect;
//     SDL_FRect text;
//     SDL_Color color;
//     bool is_focused;
// } Wordle_Grid_Square;

#endif