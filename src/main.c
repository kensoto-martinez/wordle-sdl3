#include "config.h"
#include "game.h"

int init_error(char *error_cause) {
    printf("Error: %s failed to initialize!\n", error_cause);
    return 1;
}

int main (int argc, char *argv[]) {
    //initialize sdl and the other libraries (sdl_image is initialized with sdl_init())
    if (!SDL_Init(SDL_INIT_VIDEO)){
        return init_error("SDL");
    }
    if (!TTF_Init()) {
        return init_error("SDL_ttf");
    }

    //initialize window and renderer
    Window = SDL_CreateWindow(TITLE, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!Window) {
        return init_error("SDL window");
    }
    SDL_SetWindowPosition(Window, 0, 0); //window position set to top left corner
    SDL_SetWindowMinimumSize(Window, MIN_WIDTH, MIN_HEIGHT); //window size limited
    Renderer = SDL_CreateRenderer(Window, NULL);
    if (!Renderer) {
        return init_error("SDL renderer");
    }

    bool running = true;
    SDL_Event event;

    //window loop
    while (running) {
        if (SDL_PollEvent(&event)) {
            //event handling
            switch (event.type) {
                case SDL_EVENT_QUIT: //X window button pressed
                    running = false;
                    break;
            }
        }

        SDL_SetRenderDrawColor(Renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
        SDL_RenderClear(Renderer);

        //render here
        Render_Game();
        //render here

        SDL_RenderPresent(Renderer);
    }

    //exit window
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    TTF_Quit();
    return 0;
}