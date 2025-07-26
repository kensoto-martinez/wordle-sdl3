#ifndef RENDERING_H
#define RENDERING_H

void set_aligned_position(Alignment alignment, int *x, int *y, int w, int h);
SDL_FRect Render_Rect(int w, int h, int x, int y, Alignment alignment, SDL_Color color, bool color_fill);
SDL_FRect Render_Text(char *text, TTF_Font *font, int x, int y, Alignment alignment, SDL_Color color);
SDL_FRect Render_Image(char *image, int w, int h, int x, int y, Alignment alignment, SDL_Color color);

#endif