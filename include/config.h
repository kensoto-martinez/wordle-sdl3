#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_image.h>

#define NAME "Wordle"
#define BACKGROUND_COLOR (SDL_Color){10, 10, 10, 255}
#define WIDTH 450
#define HEIGHT 600
#define FPS 30
#define FRAME_DURATION (Uint32)1000 / FPS
#define ARRAY_LISTS 26
#define TITLE_FONT_SIZE 24
#define GRID_FONT_SIZE 35
#define MAX_ATTEMPTS 8
#define MAX_WORD_LENGTH 12
#define ERROR_MESSAGE_DISPLAY_TIME_MILLI 1000
#define MENU_COUNT 3

typedef struct {
    SDL_Color WHITE;
    SDL_Color BLACK;
    SDL_Color YELLOW;
    SDL_Color RED;
    SDL_Color GREEN;
    SDL_Color DARK_GRAY;
    SDL_Color GRAY;
    SDL_Color LIGHT_GRAY;
    SDL_Color MENU_BACKGROUND;
} SDL_Colors;
extern SDL_Colors COLORS;

typedef struct {
    bool main_game;
    bool error_message;
    bool help_menu;
    bool settings_menu;
    bool stats_menu;
} WindowBools;
extern WindowBools window_bools;

typedef struct {
    SDL_FRect help_button;
    SDL_FRect settings_button;
    SDL_FRect help_exit_button;
    SDL_FRect settings_exit_button;
    SDL_FRect stats_button;
    SDL_FRect stats_exit_button;
} Buttons;
extern Buttons buttons;

typedef struct {
    int games_played;
    int wins;
    int current_streak;
    int best_streak;
} Data;
extern Data data;

typedef struct {
    SDL_FRect rect;
    bool checked;
} Checkbox;

extern const char *QWERTY[];

extern char word_answer[];
extern int tries;
extern int word_length;
extern char letter_grid[MAX_ATTEMPTS][MAX_WORD_LENGTH];
extern SDL_FRect grid[MAX_ATTEMPTS][MAX_WORD_LENGTH];
extern SDL_Color grid_colors[MAX_ATTEMPTS][MAX_WORD_LENGTH];
extern SDL_FRect keyboard_grid[3][10];
extern SDL_Color keyboard_colors[3][10];

extern int focused_attempt;
extern int focused_letter_number;

extern char error_message[40];
extern Uint32 timer_start;
extern Uint32 timer_duration;
extern bool timer_running;

extern Checkbox hard_mode;

extern const char *A_WORDS_5[];
extern const char *B_WORDS_5[];
extern const char *C_WORDS_5[];
extern const char *D_WORDS_5[];
extern const char *E_WORDS_5[];
extern const char *F_WORDS_5[];
extern const char *G_WORDS_5[];
extern const char *H_WORDS_5[];
extern const char *I_WORDS_5[];
extern const char *J_WORDS_5[];
extern const char *K_WORDS_5[];
extern const char *L_WORDS_5[];
extern const char *M_WORDS_5[];
extern const char *N_WORDS_5[];
extern const char *O_WORDS_5[];
extern const char *P_WORDS_5[];
extern const char *Q_WORDS_5[];
extern const char *R_WORDS_5[];
extern const char *S_WORDS_5[];
extern const char *T_WORDS_5[];
extern const char *U_WORDS_5[];
extern const char *V_WORDS_5[];
extern const char *W_WORDS_5[];
extern const char *X_WORDS_5[];
extern const char *Y_WORDS_5[];
extern const char *Z_WORDS_5[];
extern const int SIZES[];
extern const char **ALL_WORDS[];

#endif