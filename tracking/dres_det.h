#ifndef DRES_DET_H
#define DRES_DET_H

#include<armadillo>
using namespace arma;
struct Dres_det
{
	uvec fr;
	uvec id;
	vec x;
	vec y;
	vec w;
	vec h;
	vec r;
	uvec state;
	vec ratios;
	vec distances;
};

#endif