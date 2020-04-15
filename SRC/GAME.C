#include "GAME.H"

int o_shape[16] = {
	0, 0, 0, 0,
	0, 14, 14, 0,
	0, 14, 14, 0,
	0, 0, 0, 0
};
int i_shape[16] = {
	0, 0, 11, 0,
	0, 0, 11, 0,
	0, 0, 11, 0,
	0, 0, 11, 0
};
int j_shape[16] = {
	0, 0, 1, 0,
	0, 0, 1, 0,
	0, 1, 1, 0,
	0, 0, 0, 0
};
int l_shape[16] = {
	0, 6, 0, 0,
	0, 6, 0, 0,
	0, 6, 6, 0,
	0, 0, 0, 0
};
int s_shape[16] = {
	0, 0, 2, 0,
	0, 2, 2, 0,
	0, 2, 0, 0,
	0, 0, 0, 0
};
int t_shape[16] = {
	0, 0, 5, 0,
	0, 5, 5, 0,
	0, 0, 5, 0,
	0, 0, 0, 0
};
int z_shape[16] = {
	0, 4, 0, 0,
	0, 4, 4, 0,
	0, 0, 4, 0,
	0, 0, 0, 0
};

int shapes[7] = {&o_shape, &i_shape, &j_shape, &l_shape, &s_shape, &t_shape, &z_shape};

int calc_rotation(int px, int py, int rot)
{
	int pi;

	switch (rot % 4)
	{
	// 0 degrees
	case 0:
		pi = py * 4 + px;
		break;

	// 90 degrees
	case 1:
		pi = 12 + py - (px * 4);
		break;

	// 180 degrees
	case 2:
		pi = 15 - (py * 4) - px;
		break;

	// 270 degrees
	case 3:
		pi = 3 - py + (px * 4);
		break;
	}

	return pi;
}

//void get_piece(Piece *p, char type)
//{
	//if (type == 't')
	//{
		//p->shape = t_shape;
	//}
//}