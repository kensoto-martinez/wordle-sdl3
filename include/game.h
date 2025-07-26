#ifndef GAME_H
#define GAME_H

//rectangle, text, and image rendering
static void set_aligned_position(Alignment alignment, int *x, int *y, int w, int h);
SDL_FRect Render_Rect(int w, int h, int x, int y, Alignment alignment, SDL_Color color, bool color_fill);
SDL_FRect Render_Text(char *text, TTF_Font *font, int x, int y, Alignment alignment, SDL_Color color);
SDL_FRect Render_Image(char *image, int w, int h, int x, int y, Alignment alignment, SDL_Color color);

//containers
Container Create_Container(float x, float y, float w, float h);
void Render_Container(Container *container, Alignment alignment);
void Add_Child_To_Container(Container *container, SDL_FRect child);
void Destroy_Container(Container *container);

void Render_Game();

#endif