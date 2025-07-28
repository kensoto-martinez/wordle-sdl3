#ifndef GAME_H
#define GAME_H

static int init_error(char *error_cause);
int Game_Init();

//rectangle, text, and image rendering
static void set_aligned_position(Alignment alignment, int *x, int *y, int w, int h);
SDL_FRect Render_Rect(int w, int h, int x, int y, Alignment alignment, SDL_Color color, bool color_fill);
SDL_FRect Render_Text(char *text, TTF_Font *font, int x, int y, Alignment alignment, SDL_Color color);
SDL_FRect Render_Image(char *image, int w, int h, int x, int y, Alignment alignment, SDL_Color color);

//game rendering and logic
static Grid_Letter** Render_Grid();
void Render_Game();

#endif