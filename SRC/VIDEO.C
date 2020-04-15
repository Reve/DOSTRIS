#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <malloc.h>
#include "VIDEO.H"

#define INPUT_STATUS_0 0x3da

int old_mode;

void enter_mode13h(void)
{
	union REGS in, out;

	in.h.ah = 0xf;
	int86(0x10, &in, &out);
	old_mode=out.h.al;

	in.h.ah = 0;
	in.h.al = 0x13;
	int86(0x10, &in, &out);
}

void leave_mode13h(void)
{
	union REGS in, out;

	in.h.ah = 0;
	in.h.al = old_mode;
	int86(0x10, &in, &out);
}

int init_video_mode(screen_t *s)
{
    s->off_screen = farmalloc(64000u);

	if (s->off_screen)
	{
		s->screen = MK_FP(0xa000, 0);
		s->screen_width = 320;
		s->screen_height = 200;
		s->screen_size = 64000u;
		enter_mode13h();
		_fmemset(s->off_screen, 0, s->screen_size);
		return 0;
	}
	else
	{
		leave_mode13h();
		printf("Out of mem!\n");
		return 1;
	}
}

void exit_video_mode(screen_t *s)
{
    leave_mode13h();
    farfree(s->off_screen);
}

void update_buffer(screen_t *s)
{
	while (inportb(INPUT_STATUS_0) & 8);
	while (!inportb(INPUT_STATUS_0) & 8);

	_fmemcpy(s->screen, s->off_screen, s->screen_size);
}
