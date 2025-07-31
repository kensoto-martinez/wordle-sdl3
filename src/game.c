#include "config.h"
#include "game.h"

static int Init_Error(char *error_cause) {
    printf("Error: %s failed to initialize!\n", error_cause);
    return 1;
}

int Game_Init() {
    //initialize sdl and the other libraries (sdl_image is initialized with sdl_init())
    if (!SDL_Init(SDL_INIT_VIDEO)) return Init_Error("SDL");
    if (!TTF_Init()) return Init_Error("SDL_ttf");

    //get resolution to position window in center of screen
    const SDL_DisplayMode *dm = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
    int x, y;

    if (dm != NULL) {
        x = dm->w - DEFAULT_WIDTH;
        y = dm->h - DEFAULT_HEIGHT;
    }
    else {
        //use arbitrary positioning if display is unknown
        x = 100;
        y = 100;
    }

    //initialize window and renderer
    Window = SDL_CreateWindow(TITLE, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!Window) return Init_Error("SDL window");
    SDL_SetWindowPosition(Window, x/2, y/2); //center of screen (if display is known)
    SDL_SetWindowMinimumSize(Window, MIN_WIDTH, MIN_HEIGHT); //window size limited

    Renderer = SDL_CreateRenderer(Window, NULL);
    if (!Renderer) return Init_Error("SDL renderer");

    return 0;
}


static void Set_Aligned_Position(Alignment alignment, int *x, int *y, int w, int h) {
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

static SDL_FRect Render_Rect(int w, int h, int x, int y, Alignment alignment, SDL_Color color, bool color_fill) {
    //change x and y based on alignment
    Set_Aligned_Position(alignment, &x, &y, w, h);

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

static SDL_FRect Render_Text(char *text, TTF_Font *font, int x, int y, Alignment alignment, SDL_Color color) {
    //set up text texture
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, 0, color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(Renderer, text_surface);

    //change x and y based on alignment
    float text_width = text_surface->w;
    float text_height = text_surface->h;
    Set_Aligned_Position(alignment, &x, &y, text_width, text_height);

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

static SDL_FRect Render_Image(char *image, int w, int h, int x, int y, Alignment alignment, SDL_Color color) {
    //change x and y based on alignment
    Set_Aligned_Position(alignment, &x, &y, w, h);

    //create texture and render it
    SDL_Texture *texture = IMG_LoadTexture(Renderer, image);
    SDL_FRect destination_rect = {x, y, w, h};
    SDL_RenderTexture(Renderer, texture, NULL, &destination_rect);
    SDL_DestroyTexture(texture);

    return destination_rect;
}


static void Render_Grid() {
    //allocate memory for grid
    Grid = malloc(Max_Attempts * sizeof(Grid_Letter*));
    if (Grid == NULL) {
        printf("Error allocating memory for grid\n");
        return;
    }
    for (int i=0; i<Max_Attempts; i++) {
        Grid[i] = malloc(Word_Length*sizeof(Grid_Letter));
        if (Grid[i] == NULL) {
            printf("Error allocating memory for grid\n");
            return;
        }
    }

    //window size
    int window_width, window_height;
    SDL_GetWindowSize(Window, &window_width, &window_height);

    //frame adjustments
    const int WIDTH_MARGIN = 75;
    const int HEIGHT_MARGIN_TOP = 75;
    const int HEIGHT_MARGIN_BOTTOM = 200;
    const int GRID_PADDING = 10;

    //tile size measurements
    const int FRAME_WIDTH = window_width - WIDTH_MARGIN * 2;
    const int FRAME_HEIGHT = window_height - (HEIGHT_MARGIN_TOP + HEIGHT_MARGIN_BOTTOM);
    const int FRAME_X = WIDTH_MARGIN;
    const int FRAME_Y = HEIGHT_MARGIN_TOP;
    int tile_size = (FRAME_WIDTH - (Word_Length - 1) * GRID_PADDING) / Word_Length;
    int tile_size_h = (FRAME_HEIGHT - (Max_Attempts - 1) * GRID_PADDING) / Max_Attempts;
    if (tile_size_h < tile_size) tile_size = tile_size_h;
    const int GRID_WIDTH = tile_size * Word_Length + (Word_Length - 1) * GRID_PADDING;
    const int GRID_HEIGHT = tile_size * Max_Attempts + (Max_Attempts - 1) * GRID_PADDING;
    const int GRID_X = FRAME_X + (FRAME_WIDTH - GRID_WIDTH) / 2;
    const int GRID_Y = FRAME_Y + (FRAME_HEIGHT - GRID_HEIGHT) / 2;

    //set up grid
    for (int i=0; i<Word_Length; i++) {
        for (int j=0; j<Max_Attempts; j++) {
            int x = GRID_X + i * (tile_size + GRID_PADDING);
            int y = GRID_Y + j * (tile_size + GRID_PADDING);

            //render focus tile if tile is focused
            if (j == Focus_Letter_Index[0] && i == Focus_Letter_Index[1]) {
                Render_Rect(tile_size+FOCUS_TILE_EXTRA_SIZE_PIXELS, tile_size+FOCUS_TILE_EXTRA_SIZE_PIXELS, x-FOCUS_TILE_EXTRA_SIZE_PIXELS/2, y-FOCUS_TILE_EXTRA_SIZE_PIXELS/2, Default, Color.White, true);
            }

            //render grid tile
            Grid_Letter grid_letter;
            grid_letter.rect = Render_Rect(tile_size, tile_size, x, y, Default, Color.DarkGray, true);
            grid_letter.color = Color.DarkGray;
            grid_letter.letter = ' ';

            //assign grid tile
            Grid[j][i] = grid_letter;
        }
    }
}

void Render_Game() {
    //render grid
    Render_Grid();

    //blah blah blah

    //free grid
    for (int i=0; i<Max_Attempts; i++) {
        free(Grid[i]);
    }
    free(Grid);
}

void Register_Key_Press(SDL_Keycode key) {
    Grid_Letter focused_tile = Grid[Focus_Letter_Index[0]][Focus_Letter_Index[1]];

    switch(key) {
        //handle key deletion
        case SDLK_BACKSPACE:
            focused_tile.letter = ' ';
            if (Focus_Letter_Index[1] > 0) {
                Focus_Letter_Index[1]--;
            }

            break;
        //handle focus tile movement to the left
        case SDLK_LEFT:
            if (Focus_Letter_Index[1] > 0) {
                Focus_Letter_Index[1]--;
            }
            break;
        //handle focus tile movement to the right
        case SDLK_RIGHT:
            if (Focus_Letter_Index[1] < Word_Length - 1) {
                Focus_Letter_Index[1]++;
            }
            break;
        default:
            //handle letter input
            if (key >= 'a' && key <= 'z') {
                focused_tile.letter = key;
                
                if (Focus_Letter_Index[1] < Word_Length - 1) {
                    Focus_Letter_Index[1]++;
                }
            }

            break;
    }
}