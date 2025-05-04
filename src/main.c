#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include "config.h"
#include "game_functions.h"

int main(int argc, char *argv[]) {
    srand(time(0));
 
    //initialize sdl library
    if (!SDL_Init(SDL_INIT_VIDEO)) {printf("SDL library failed to initialize\n"); return 1;}

    //initialize window
    SDL_Window *window = SDL_CreateWindow(NAME, WIDTH, HEIGHT, SDL_WINDOW_ALWAYS_ON_TOP);
    if (window == NULL) {printf("Window failed to initialize\n"); return 1;}

    //initialize renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {printf("Renderer failed to initialize\n"); return 1;}

    //initialize text library & provided font
    if (!TTF_Init()) {printf("TTF library failed to initialize\n"); return 1;}

    TTF_Font *font = TTF_OpenFont("assets/ShipporiAntique.ttf", TITLE_FONT_SIZE);

    //set up window loop
    bool running = true;
    SDL_Event window_event;

    //initialize key and letter grid arrayhihi
    SDL_Keycode key;
    clear_grid();
    generate_new_word();
    
    //load stats
    int *stats = get_stats();
    if (stats == NULL) {
        printf("Failed to get stats\n");
        return 1;
    }
    else {
        data.games_played = stats[0];
        data.wins = stats[1];
        data.current_streak = stats[2];
        data.best_streak = stats[3];
    }
    
    free(stats);

    while (running) {
        Uint32 frame_start = SDL_GetTicks();

        //get mouse position
        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        //handle events
        if (SDL_PollEvent(&window_event)) {
            switch (window_event.type) {
                //user exits window
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                //keyboard input
                case SDL_EVENT_KEY_DOWN: {
                    //don't register key presses if other menus are open
                    if (window_bools.help_menu || window_bools.settings_menu || window_bools.stats_menu) {break;}

                    key = window_event.key.key;

                    //only process letters and backspace
                    if (key >= 'a' && key <= 'z') {
                        letter_grid[focused_attempt - 1][focused_letter_number - 1] = toupper((char)key);
                        change_focus_square_position(true);
                    }
                    else if (key == SDLK_BACKSPACE) {
                        //last square has letter
                        if (focused_letter_number == word_length && letter_grid[focused_attempt - 1][focused_letter_number - 1] != '0') {
                            letter_grid[focused_attempt - 1][focused_letter_number - 1] = '0';
                        }
                        else {
                            letter_grid[focused_attempt - 1][focused_letter_number - 2] = '0';
                            change_focus_square_position(false);
                        }
                    }

                    switch (window_event.key.scancode) {
                        //move focus square
                        case SDL_SCANCODE_LEFT:
                            change_focus_square_position(false);

                            break;
                        case SDL_SCANCODE_RIGHT:
                            change_focus_square_position(true);

                            break;
                        //check word with Enter/Return key
                        case SDL_SCANCODE_RETURN:
                            check_word_and_start_next_attempt(renderer);
                        default:
                            break;
                    }

                    break;
                }
                //left mouse click down
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    //check if mouse is clicked on a keyboard key                   
                    for (int row = 0; row < 3; row++) {
                        for (int key_num = 0; key_num < 10; key_num++) {
                            //don't register keyboard click if other menus aren't open
                            if (window_bools.help_menu || window_bools.settings_menu || window_bools.stats_menu) {break;}

                            if (mouse_in_rect(mouse_x, mouse_y, keyboard_grid[row][key_num])) {
                                //set letter to clicked square
                                letter_grid[focused_attempt - 1][focused_letter_number - 1] = QWERTY[row][key_num];
                                change_focus_square_position(true);
                            }
                        }
                    }
                    
                    open_or_close_menu(mouse_x, mouse_y);

                    //check if mouse is clicked on hard mode checkbox
                    if (mouse_in_rect(mouse_x, mouse_y, hard_mode.rect) && window_bools.settings_menu) {
                        focused_attempt == 1 ? hard_mode.checked = !hard_mode.checked : change_error_message_and_call_timer("Wait for next game.");
                    }
                    
                    break;
            }
        }

        //color background and clear it from renders
        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
        SDL_RenderClear(renderer);

        //RENDER HERE

        render_main_game(renderer, font, (char)key);

        //RENDER HERE

        //renders present
        SDL_RenderPresent(renderer);

        //delay frame to achieve desired FPS
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DURATION) {
            SDL_Delay(FRAME_DURATION - frame_time);
        }
    }

    update_stats();

    //destroys everything
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}