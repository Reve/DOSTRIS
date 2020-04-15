#ifndef DEF_INPUT
#define DEF_INPUT 1

#define EVENT_MASK 31
#define MAX_INPUT 30
#define KEY_UP_MASK 128
#define KEY_ALL_MASK 127
#define KEY_CODE_UP 72
#define KEY_CODE_LEFT 75
#define KEY_CODE_RIGHT 77
#define KEY_CODE_DOWN 80

#define BYTE unsigned char
#define NUM_SCAN_QUE 256

enum input_types {
	KEY
};

enum sub_input_types {
	KEY_UP,
	KEY_DOWN
};

typedef struct {
	int type, sub_type, x, y;
	int data1, data2;
} event_t;


static void interrupt (far *oldkb)(void);
void interrupt get_scan(void);
void init_keyboard(void);
void deinit_keyboard(void);
void add_input(event_t *event);
int check_input(event_t *event);

#endif