#ifndef FUNCTIONS_H
#define FUNCTIONS_H

SDL_FRect render_rect(SDL_Renderer *renderer, int x, int y, int width, int height, SDL_Color color, bool filled);
SDL_FRect render_text(SDL_Renderer *renderer, TTF_Font *font, char *text, int x, int y, SDL_Color color);
SDL_FRect render_texture(SDL_Renderer *renderer, char *file_path, int x, int y, int width, int height);
Checkbox render_checkbox(SDL_Renderer *renderer, int x, int y, int size, bool checked);
void render_main_game(SDL_Renderer *renderer, TTF_Font *font, char new_letter);
void render_error_message(SDL_Renderer *renderer);
void render_help_menu(SDL_Renderer *renderer, TTF_Font *grid_font);
void render_settings_menu(SDL_Renderer *renderer);
void render_stats_menu(SDL_Renderer *renderer);
void render_background_with_exit_button(SDL_Renderer *renderer, SDL_FRect *exit_rect);
void clear_grid();
void check_word_and_start_next_attempt(SDL_Renderer *renderer);
void change_focus_square_position(bool going_positive);
void generate_new_word();
void restart_game(SDL_Renderer *renderer);
void change_error_message_and_call_timer(char *message);
void start_timer(Uint32 duration);
void open_or_close_menu(float mouse_x, float mouse_y);
void update_stats();
int* get_stats();
bool mouse_in_rect(int mouse_x, int mouse_y, SDL_FRect rect);

#endif