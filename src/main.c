#include "config.h"
#include "rendering.h"

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
        TTF_Font *font = TTF_OpenFont("assets/fonts/SG-ExtraLight.ttf", 24);
        Render_Rect(100, 50, 0, 0, Center, (SDL_Color){255, 255, 255, 255}, true);
        Render_Text("Hello world!", font, 0, 100, LowerCenter, (SDL_Color){255, 255, 255, 255});
        Render_Image("assets/images/wordleicon.png", 100, 100, 100, 0, UpperCenter, (SDL_Color){255, 255, 255, 255});
        //render here

        SDL_RenderPresent(Renderer);
        TTF_CloseFont(font);
    }

    //exit window
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    TTF_Quit();
    return 0;
}