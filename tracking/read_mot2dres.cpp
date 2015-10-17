#include "dres_det.h"
#include<armadillo>
using namespace arma;

int read_mot2dres(Dres_det *dres_det,char *filename)
{
	mat C;
    C.load(filename);
	dres_det->fr = C.col(1);
	dres_det->id = C.col(2);
	dres_det->x = C.col(3);
	dres_det->y = C.col(4);
	dres_det->w = C.col(5);
	dres_det->h = C.col(6);
	dres_det->r = C.col(7);
}