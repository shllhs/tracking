#ifndef OPT_H
#define OPT_H

#include<utility>
#include<cstring>
#include<string>
using namespace std;
struct Opt
{
	string root;
	string mot;
	string mot2d;
	string results;
	
	string mot2d_train_seqs[15];
	int mot2d_train_nums[15];
	string mot2d_test_seqs[15];
	int mot2d_test_nums[15];

	//tracking parameters
	int num;
	int fb_factor;
	double threshold_ratio;
	double threshold_dis;
	double threshold_box;
	std::pair<int,int>std_box;
	std::pair<int,int>margin_box;
	std::pair<int,int>enlarge_box;
	int level_track;
	int level;
	double max_ratio;
	double min_vnorm;
	double overlap_box;
	pair<int,int>patchsize;
	double weight_tracking;
	double weight_association;

	//parameters for generating training data
	double overlap_occ;
	double overlap_pos;
	double overlap_neg;
	double overlap_sup;

	//training parameters
	int max_iter;
	int max_count;
	int max_pass;

	//parameters to transite to inactive
	double max_occlusion;
	double exit_threshold;
	int tracked;
	int is_text;
};

#endif