#include "config.h"
#include "game.h"

int main (int argc, char *argv[]) {
    //initialize game
    int init = Game_Init();
    if (init != 0) { //0 means success
        return init;
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