#include "config.h"
#include "game.h"

static int init_error(char *error_cause) {
    printf("Error: %s failed to initialize!\n", error_cause);
    return 1;
}

int Game_Init() {
    //initialize sdl and the other libraries (sdl_image is initialized with sdl_init())
    if (!SDL_Init(SDL_INIT_VIDEO)) return init_error("SDL");
    if (!TTF_Init()) return init_error("SDL_ttf");

    //initialize window and renderer
    Window = SDL_CreateWindow(TITLE, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!Window) return init_error("SDL window");
    SDL_SetWindowPosition(Window, 0, 0); //window position set to top left corner
    SDL_SetWindowMinimumSize(Window, MIN_WIDTH, MIN_HEIGHT); //window size limited

    Renderer = SDL_CreateRenderer(Window, NULL);
    if (!Renderer) return init_error("SDL renderer");

    return 0;
}

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

static Grid_Letter** Render_Grid() {
    Grid_Letter **grid = malloc(Word_Length * Max_Attempts * sizeof(Grid_Letter));
    if (!grid) return NULL;

    //window size
    int window_width, window_height;
    SDL_GetWindowSize(Window, &window_width, &window_height);

    //measurements
    const int WIDTH_MARGIN = 75;
    const int HEIGHT_MARGIN_TOP = 75;
    const int HEIGHT_MARGIN_BOTTOM = 200;
    const int GRID_PADDING = 10;
    const int FRAME_WIDTH = window_width - WIDTH_MARGIN * 2;
    const int FRAME_HEIGHT = window_height - (HEIGHT_MARGIN_TOP + HEIGHT_MARGIN_BOTTOM);
    const int FRAME_X = WIDTH_MARGIN;
    const int FRAME_Y = HEIGHT_MARGIN_TOP;

    //set up frame and children
    Render_Rect(FRAME_WIDTH, FRAME_HEIGHT, FRAME_X, FRAME_Y, Default, Color.White, true);
    for (int i=0; i<Word_Length; i++) {
        for (int j=0; j<Max_Attempts; j++) {
            //make rectangle child
            SDL_FRect child = Render_Rect(
                FRAME_WIDTH/Word_Length-GRID_PADDING,
                FRAME_HEIGHT/Max_Attempts-GRID_PADDING,
                FRAME_X+i*(FRAME_WIDTH/Word_Length)+GRID_PADDING/2,
                FRAME_Y+j*(FRAME_HEIGHT/Max_Attempts)+GRID_PADDING/2,
                Default,
                Color.DarkGray,
                true
            );
            Grid_Letter grid_letter;
            grid_letter.rect = child;
            grid_letter.color = Color.DarkGray;
            grid[j][i] = grid_letter;
        }
    }

    return grid;
}

void Render_Game() {
    Grid_Letter **grid = Render_Grid();

    if (!grid) {
        printf("Error: Failed to allocate memory for grid.\n");
    }
    else {
        printf("Success!");
    }
}