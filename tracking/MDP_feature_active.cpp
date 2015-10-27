#include<armadillo>
#include"tracker.h"
#include"dres_det.h"
using namespace arma;
mat MDP_feature_active(Tracker tracker,Dres_det dres)
{
	int num = dres.fr.size();
	mat f = zeros<mat>(num,tracker.fnum_active);
	f.col(1) = dres.x / tracker.image_width;
	f.col(2) = dres.y / tracker.image_height;
	f.col(3) = dres.w / tracker.max_width;
	f.col(4) = dres.h / tracker.max_height;
	f.col(5) = dres.r / tracker.max_score;
	f.col(6) = 1;
	return f;
}