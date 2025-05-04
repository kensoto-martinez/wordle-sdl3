#include "config.h"
#include "game_functions.h"

bool game_ended = false;

SDL_FRect render_rect(SDL_Renderer *renderer, int x, int y, int width, int height, SDL_Color color, bool filled) { //put x or y as -1 to center text in that axis
    if (x == -1) {x = WIDTH / 2 - width / 2;}
    if (y == -1) {y = HEIGHT / 2 - height / 2;}

    SDL_FRect rect = {x, y, width, height};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    //filled meaning the square will be colored in
    if (filled) { 
        SDL_RenderFillRect(renderer, &rect);
    }
    else {
        SDL_RenderRect(renderer, &rect);
    }

    return rect;
}

SDL_FRect render_text(SDL_Renderer *renderer, TTF_Font *font, char *text, int x, int y, SDL_Color color) { //put x or y as -1 to center text in that axis
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, 0, color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    //text texture dimensions
    float text_width = text_surface->w;
    float text_height = text_surface->h;

    if (x == -1) {x = WIDTH / 2 - text_width / 2;}
    if (y == -1) {y = HEIGHT / 2 - text_height / 2;}

    SDL_FRect destination_rect = {
        (float)x,
        (float)y,
        text_width,
        text_height
    };

    SDL_RenderTexture(renderer, text_texture, NULL, &destination_rect);
    SDL_DestroySurface(text_surface);
    SDL_DestroyTexture(text_texture);

    return destination_rect;
}

SDL_FRect render_texture(SDL_Renderer *renderer, char *file_path, int x, int y, int width, int height) {
    //create texture
    SDL_Texture *texture = IMG_LoadTexture(renderer, file_path);
    if (!texture) {printf("Failed to load texture: %s\n", SDL_GetError()); return (SDL_FRect){0, 0, 0, 0};}

    SDL_FRect destination_rect = {x, y, width, height};
    SDL_RenderTexture(renderer, texture, NULL, &destination_rect);
    SDL_DestroyTexture(texture);

    return destination_rect;
}

Checkbox render_checkbox(SDL_Renderer *renderer, int x, int y, int size, bool checked) {
    SDL_FRect checkbox;

    if (checked) {
        checkbox = render_rect(renderer, x, y, size, size, COLORS.GREEN, true);
        render_texture(renderer, "assets/checkmark.png", checkbox.x + size * .2, checkbox.y + size * .2, size * .6, size * .6);
    }
    else {
        checkbox = render_rect(renderer, x, y, size, size, COLORS.BLACK, false);
    }

    return (Checkbox){checkbox, checked};
}

void render_main_game(SDL_Renderer *renderer, TTF_Font *font, char new_letter) {
    TTF_Font *grid_font = TTF_OpenFont("assets/ShipporiAntique.ttf", GRID_FONT_SIZE);

    //renders if main game boolean is true
    if (window_bools.main_game) {
        //different font for button text
        TTF_Font *button_font = TTF_OpenFont("assets/Outfit-Bold.ttf", 23);

        //render title text & set margins
        SDL_FRect title_text = render_text(renderer, font, "WORDLE UNLIMITED", -1, 10, COLORS.WHITE);
        const int title_bottom = title_text.y + title_text.h + 10;
        const int available_height = 400;

        //calculate square size based on gap
        const int MARGIN = 20;
        const int GAP = 10;
        const int usable_width = WIDTH - 2 * MARGIN;
        const int max_grid_width = usable_width / word_length - GAP;
        const int max_grid_height = available_height / tries - GAP;
        const int square_size = max_grid_width < max_grid_height ? max_grid_width : max_grid_height;
        
        //calculate grid size with square size and gaps
        const int start_x = (WIDTH - (square_size * word_length + (word_length - 1) * GAP)) / 2;
        const int start_y = title_bottom + (available_height - (square_size * tries + (tries - 1) * GAP)) / 2;

        //render grid with text
        for (int row = 0; row < tries; row++) {
            for (int col = 0; col < word_length; col++) {
                //set square position
                const int x = start_x + col * (square_size + GAP);
                const int y = start_y + row * (square_size + GAP);
                const int highlight_padding = 5;

                //focus square
                if (col == focused_letter_number - 1 && row == focused_attempt - 1) {
                    render_rect(renderer, x - highlight_padding, y - highlight_padding, square_size + highlight_padding * 2, square_size + highlight_padding * 2, COLORS.WHITE, true);
                }

                //grid square
                SDL_FRect grid_square = render_rect(renderer, x, y, square_size, square_size, grid_colors[row][col], true);
                grid[col][row] = grid_square;

                //renders character if stored; 0 is sentinel value
                if (letter_grid[row][col] != '0') {
                    render_text(renderer, grid_font, (char[2]){letter_grid[row][col], '\0'}, x + grid_square.w * .3, y - grid_square.h * .05, COLORS.WHITE); //arbitrary centering
                }
            }
        }

        //render keyboard
        int game_grid_bottom = start_y + square_size * tries + GAP * (tries - 1);
        int keyboard_start_y = game_grid_bottom + MARGIN;
        int key_square_size = 37;
        int key_gap = 7;
        int keys_per_row[] = {10, 9, 7};
        int row_offsets[] = {0, 1, 2};

        for (int r = 0; r < 3; r++) {
            int row_length = keys_per_row[r];
            int total_row_width = row_length * key_square_size + (row_length - 1) * key_gap;
            int start_x = (WIDTH - total_row_width) / 2;

            for (int i = 0; i < row_length; i++) {
                int x = start_x + i * (key_square_size + key_gap);
                int y = keyboard_start_y + r * (key_square_size + key_gap);
                char letter = QWERTY[r][i];

                //render keyboard button
                keyboard_grid[r][i] = render_rect(renderer, x, y, key_square_size, key_square_size, keyboard_colors[r][i], true);
                render_text(renderer, grid_font, (char[2]){toupper(letter), '\0'}, x + key_square_size * .1, y - key_square_size * .2, COLORS.WHITE);
            }
        }

        //render help & settings & stats button
        const int BUTTON_MARGIN = 10;
        const int BUTTON_SIZE = 25;

        buttons.help_button = render_text(renderer, button_font, "?", WIDTH - BUTTON_MARGIN * 2.5, BUTTON_MARGIN - 5, COLORS.WHITE);
        buttons.settings_button = render_texture(renderer, "assets/gear.png", BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_SIZE, BUTTON_SIZE);
        buttons.stats_button = render_texture(renderer, "assets/barchart.png", BUTTON_MARGIN * 2 + BUTTON_SIZE, BUTTON_MARGIN, BUTTON_SIZE, BUTTON_SIZE);

        TTF_CloseFont(button_font);
    }

    //after timer, error message bool = false
    if (timer_running) {
        Uint32 elapsed_time = SDL_GetTicks() - timer_start;

        if (elapsed_time >= timer_duration) {
            timer_running = false;
            window_bools.error_message = false;
        }
    }

    //renders menus if respective boolean is true
    if (window_bools.help_menu) {
        render_help_menu(renderer, grid_font);
    }
    if (window_bools.settings_menu) {
        render_settings_menu(renderer);
    }
    if (window_bools.error_message) {
        render_error_message(renderer);
    }
    if (window_bools.stats_menu) {
        render_stats_menu(renderer);
    }

    //restart game if lost or won
    if (game_ended) {
        restart_game(renderer);
        game_ended = false;
    }

    TTF_CloseFont(grid_font);
}

void render_error_message(SDL_Renderer *renderer) {
    TTF_Font *error_font = TTF_OpenFont("assets/ShipporiAntique.ttf", 16);
    SDL_FRect message_box = render_rect(renderer, -1, HEIGHT / 2 - 20, 300, 40, COLORS.WHITE, true);

    render_text(renderer, error_font, error_message, -1, message_box.y + 10, COLORS.BLACK);

    TTF_CloseFont(error_font);
}

void render_help_menu(SDL_Renderer *renderer, TTF_Font *grid_font) {
    TTF_Font *title_font = TTF_OpenFont("assets/Outfit-ExtraBold.ttf", 25);
    TTF_Font *text_font = TTF_OpenFont("assets/Outfit-Bold.ttf", 15);
    char info_text[50];
    sprintf(info_text, "Guess the %d-letter word in %d tries!", word_length, tries);

    const int MARGIN = 15;
    const int LINE_SPACING = 30;
    const int LINE_HEIGHT = 35;
    int current_y = 10;

    char *texts[] = {
        "HOW TO PLAY",
        info_text,
        "Guess the mystery word from clues given by the words you type.",
        "Each guess must be valid, you cannot enter random letters.",
        "Some valid words derive from other languages or are slang.",
        "Hit the enter button to submit the guess.",
        "0",
        "Examples",
        "CORAL",
        "The A is in the correct position",
        "SPRAY",
        "The P is in the word but not in the correct position",
        "GHOST",
        "The G is not in the word",
        "0",
        "Check your stats by pressing the stats button!",
        "Edit individual letters in your guess using the arrow keys!",
        "There is one super secret word, you'll know it when you see it!"
    };
    int texts_length = sizeof(texts) / sizeof(texts[0]);
    int example_word_indexes[] = {8, 10, 12};

    render_background_with_exit_button(renderer, &buttons.help_exit_button);

    //text
    for (int i = 0; i < texts_length; i++) {
        int text_gap = 0;

        if (strcmp(texts[i], "0") == 0) {
            //line
            SDL_SetRenderDrawColor(renderer, COLORS.WHITE.r, COLORS.WHITE.g, COLORS.WHITE.b, COLORS.WHITE.a);
            SDL_RenderLine(renderer, MARGIN, current_y, WIDTH - MARGIN, current_y);
            text_gap = -15;
        } else {
            //text
            TTF_Font *font;
            bool is_example_word = false;

            //render word using grid
            for (int j = 0; j < 3; j++) {
                if (i == example_word_indexes[j]) {
                    is_example_word = true;

                    for (int k = 0; k < word_length; k++) {
                        SDL_Color color = COLORS.DARK_GRAY;
                        if (j == 0 && k == 3) {
                            color = COLORS.GREEN;
                        }
                        else if (j == 1 && k == 1) {
                            color = COLORS.YELLOW;
                        }
                        else if (j == 2 && k == 0) {
                            color = COLORS.GRAY;
                        }

                        SDL_FRect grid_square = render_rect(renderer, MARGIN + k * (LINE_HEIGHT + LINE_SPACING) + 60, current_y, 40, 40, color, true);
                
                        render_text(renderer, grid_font, (char[2]){texts[i][k], '\0'}, grid_square.x + grid_square.w * .15, grid_square.y - 7, COLORS.WHITE);
                    }
                }
            }

            //change text gap and font if title
            if (is_example_word) {
                text_gap = 20;
            }
            else {
                if (i == 0) {
                    font = title_font;
                    text_gap = 15;
                }
                else {
                    font = text_font;
                }
                
                render_text(renderer, font, texts[i], -1, current_y, COLORS.WHITE);
            }
        }

        current_y += LINE_SPACING + text_gap;
    }

    TTF_CloseFont(title_font);
    TTF_CloseFont(text_font);
}

void render_settings_menu(SDL_Renderer *renderer) {
    TTF_Font *title_font = TTF_OpenFont("assets/Outfit-ExtraBold.ttf", 25);
    TTF_Font *text_font = TTF_OpenFont("assets/Outfit-Bold.ttf", 20);
    TTF_Font *subtext_font = TTF_OpenFont("assets/Outfit-Bold.ttf", 10);

    render_background_with_exit_button(renderer, &buttons.settings_exit_button);

    //settings options
    char *texts[] = {
        "SETTINGS",
        "Hard Mode",
        "Word Length",
        "Guesses"
    };
    char *subtexts[] = {
        "i.e., green/yellow letters must be used in next guess, gray letters cannot be used.",
        "Change mystery word length. (COMING SOON)",
        "Change number of guesses to guess mystery word. (COMING SOON)"
    };
    const int TEXTS_LENGTH = sizeof(texts) / sizeof(texts[0]);
    const int MARGIN = 15;
    const int LINE_SPACING = 30;
    const int LINE_HEIGHT = 35;
    int current_y = 10;

    for (int i = 0; i < TEXTS_LENGTH; i++) {
        int text_gap = 0;
        TTF_Font *font;

        // Change text gap and font if title
        if (i == 0) {
            font = title_font;
            text_gap = 20; // Extra spacing for the title
        } else {
            font = text_font;
        }

        //render text
        SDL_FRect text_rect = render_text(renderer, font, texts[i], MARGIN, current_y, COLORS.WHITE);

        if (strcmp(texts[i], "Hard Mode") == 0) {
            //hard mode checkbox
            hard_mode = render_checkbox(renderer, text_rect.x + text_rect.w + MARGIN, current_y + 5, hard_mode.rect.w, hard_mode.checked);
        }

        current_y += LINE_SPACING - 20;

        //render subtext if not title
        if (i != 0) {
            current_y += text_rect.h + 5;
            render_text(renderer, subtext_font, subtexts[i - 1], MARGIN, current_y, COLORS.WHITE);

            current_y += LINE_SPACING;

            SDL_SetRenderDrawColor(renderer, COLORS.WHITE.r, COLORS.WHITE.g, COLORS.WHITE.b, COLORS.WHITE.a);
            SDL_RenderLine(renderer, MARGIN, current_y, WIDTH - MARGIN, current_y);
        }

        current_y += LINE_SPACING + text_gap;
    }

    TTF_CloseFont(title_font);
    TTF_CloseFont(text_font);
    TTF_CloseFont(subtext_font);
}

void render_stats_menu(SDL_Renderer *renderer) {
    TTF_Font *title_font = TTF_OpenFont("assets/Outfit-ExtraBold.ttf", 25);
    TTF_Font *text_font = TTF_OpenFont("assets/Outfit-Bold.ttf", 20);

    render_background_with_exit_button(renderer, &buttons.stats_exit_button);

    //stats options
    char *texts[] = {
        "STATS",
        "Games Played",
        "Wins",
        "Current Streak",
        "Best Streak",
        "Guesses Distribution (COMING SOON)"
    };
    const int TEXTS_LENGTH = sizeof(texts) / sizeof(texts[0]);
    const int MARGIN = 15;
    const int LINE_SPACING = 30;
    const int LINE_HEIGHT = 35;
    int current_y = 10;

    for (int i = 0; i < TEXTS_LENGTH; i++) {
        int text_gap = 0;
        TTF_Font *font;

        //change text gap and font if title
        if (i == 0) {
            font = title_font;
            text_gap = 20;
        } else {
            font = text_font;
        }

        //text
        SDL_FRect text_rect = render_text(renderer, font, texts[i], MARGIN, current_y, COLORS.WHITE);

        //render corresponding data to text
        char data_text[20];
        switch (i) {
            case 1:
                sprintf(data_text, "%d", data.games_played);
                break;
            case 2:
                sprintf(data_text, "%d", data.wins);
                break;
            case 3:
                sprintf(data_text, "%d", data.current_streak);
                break;
            case 4:
                sprintf(data_text, "%d", data.best_streak);
                break;
            default:
                sprintf(data_text, "NULL");
                break;
        }
        if (strcmp(data_text, "NULL") != 0) {
            render_text(renderer, font, data_text, WIDTH - 40, current_y, COLORS.WHITE);
        }
    
        current_y += LINE_SPACING - 20;
        current_y += text_rect.h + 5;

        //render line
        SDL_SetRenderDrawColor(renderer, COLORS.WHITE.r, COLORS.WHITE.g, COLORS.WHITE.b, COLORS.WHITE.a);
        SDL_RenderLine(renderer, MARGIN, current_y, WIDTH - MARGIN, current_y);

        current_y += LINE_SPACING + text_gap;
    }

    TTF_CloseFont(title_font);
    TTF_CloseFont(text_font);
}

void render_background_with_exit_button(SDL_Renderer *renderer, SDL_FRect *exit_rect) {
    render_rect(renderer, 0, 0, WIDTH, HEIGHT, COLORS.MENU_BACKGROUND, true);
    *exit_rect = render_texture(renderer, "assets/exit.png", WIDTH - 70, 10, 60, 30);
}

void clear_grid() {
    //clear square grid & word grid
    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        for (int j = 0; j < MAX_WORD_LENGTH; j++) {
            grid_colors[i][j] = COLORS.DARK_GRAY;
            letter_grid[i][j] = '0';
            focused_attempt = 1;
            focused_letter_number = 1;
        }
    }

    //clear keyboard grid
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            keyboard_colors[i][j] = COLORS.LIGHT_GRAY;
        }
    }
}

void check_word_and_start_next_attempt(SDL_Renderer *renderer) {
    int current_attempt = focused_attempt - 1;
    bool valid_word = false;

    //check if word is in word list & if inputted word is complete
    char lowercase_input[word_length + 1];

    for (int i = 0; i < word_length; i++) {
        //stop if letter is not set
        if (letter_grid[current_attempt][i] == '0') {
            change_error_message_and_call_timer("Word has missing letters!");
            return;
        }

        lowercase_input[i] = tolower(letter_grid[current_attempt][i]);
    }

    lowercase_input[word_length] = '\0';
    char first_letter = letter_grid[current_attempt][0];
    const char **word_list = ALL_WORDS[first_letter - 'A'];

    for (int i = 0; i < SIZES[first_letter - 'A']; i++) {
        if (strcmp(lowercase_input, word_list[i]) == 0) {
            valid_word = true;
            break;
        }
    }

    //if word is valid, color letters accordingly
    if (current_attempt >= 0 && current_attempt < tries && valid_word) {
        bool letter_used[word_length];
        memset(letter_used, false, sizeof(letter_used));

        //check for exact matches (GREEN)
        for (int i = 0; i < word_length; i++) {
            if (letter_grid[current_attempt][i] == word_answer[i]) {
                grid_colors[current_attempt][i] = COLORS.GREEN;
                letter_used[i] = true;
            } else {
                grid_colors[current_attempt][i] = COLORS.GRAY;
            }
        }

        //check for present letters (YELLOW)
        for (int i = 0; i < word_length; i++) {
            if (grid_colors[current_attempt][i].r == COLORS.GREEN.r &&
                grid_colors[current_attempt][i].g == COLORS.GREEN.g &&
                grid_colors[current_attempt][i].b == COLORS.GREEN.b &&
                grid_colors[current_attempt][i].a == COLORS.GREEN.a) {
                continue; //skip matched letters
            }

            for (int j = 0; j < word_length; j++) {
                if (!letter_used[j] && letter_grid[current_attempt][i] == word_answer[j]) {
                    grid_colors[current_attempt][i] = COLORS.YELLOW;
                    letter_used[j] = true;
                    break;
                }
            }
        }
        
        //change keyboard letter color to green/yellow/gray
        for (int i = 0; i < word_length; i++) {
            char letter = letter_grid[current_attempt][i];

            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 10; k++) {
                    if (QWERTY[j][k] == letter) {
                        //only update the color if it's a higher priority
                        if (grid_colors[current_attempt][i].r == COLORS.GREEN.r &&
                            keyboard_colors[j][k].r != COLORS.GREEN.r) {
                            keyboard_colors[j][k] = COLORS.GREEN;
                        } else if (grid_colors[current_attempt][i].r == COLORS.YELLOW.r &&
                                keyboard_colors[j][k].r != COLORS.GREEN.r &&
                                keyboard_colors[j][k].r != COLORS.YELLOW.r) {
                            keyboard_colors[j][k] = COLORS.YELLOW;
                        } else if (grid_colors[current_attempt][i].r == COLORS.GRAY.r &&
                                keyboard_colors[j][k].r != COLORS.GREEN.r &&
                                keyboard_colors[j][k].r != COLORS.YELLOW.r) {
                            keyboard_colors[j][k] = COLORS.GRAY;
                        }
                    }
                }
            }
        }

        //next attempt
        focused_attempt++;
        focused_letter_number = 1;

        //if word is correct, show message & reset game
        bool right_answer = true;
        for (int i = 0; i < word_length; i++) {
            if (grid_colors[current_attempt][i].r != COLORS.GREEN.r) {
                right_answer = false;
                break;
            }
        }
        //win
        if (right_answer) {
            //update data
            data.wins++;
            data.current_streak++;
            data.games_played++;

            if (data.current_streak > data.best_streak) {
                data.best_streak = data.current_streak;
            }

            game_ended = true;
        }
        //lose
        else if (focused_attempt > tries) {
            //error message
            char message[50];
            sprintf(message, "You lose! The word was %s", word_answer);
            change_error_message_and_call_timer(message);

            //update data
            data.current_streak = 0;
            data.games_played++;

            game_ended = true;
        }
    }
    else if (!valid_word) {
        change_error_message_and_call_timer("Not a valid word!");
    }
}

void change_focus_square_position(bool going_positive) {
    //self-explanatory
    if (going_positive) {
        if (focused_letter_number != word_length) {
            focused_letter_number++;
        }
    }
    else {
        if (focused_letter_number != 1) {
            focused_letter_number--;
        }
    }
}

void generate_new_word() {
    int random_array_index = rand() % ARRAY_LISTS;
    int random_word_index = rand() % SIZES[random_array_index];
    sprintf(word_answer, ALL_WORDS[random_array_index][random_word_index]);

    //convert to uppercase
    for (int i = 0; word_answer[i] != '\0'; i++) {
        word_answer[i] = toupper(word_answer[i]);
    }
}

void restart_game(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
    SDL_Delay(2000); //2 second delay
    clear_grid();
    generate_new_word();
}

void change_error_message_and_call_timer(char *message) {
    snprintf(error_message, sizeof(error_message), message);
    window_bools.error_message = true;
    start_timer(ERROR_MESSAGE_DISPLAY_TIME_MILLI);
}

void start_timer(Uint32 duration) {
    timer_start = SDL_GetTicks();
    timer_duration = duration;
    timer_running = true;
}

void open_or_close_menu(float mouse_x, float mouse_y) {
    bool *menu_states[MENU_COUNT] = {&window_bools.help_menu, &window_bools.settings_menu, &window_bools.stats_menu};
    SDL_FRect *menus[MENU_COUNT] = {&buttons.help_button, &buttons.settings_button, &buttons.stats_button};
    SDL_FRect *exit_buttons[MENU_COUNT] = {&buttons.help_exit_button, &buttons.settings_exit_button, &buttons.stats_exit_button};

    for (int i = 0; i < MENU_COUNT; i++) {
        if (*menu_states[i] && mouse_in_rect(mouse_x, mouse_y, *exit_buttons[i])) {
            //close menu
            *menu_states[i] = false;
            return;
        }
        else if (!*menu_states[i] && mouse_in_rect(mouse_x, mouse_y, *menus[i])) {
            //open menu
            *menu_states[i] = true;
            return;
        }
    }
}

void update_stats() {
    FILE *file = fopen("build/stats.csv", "w");
    if (file == NULL) {
        printf("Error opening file: build/stats.csv\n");
        return;
    }

    fprintf(file, "Games Played,Wins,Current Streak,Best Streak\n");
    fprintf(file, "%d,%d,%d,%d\n", data.games_played, data.wins, data.current_streak, data.best_streak);

    fclose(file);
}

int* get_stats() {
    int *stats = malloc(5 * sizeof(int));
    if (!stats) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    FILE *file = fopen("build/stats.csv", "r");
    if (!file) {
        file = fopen("stats.csv", "r");
    }
    if (file == NULL) {
        printf("Error opening file: build/stats.csv\n");
        free(stats);
        return NULL;
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("Error reading header\n");
        fclose(file);
        free(stats);
        return NULL;
    }

    if (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "%d,%d,%d,%d", &stats[0], &stats[1], &stats[2], &stats[3]) != 4) {
            printf("Error parsing stats data\n");
            free(stats);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return stats;
}

bool mouse_in_rect(int mouse_x, int mouse_y, SDL_FRect rect) {
    return (mouse_x >= rect.x && mouse_x <= rect.x + rect.w &&
            mouse_y >= rect.y && mouse_y <= rect.y + rect.h);
}