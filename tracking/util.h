#ifndef UTIL_H
#define UTIL_H

#include "opt.h"
#include "bbox.h"
#include "dres_det.h"
#include "tracker.h"
#include<armadillo>
#include<iostream>
#include<vector>
#include<opencv2/core/core.hpp>
using namespace std;

int global(Opt *tmp);

cv::Mat im_crop(cv::Mat img,Bbox bb);

int LK_crop_image_box(Mat &I_crop,Bbox &BB_crop,Bbox &bb_crop,pair<double,double>&s,Mat I,Bbox BB,Tracker tracker)

int LK_crop_image_box(cv::Mat &I_crop,Bbox &BB_crop,cv::Mat I,Bbox BB,Tracker tracker);

Bbox bb_rescale_relative(Bbox BB,pair<double,double>s);

Bbox bb_shift_absolute(Bbox bb,pair<int,int>shift);

Dres_det MDP_crop_image_box(Dres_det dres_det,cv::Mat I,Tracker tracker);

int MDP_initialize_test(Tracker *tracker,int image_width,int image_height,Dres_det *dres_det,int is_show);

mat bb_shift_relative(mat bb,pair<double,double>shift);
#endif