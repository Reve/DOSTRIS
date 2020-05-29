#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include "CONST.H"
#include "DRAW.H"
#include "SCREEN.H"
#include "VIDEO.H"
#include "GAME.H"
#include "INPUT.H"
#include "TEXT.H"

#define ESC 27
#define TICKS (*(volatile unsigned long far *)(0x0040006CL))

typedef struct {
	int x, y, color;
} block_t;

screen_t *s;
Piece *currentPiece;
int game_over = 0;
int curX, curY;
int lane[LANE_HEIGHT * LANE_WIDTH];
int lastPos[LANE_HEIGHT * LANE_WIDTH];

void init_lane(void)
{
	int x, y;

	for (y = 0; y < LANE_HEIGHT; y++)
		for (x = 0; x < LANE_WIDTH; x++)
			lane[y * LANE_WIDTH + x] = 0;
}

void init_walls(screen_t *s)
{
	int x, y;

	for (y = 0; y < LANE_HEIGHT + 1; y++)
	{
		for (x = 0; x < LANE_WIDTH + 1; x++)
		{
			draw_block(s,
				LANE_POSX - BLOCK_SIZE,
				LANE_POSY + y * BLOCK_SIZE,
				BLOCK_SIZE,
				BLOCK_SIZE,
				WHITE);
			draw_block(s,
				LANE_POSX + LANE_WIDTH * BLOCK_SIZE,
				LANE_POSY + y * BLOCK_SIZE,
				BLOCK_SIZE,
				BLOCK_SIZE,
				WHITE);
			draw_block(s,
				LANE_POSX + (LANE_WIDTH - 1) * x,
				LANE_POSY + (LANE_HEIGHT) * BLOCK_SIZE,
				BLOCK_SIZE,
				BLOCK_SIZE,
				WHITE);

		}
	}
}

int get_field_idx(int posX, int posY, int px, int py)
{
	int fx = (posX - LANE_POSX) / BLOCK_SIZE;
	int fy = (posY - LANE_POSY) / BLOCK_SIZE;
	int fi = (fy + py) * LANE_WIDTH + (fx + px);

	return fi;
}

int check_next_move(Piece *p, int nRot, int nPosX, int nPosY)
{
	int px, py;

	for (px = 0; px < 4; px++)
	{
		for (py = 0; py < 4; py++)
		{
			int fx = (nPosX - LANE_POSX) / BLOCK_SIZE;
			int fy = (nPosY - LANE_POSY) / BLOCK_SIZE;
			int fi = get_field_idx(nPosX, nPosY, px, py);
			int pi = calc_rotation(px, py, nRot);

			if (fx + px >= 0 && fx + px < LANE_WIDTH)
			{
				if (fy + py >= 0 && fy + py < LANE_HEIGHT)
				{
					if (lane[fi] != 0 && p->shape[pi] != 0)
						return 0;
				}
			}

			if (fx + px < 0 || fx + px > LANE_WIDTH - 1)
				if (p->shape[pi] != 0)
					return 0;

			if (fy + py < 0 || fy + py > LANE_HEIGHT)
				return 0;
		}
	}

	return 1;
}

// Move the piece to the left and restore last position
void move_left(Piece *p)
{
	if (check_next_move(p, p->rotation, p->pos.x - BLOCK_SIZE, p->pos.y))
		p->pos.x = p->pos.x - BLOCK_SIZE;
}

void move_right(Piece *p)
{
	if (check_next_move(p, p->rotation, p->pos.x + BLOCK_SIZE, p->pos.y))
		p->pos.x = p->pos.x + BLOCK_SIZE;
}

int move_down(Piece *p)
{
	if (check_next_move(p, p->rotation, p->pos.x, p->pos.y + BLOCK_SIZE))
	{
		p->pos.y = p->pos.y + BLOCK_SIZE;
		return 1;
	}
	else
		return 0;
}

void rotate(Piece *p)
{
	if (check_next_move(p, p->rotation + 1, p->pos.x, p->pos.y))
		p->rotation += 1;
}

void cement_piece(Piece *p)
{
	int px, py;

	for (px = 0; px < 4; px++)
	{
		for (py = 0; py < 4; py++)
		{
			int fi = get_field_idx(p->pos.x, p->pos.y, px, py);
			int pi = calc_rotation(px, py, p->rotation);

			if (fi < LANE_WIDTH * LANE_HEIGHT && p->shape[pi] != 0)
			{
				lane[fi] = p->shape[pi];
			}
		}

	}
}

void get_next_piece(Piece *p)
{
	int r = rand() % 7;
	p->pos.x = LANE_POSX + BLOCK_SIZE * 3;
	p->pos.y = LANE_POSY + BLOCK_SIZE * 0;
	p->rotation = 0;
	p->shape = shapes[r];

	if (!check_next_move(p, p->rotation, p->pos.x, p->pos.y))
		game_over = 1;
}

unsigned long get_tick(void)
{
	return (TICKS);
}

void check_for_line(void)
{
	int x, y;

	for(y = 0; y < LANE_HEIGHT; y++)
	{
		int l = 0;
		for(x = 0; x < LANE_WIDTH; x++)
		{
			if (lane[y * LANE_WIDTH + x] != 0)
			{
				 l++;
			}
		}

		if(l == LANE_WIDTH)
		{
			// make line white
			int k;

			for(k = 0; k < LANE_WIDTH; k++)
			{
				lane[y * LANE_WIDTH + k] = WHITE;
			}

			draw_lane(s, lane, LANE_POSX, LANE_POSY);
			update_buffer(s);
			//sleep(1);

			// clear line
			for(k = 0; k < LANE_WIDTH; k++)
			{
				lane[y * LANE_WIDTH + k] = 0;
			}

			draw_lane(s, lane, LANE_POSX, LANE_POSY);
			update_buffer(s);
			//sleep(1);
			// shift upper rows down
			for (k = y - 1; k > 0; k--)
			{
				int xx;

				for (xx = 0; xx < LANE_WIDTH; xx++)
				{
					lane[(k + 1) * LANE_WIDTH + xx] = lane[k * LANE_WIDTH + xx];
				}
			}

			draw_lane(s, lane, LANE_POSX, LANE_POSY);
			update_buffer(s);
		}
	}
}

void main(void)
{
	event_t event;

	long input_next_time = 0;
	long one_second = 0;
	int fps = 0;
	char sfps[10];
	char ssfps[10];

	Piece p;

	s = malloc(sizeof(screen_t));

	init_video_mode(s);
	init_keyboard();

	text_init();
	set_font(0);

	init_lane();
	init_walls(s);

	get_next_piece(&p);

	// draw static UI components (this should be drawn once)
	// draw stats box
	draw_rect_outline(s, 30, 10, 20, 50, WHITE);
	draw_string(s, "SCORE", 3, 32, 6, WHITE, BLACK);

	// draw next box
	draw_rect_outline(s, 210, 10, 60, 90, WHITE);
	draw_string(s, "NEXT PIECE", 10, 212, 6, WHITE, BLACK);

	// draw help box
	draw_rect_outline(s, 210, 90, 60, 90, WHITE);

	while (!game_over)
	{
		check_for_line();
		draw_lane(s, lane, LANE_POSX, LANE_POSY);
		draw_shape(s, &p);
		update_buffer(s);
		fps++;

		if (check_input(&event) && get_tick() >= input_next_time)
		{
			//printf("KEY = 0x%0x %d \n", event.data1, event.data1);
			if (event.data1 == 1)
				game_over = 1;

			if (event.data1 == KEY_CODE_LEFT)
				move_left(&p);

			if (event.data1 == KEY_CODE_RIGHT)
				move_right(&p);

			if (event.data1 == KEY_CODE_DOWN)
				move_down(&p);

			if (event.data1 == KEY_CODE_UP)
				rotate(&p);

			input_next_time = get_tick() + 3;
		}

		if (get_tick() >= one_second)
		{
			if (move_down(&p) == 0)
			{
				// if there is no more free space down, cement piece
				cement_piece(&p);
				get_next_piece(&p);
			}
			sprintf(sfps, "FPS: %d", fps);
			draw_string(s, sfps, 10, 10, 60, WHITE, BLACK);
			fps = 0;
			sprintf(sfps, "");
			one_second = get_tick() + 18;
		}
	}

	exit_video_mode(s);
	free(s);
	deinit_keyboard();
}