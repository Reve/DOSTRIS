/* Helper draw rutines
	Author: Alex Gheorghita
*/
#include "SCREEN.H"
#include "GAME.H"
#include "CONST.H"

#ifndef DEF_DRAW
#define DEF_DRAW 1

void draw_pixel(screen_t *s, int x, int y, int color);
int get_pixel(screen_t *s, int x, int y);

void draw_vert_line(screen_t *s, int x, int y, int length, int color);
void draw_horiz_line(screen_t *s, int x, int y, int length, int color);
void draw_rect(screen_t *s, int x, int y, int h, int w, int color);
void draw_rect_outline(screen_t *s, int x, int y, int h, int w, int color);
void draw_block(screen_t *s, int x, int y, int h, int w, int color);
void draw_lane(screen_t *s, int lane[LANE_HEIGHT * LANE_WIDTH], int x, int y);
void draw_shape(screen_t *s, Piece *p);

#endif