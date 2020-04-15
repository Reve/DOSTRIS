#include "SCREEN.H"

#ifndef DEF_TEXT
#define DEF_TEXT 1

void text_init(void);
void set_font(int font_id);
void draw_char(screen_t *s, int c, int x, int y, int fgcolor, int bgcolor);
void draw_string(screen_t *s, char *c, int l, int x, int y, int fg, int bg);

#endif