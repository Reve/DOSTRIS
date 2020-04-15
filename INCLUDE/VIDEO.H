#include "SCREEN.H"

#ifndef DEF_VIDEO
#define DEF_VIDEO 1

void enter_mode13h(void);
void leave_mode13h(void);
int init_video_mode(screen_t *s);
void exit_video_mode(screen_t *s);
void update_buffer(screen_t *s);

#endif
