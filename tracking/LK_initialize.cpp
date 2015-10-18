#include "tracker.h"
#include "dres_det.h"
#include "dres_image.h"
#include "bbox.h"
#include "util.h"
#include<armadillo>
#include<utility>
using namespace std;
using namespace arma;
// initialize the LK tracker
Tracker LK_initialize(Tracker tracker,int frame_id,int target_id,Dres_det dres,int ind,Dres_image dres_image)
{
	double x1 = dres.x(ind);
	double y1 = dres.y(ind);
	double x2 = dres.x(ind)+dres.w(ind);
	double y2 = dres.y(ind)+dres.h(ind);

	//template num
	int num = tracker.num;
	tracker.target_id = target_id;
	tracker.bb = zeros<vec>(4);

	//initialize all the templates
	mat A = zeros<mat>(4,1);
	A<<x1<<endr
	<<y1<<endr
	<<x2<<endr
	<<y2<<endr;

	mat bb = repmat(A,1,num);
	bb.col(2) = bb_shift_relative(bb.col(1),make_pair(-0.01,-0.01));
	bb.col(3) = bb_shift_relative(bb.col(1),make_pair(-0.01,0.01));
	bb.col(4) = bb_shift_relative(bb.col(1),make_pair(0.01,-0.01));
	bb.col(5) = bb_shift_relative(bb.col(1),make_pair(0.01,0.01));

	tracker.frame_ids = frame_id*ones<uvec>(num);
	tracker.x1 = trans(bb.row(1));
	tracker.y1 = trans(bb.row(2));
	tracker.x2 = trans(bb.row(3));
	tracker.y2 = trans(bb.row(4));
	tracker.anchor = 1;

	//initialze the images for LK association
	tracker.Is = Col<cv::Mat>(num);
	tracker.BBs = Col<Bbox>(num);
	for(int i = 0;i<num;i++)
	{
		cv::Mat I = dres_image.Igray(tracker.frame_ids(i));
		Bbox BB;
		BB.x1 = tracker.x1(i);
		BB.y1 = tracker.y1(i);
		BB.x2 = tracker.x2(i);
		BB.y2 = tracker.y2(i);

		//crop images and boxes
		cv::Mat I_crop;
		Bbox BB_crop;
		LK_crop_image_box(I_crop,BB_crop,I,BB,tracker);
		tracker.Is(i) = I_crop;
		tracker.BBs(i) = BB_crop;
	}

	//initialize the patterns
	cv::Mat img = dres_image.Igray(frame_id);
	tracker.patterns = generate_pattern(img, bb, tracker.patchsize);

	//box overlap history
	tracker.bb_overlaps = ones<vec>(num);

	//tracker resutls
	tracker.bbs = cell(num, 1);
	tracker.points = cell(num, 1);
	tracker.flags = ones<uvec>(num);
	tracker.medFBs = zeros<vec>(num);
	tracker.medFBs_left = zeros<vec>(num);
	tracker.medFBs_right = zeros<vec>(num);
	tracker.medFBs_up = zeros<vec>(num);
	tracker.medFBs_down = zeros<vec>(num);
	tracker.medNCCs = zeros<vec>(num);
	tracker.overlaps = zeros<vec>(num);
	tracker.scores = zeros<vec>(num);
	tracker.indexes = zeros<uvec>(num);
	tracker.nccs = zeros<vec>(num);
	tracker.angles = zeros<vec>(num);
	tracker.ratios = zeros<vec>(num);
}