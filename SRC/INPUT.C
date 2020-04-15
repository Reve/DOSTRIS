#include <dos.h>
#include "INPUT.H"

BYTE gb_scan;
BYTE gb_scan_q[NUM_SCAN_QUE];
BYTE gb_scan_head;
BYTE gb_scan_tail;

int i_head = 0;
int i_tail = 0;

event_t in[MAX_INPUT];

void interrupt get_scan(void)
{
	asm cli

	asm	{
		in al, 060h
		mov gb_scan, al
		in al, 061h
		mov bl, al
		or al, 080h
		out 061h, al
		mov al, bl
		out 061h, al

		mov al, 020h
		out 020h, al

		sti
		}

	*(gb_scan_q + gb_scan_tail) = gb_scan;
	++gb_scan_tail;
}

void init_keyboard(void)
{
	BYTE far *bios_key_state;

	oldkb = getvect(9);

	bios_key_state = MK_FP(0x040, 0x017);
	*bios_key_state &= (~(32 | 64));

	oldkb();

	gb_scan_head = 0;
	gb_scan_tail = 0;
	gb_scan = 0;

	setvect(9, get_scan);
}

void deinit_keyboard(void)
{
	setvect(9, oldkb);
}

void add_input(event_t *event)
{
	in[i_tail].type = event->type;
	in[i_tail].sub_type = event->sub_type;
	in[i_tail].x = event->x;
	in[i_tail].y = event->y;
	in[i_tail].data1 = event->data1;
	in[i_tail].data2 = event->data2;

	i_tail++;

	if (i_tail == MAX_INPUT)
		i_tail = 0;

	if (i_tail == i_head)
	{
		i_head++;

		if (i_head == MAX_INPUT)
			i_head = 0;
	}
}

int check_input(event_t *event)
{
	int is_event = 0;
	event_t new_event;

	while (gb_scan_head != gb_scan_tail)
	{
		new_event.type = KEY;
		new_event.data1 = gb_scan_q[gb_scan_head];

		if (new_event.data1 == 0xe0)
		{
			gb_scan_head++;
			continue;
		}

		gb_scan_head++;

		if (new_event.data1 & KEY_UP_MASK)
			new_event.sub_type = KEY_UP;
		else
			new_event.sub_type = KEY_DOWN;

		new_event.data1 &= KEY_ALL_MASK;

		add_input(&new_event);
	}

	if (i_head != i_tail)
	{
		is_event = 1;

		event->type = in[i_head].type;
		event->sub_type = in[i_head].sub_type;
		event->x = in[i_head].x;
		event->y = in[i_head].y;
		event->data1 = in[i_head].data1;
		event->data2 = in[i_head].data2;

		i_head++;

		if (i_head == MAX_INPUT)
			i_head = 0;
	}

	return is_event;
}