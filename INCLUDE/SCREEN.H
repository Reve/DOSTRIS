#ifndef DEF_SCREEN
#define DEF_SCREEN

typedef struct {
    unsigned char far *screen;
    unsigned char far *off_screen;
    int screen_width, screen_height;
    unsigned int screen_size;
} screen_t;

#endif