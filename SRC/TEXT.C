#include <dos.h>
#include "TEXT.H"

int CharHeight;
int CharWidth;
unsigned char far *FontPtr;
unsigned char far *F8x8Ptr;
unsigned char far *F8x14Ptr;

void text_init(void)
{
	struct REGPACK reg;

	reg.r_ax = 0x1130;
	reg.r_bx = 0x0300;

	intr(0x10, &reg);

	F8x8Ptr = MK_FP(reg.r_es, reg.r_bp);

	reg.r_ax = 0x1130;
	reg.r_bx = 0x0200;

	intr(0x10, &reg);

	F8x14Ptr = MK_FP(reg.r_es, reg.r_bp);

	FontPtr = F8x8Ptr;
	CharWidth = 8;
	CharHeight = 8;
}

void set_font(int font_id)
{
	if (font_id == 0)
	{
		FontPtr = F8x8Ptr;
		CharWidth = 8;
		CharHeight = 8;
	}
	else
	{
		if (font_id == 1)
		{
			FontPtr = F8x14Ptr;
			CharWidth = 8;
			CharHeight = 14;
		}
		else
		{
			// user defined font, not implemented
		}
	}
}

void draw_char(screen_t *s, int c, int x, int y, int fgcolor, int bgcolor)
{
	unsigned char far *p, far *fnt;
	int width, height, adj;
	unsigned char mask;

	p = s->off_screen + y * s->screen_width + x;
	adj = s->screen_width - CharWidth;

	fnt = FontPtr + c * CharHeight;
	height = CharHeight;

	while (height--)
	{
		width = CharWidth;
		mask = 128;

		while (width--)
		{
			if ((*fnt) & mask)
			{
				*p++ = fgcolor;
			}
			else
			{
				*p++ = bgcolor;
			}

			mask >>= 1;
		}

		p += adj;
		fnt++;
	}
}

void draw_string(screen_t *s, char *c, int l, int x, int y, int fg, int bg)
{
	int i;
	
	for (i = 0; i < strlen(c); i++)
	{
		draw_char(s, c[i], x + i * CharWidth, y, fg, bg);
	}
}