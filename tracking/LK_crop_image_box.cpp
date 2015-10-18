#include "bbox.h"
#include "tracker.h"
#include "util.h"
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<armadillo>
#include<cmath>
#include<utility>
#include<algorithm>
using namespace std;
using namespace cv;

int LK_crop_image_box(cv::Mat &I_crop,Bbox &BB_crop,Bbox &bb_crop,pair<double,double>&s,cv::Mat I,Bbox BB,Tracker tracker)
{
	s = make_pair(tracker.std_box(1)/(BB.x2-BB.x1+1.0),tracker.std_box(2)/(BB.y2-BB.y1+1));
	arma::uvec bb_scale(floor(BB.x1*s.first),floor(BB.y1*s.second),floor(BB.x2*s.first),floor(BB.y2*s.second));
	bb_scale(3) = bb_scale(1) + tracker.std_box(1) - 1;
	bb_scale(4) = bb_scale(2) + tracker.std_box(2) - 1; 
	cv::Mat I_scale;
	Size imsize = Size(floor(I.cols*s.first),floor(I.rows*s.second));
	resize(I,I_scale,imsize);
	bb_crop = bb_rescale_relative(bb_scale,tracker.enlarge_box);
	I_crop = im_crop(I_scale,bb_crop);
	BB_crop = bb_shift_absolute(bb_scale, make_pair(-bb_crop(1),-bb_crop(2)));
}

int LK_crop_image_box(cv::Mat &I_crop,Bbox &BB_crop,cv::Mat I,Bbox BB,Tracker tracker)
{
	pair<double,double>s = make_pair(tracker.std_box(1)/(BB.x2-BB.x1+1.0),tracker.std_box(2)/(BB.y2-BB.y1+1));
	arma::uvec bb_scale(floor(BB.x1*s.first),floor(BB.y1*s.second),floor(BB.x2*s.first),floor(BB.y2*s.second));
	bb_scale(3) = bb_scale(1) + tracker.std_box(1) - 1;
	bb_scale(4) = bb_scale(2) + tracker.std_box(2) - 1; 
	cv::Mat I_scale;
	Size imsize = Size(floor(I.cols*s.first),floor(I.rows*s.second));
	resize(I,I_scale,imsize);
	bb_crop = bb_rescale_relative(bb_scale,tracker.enlarge_box);
	I_crop = im_crop(I_scale,bb_crop);
	BB_crop = bb_shift_absolute(bb_scale, make_pair(-bb_crop(1),-bb_crop(2)));
}