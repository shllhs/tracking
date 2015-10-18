#include "tracker.h"
#include "dres_det.h"
#include<vector>
#include<armadillo>
using namespace arma;

int MDP_initialize_test(Tracker *tracker,int image_width,int image_height,Dres_det *dres_det,int is_show)
{
	tracker->image_width = image_width;
	tracker->image_height = image_height;
	tracker->max_width = max(dres_det->w);
	tracker->max_height = max(dres_det->h);

	tracker->streak_tracked = 0;
	tracker->is_show = is_show;
}