#include "config.h"
#include "rendering.h"

SDL_FRect Render_Rect(int w, int h, int x, int y, Alignment alignment, SDL_Color color, bool color_fill) {
    //if given alignment, calculate new x and y
    switch (alignment) {
        case UpperLeft:
            x = 0;
            y = 0;
            break;
        case UpperCenter:
            x = (WIDTH - w) / 2;
            y = 0;
            break;
        case UpperRight:
            x = WIDTH - w;
            y = 0;
            break;
        case MiddleLeft:
            x = 0;
            y = (HEIGHT - h) / 2;
            break;
        case Center:
            x = (WIDTH - w) / 2;
            y = (HEIGHT - h) / 2;
            break;
        case MiddleRight:
            x = WIDTH - w;
            y = (HEIGHT - h) / 2;
            break;
        case LowerLeft:
            x = 0;
            y = HEIGHT - h;
            break;
        case LowerCenter:
            x = (WIDTH - w) / 2;
            y = HEIGHT - h;
            break;
        case LowerRight:
            x = WIDTH - w;
            y = HEIGHT - h;
            break;
        default:
            break; //Alignment.Default; fall back on given x and y values
    }

    SDL_FRect rect = {x, y, w, h};
    SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    if (color_fill) {
        SDL_RenderFillRect(Renderer, &rect);
    }
    else {
        SDL_RenderRect(Renderer, &rect);
    }

    return rect;

}