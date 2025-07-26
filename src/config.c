#include "config.h"

SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;

Colors Color = {
    .White = {255, 255, 255, 255},
    .Black = {0, 0, 0, 255},
    .LightGray = {175, 175, 175, 255},
    .Gray = {100, 100, 100, 255},
    .DarkGray = {50, 50, 50, 255},
    .Green = {0, 255, 0, 255},
    .Yellow = {255, 255, 0, 255},
    .Red = {255, 0, 0, 255}
};

//game configuring variables (changable in Settings)
int Word_Length = 5;
int Max_Attempts = 6;