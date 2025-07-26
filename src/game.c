#include "config.h"
#include "game.h"

#include "config.h"

static void set_aligned_position(Alignment alignment, int *x, int *y, int w, int h){
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

Container Create_Container(float x, float y, float w, float h) {
    //sdl_frect is used as a container to hold children
    Container container;
    container.parent = (SDL_FRect){x, y, w, h};
    container.children = NULL;
    container.child_count = 0;
    container.capacity = 0;
    return container;
}

void Render_Container(Container *container, Alignment alignment) {
    //render parent
    SDL_FRect parent_rect = Render_Rect(container->parent.w, container->parent.h, container->parent.x, container->parent.y, alignment, Color.White, false);

    //render children
    for (int i = 0; i < container->child_count; i++) {
        SDL_FRect child = container->children[i];
        float absolute_x = container->parent.x + child.x * container->parent.w;
        float absolute_y = container->parent.y + child.y * container->parent.h;
        float absolute_w = child.w * container->parent.w;
        float absolute_h = child.h * container->parent.h;

        Render_Rect(absolute_w, absolute_h, absolute_x, absolute_y, Default, Color.LightGray, true);
    }
}

void Add_Child_To_Container(Container *container, SDL_FRect child) {
    //allocate or expand the children array
    if (container->child_count == container->capacity) {
        container->capacity = (container->capacity == 0) ? 4 : container->capacity * 2;
        container->children = realloc(container->children, container->capacity * sizeof(SDL_FRect));
    }
    //add child
    container->children[container->child_count++] = child;
}

void Destroy_Container(Container *container) {
    //free container from memory
    free(container->children);
    container->children = NULL;
    container->child_count = 0;
    container->capacity = 0;
}

void Render_Game() {
    Container grid_container = Create_Container(5, 25, MIN_WIDTH-10, MIN_HEIGHT-50);

    for (int i=0; i<Word_Length; i++) {
        for (int j=0; j<Max_Attempts; j++) {
            SDL_FRect child = {
                .x = i*.1f,
                .y = j*.15f,
                .w = .08f,
                .h = .1f
            };
            Add_Child_To_Container(&grid_container, child);
        }
    }

    Render_Container(&grid_container, Center);
    Destroy_Container(&grid_container);
}