#include "config.h"
#include "rendering.h"

void set_aligned_position(Alignment alignment, int *x, int *y, int w, int h){
    //get window size
    int window_width, window_height;
    SDL_GetWindowSize(Window, &window_width, &window_height);

    //if given alignment, calculate new x and y
    switch (alignment) {
        case UpperLeft:
            *x = 0;
            *y = 0;
            break;
        case UpperCenter:
            *x = (window_width - w) / 2;
            *y = 0;
            break;
        case UpperRight:
            *x = window_width - w;
            *y = 0;
            break;
        case MiddleLeft:
            *x = 0;
            *y = (window_height - h) / 2;
            break;
        case Center:
            *x = (window_width - w) / 2;
            *y = (window_height - h) / 2;
            break;
        case MiddleRight:
            *x = window_width - w;
            *y = (window_height - h) / 2;
            break;
        case LowerLeft:
            *x = 0;
            *y = window_height - h;
            break;
        case LowerCenter:
            *x = (window_width - w) / 2;
            *y = window_height - h;
            break;
        case LowerRight:
            *x = window_width - w;
            *y = window_height - h;
            break;
        default:
            break; //Alignment.Default; fall back on given x and y values
    }
}

SDL_FRect Render_Rect(int w, int h, int x, int y, Alignment alignment, SDL_Color color, bool color_fill) {
    //change x and y based on alignment
    set_aligned_position(alignment, &x, &y, w, h);

    //set up rect with renderer
    SDL_FRect rect = {x, y, w, h};
    SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    //render rect
    if (color_fill) {
        SDL_RenderFillRect(Renderer, &rect);
    }
    else {
        SDL_RenderRect(Renderer, &rect);
    }

    return rect;
}

SDL_FRect Render_Text(char *text, TTF_Font *font, int x, int y, Alignment alignment, SDL_Color color) {
    //set up text texture
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, 0, color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(Renderer, text_surface);

    //change x and y based on alignment
    float text_width = text_surface->w;
    float text_height = text_surface->h;
    set_aligned_position(alignment, &x, &y, text_width, text_height);

    //render text texture
    SDL_FRect destination_rect = {
        (float)x,
        (float)y,
        text_width,
        text_height
    };
    SDL_RenderTexture(Renderer, text_texture, NULL, &destination_rect);
    SDL_DestroySurface(text_surface);
    SDL_DestroyTexture(text_texture);

    return destination_rect;
}

SDL_FRect Render_Image(char *image, int w, int h, int x, int y, Alignment alignment, SDL_Color color) {
    //change x and y based on alignment
    set_aligned_position(alignment, &x, &y, w, h);

    //create texture and render it
    SDL_Texture *texture = IMG_LoadTexture(Renderer, image);
    SDL_FRect destination_rect = {x, y, w, h};
    SDL_RenderTexture(Renderer, texture, NULL, &destination_rect);
    SDL_DestroyTexture(texture);

    return destination_rect;
}