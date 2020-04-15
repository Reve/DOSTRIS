#include "DRAW.H"

void draw_pixel(screen_t *s, int x, int y, int color)
{
	*(s->off_screen + y * s->screen_width + x) = color;
}

int get_pixel(screen_t *s, int x, int y)
{
	return *(s->off_screen + y * s->screen_width + x);
}

void draw_vert_line(screen_t *s, int x, int y, int length, int color)
{
	int i;

	for (i = 0; i <= length; i++) {
		draw_pixel(s, x, y + i, color);
	}
}

void draw_horiz_line(screen_t *s, int x, int y, int length, int color)
{
	int i;

	for (i = 0; i <= length; i++) {
		draw_pixel(s, x + i, y, color);
	}
}

void draw_rect(screen_t *s, int x, int y, int h, int w, int color)
{
	int i;

	for (i = 0; i <= h; i++) {
		draw_horiz_line(s, x, y + i, w, color);
	}
}

void draw_rect_outline(screen_t *s, int x, int y, int h, int w, int color)
{
	draw_horiz_line(s, x, y, w, color);
	draw_horiz_line(s, x, y + h, w, color);

	draw_vert_line(s, x, y, h, color);
	draw_vert_line(s, x + w, y, h, color);
}

void draw_block(screen_t *s, int x, int y, int h, int w, int color)
{
	draw_rect(s, x, y, h, w, color);
	draw_horiz_line(s, x + 1, y + 8, w - 2, LIGHT_GRAY);
	//draw_horiz_line(s, x + 1, y + h - 1, w - 1, DARK_GRAY);
}

void draw_lane(screen_t *s, int lane[LANE_HEIGHT * LANE_WIDTH], int x, int y)
{
	int i, j;

	for (i = 0; i < LANE_HEIGHT; i++)
	{
		for (j = 0; j < LANE_WIDTH; j++)
		{
			if (lane[i * LANE_WIDTH + j] != 0)
			{
				draw_block(s, x + j * BLOCK_SIZE, y + i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, lane[i * LANE_WIDTH + j]);
			}
			else
			{
				draw_rect(s, x + j * BLOCK_SIZE, y + i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
			}
		}
	}
}

void draw_shape(screen_t *s, Piece *p)
{
	int px, py;

	for (px = 0; px < 4; px++)
	{
		for (py = 0; py < 4; py++)
		{
			int pi = calc_rotation(px, py, p->rotation);

			if (p->shape[pi] != 0)
				draw_block(s,
					p->pos.x + px * BLOCK_SIZE,
					p->pos.y + py * BLOCK_SIZE,
					BLOCK_SIZE,
					BLOCK_SIZE,
					p->shape[pi]);
		}
	}
}