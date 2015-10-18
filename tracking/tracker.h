#ifndef TRACKER_H
#define TRACKER_H

#include "dres_det.h"
#include<opencv2\core\core.hpp>
struct Tracker
{
	int target_id;
	int num;
	int anchor;

	int image_width;
	int image_height;
	int max_width;
	int max_height;
	int max_score;

	int is_show;
	int streak_tracked;
	int streak_occluded;
	int state;

	Dres_det dres;
	vec bb;
	uvec frame_ids;

	vec x1;
	vec y1;
	vec x2;
	vec y2;

	Col<Mat>Is;
	Col<bbox>BBs;

	vec bb_overlaps;

};

#endif