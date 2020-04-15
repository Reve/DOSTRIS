#ifndef DEF_GAME
#define DEF_GAME 1

typedef struct {
	int x, y;
} Vector2;

typedef struct {
	Vector2 pos;
	int rotation;
	int *shape;
} Piece;

//void get_piece(Piece *p, char type);

extern int o_shape[16];
extern int i_shape[16];
extern int j_shape[16];
extern int l_shape[16];
extern int s_shape[16];
extern int t_shape[16];
extern int z_shape[16];
extern int shapes[7];
int calc_rotation(int px, int py, int rot);

#endif