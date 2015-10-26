#include "opt.h"
#include "tracker.h"
#include "dres_det.h"
#include "dres_image.h"
#include "util.h"
#include<string>
#include<vector>
#include<opencv2/core/core.hpp>
#include<armadillo>
using namespace std;
int MDP_test(int seq_idx,string seq_set,Tracker *track)
{
	Opt opt;
	int flag = global(opt);
	if(flag == -1)
	{
		printf("initialize failed\n");
		return -1;
	}

	string seq_name;
	int seq_num;
	if(seq_set == "train")
	{
		seq_name = opt.mot2d_train_seqs[seq_idx];
		seq_num = opt.mot2d_train_nums[seq_idx];
	}
	else
	{
		seq_name = opt.mot2d_test_seqs[seq_idx];
		seq_num = opt.mot2d_test_nums[seq_idx];
	}

	string filename = opt.mot+opt.mot2d+seq_set+seq_name+"det"+"det.txt";
	Dres_det dres_det= read_mot2dres(filename);

	//intialize tracker
	cv::Mat I = dres_image.I[1];
	tracker = MDP_initialize_test(tracker,I.rows,I.cols,dres_det,is_show);

	// for each frame
	Col<Tracker>trackers;
	int id = 0;

	Dres_det dres_tmp,dres_associate;
	uvec index;
	for(int fr = 1;fr<seq_num;fr++)
	{
		//extract detection
		arma::uvec index = arma::find(dres_det.fr == fr);
		Dres_det dres = sub(dres_det,index);

		dres = MDP_crop_image_box(dres, dres_image.Igray{fr}, tracker);

		//sort trackers
		uvec index_track = sort_trackers(trackers);

		//process trackers
		for(int i = 1;i<=index_track.size();i++)
		{
			int ind = index_track(i);
			if(trackers(ind).state == 2)
			{
				track(fr,dres_image,dres,trackers(ind),opt);
				if(trackers(ind).state == 3)
				{
					generate_initial_index(dres_tmp,index,trackers(index_track(span(1,i-1))),dres);
					dres_associate = sub(dres_tmp,index);
					int flag = associate(fr, dres_image,  dres_associate, trackers(ind), opt);
					if(flag!=1)printf("associate failed!\n");
				}
			}
			else if(trackers(ind).state == 3)
			{
				generate_initial_index(dres_tmp,index,trackers(index_track(span(1,i-1))),dres);
				dres_associate = sub(dres_tmp,index);
				int flag = associate(fr, dres_image,  dres_associate, trackers(ind), opt);
				if(flag!=1)printf("associate failed!\n");
			}
		}
	}
}

Dres_det sub(Dres_det dres_det,uvec index)
{
	Dres_det dres;
	dres.fr = dres_det.fr(index);
	dres.id = dres_det.id(index);
	dres.x = dres_det.x(index);
	dres.y = dres_det.y(index);
	dres.w = dres_det.w(index);
	dres.h = dres_det.h(index);
	dres.r = dres_det.r(index);
}

uvec sort_trackers(Col<Tracker>trackers)
{
	int sep = 10;
	int num = trackers.size();
	uvec len = zeros<uvec>(num);
	uvec state = zeros<uvec>(num);

	for(int i = 0;i<num;i++)
	{
		len(i) = trackers(i).streak_tracked;
		state(i) = trackers(i).state;
	}

	uvec index1 = find(len>sep);
	uvec ind = sort_index(state(index1));
	index1 = index1(ind);

	uvec index2 = find(len<=sep);
	ind = sort_index(state(index2));
	index2 = index2(ind);

	uvec index(index1.size()+index2.size());
	for(int i = 0;i<index1.size();i++)
	{
		index(i) = index1(i);
	}
	for(int i = 0;i<index2.size();i++)
	{
		index(i+index1.size()) = index2(i);
	}
}

Tracker initialize(int fr,Dres_image dres_image,int id,Dres_det dres,int ind,Tracker tracker)
{
	if(tracker.state!=1)return;
	else
	{
		//initialize the LK tracker
		tracker = LK_initialize(tracker, fr, id, dres, ind, dres_image);
		tracker.state = 2;
		tracker.streak_occluded = 0;
		tracker.streak_tracked = 0;

		//build the dres structure
		Dres_det dres_one;
		dres_one.fr = dres.fr(ind);
		dres_one.id = tracker.target_id;
		dres_one.x = dres.x(ind);
		dres_one.y = dres.y(ind);
		dres_one.w = dres.w(ind);
		dres_one.h = dres.h(ind);
		dres_one.r = dres.r(ind);
		dres_one.state = tracker.state;
		tracker.dres = dres_one;
	}
}

void track(int fr,Dres_image dres_image,Dres_det dres,Tracker &tracker,Opt opt)
{

	if(tracker.state == 2)
	{
		tracker.streak_occluded = 0;
		tracker.streak_tracked = tracker.streak_tracked + 1;

		uvec index_det;
		MDP_value(tracker, fr, dres_image, dres, index_det);

		double ov = calc_overlap(tracker.dres,tracker.dres.fr.size(),dres_image,fr);
		if(ov<opt.exit_threshold)
		{
			if(opt.is_text)
			{
				printf("target outside image by checking boarders\n");
			}
			tracker.state = 0;
		}

	}
}

//associate a lost target
int associate(int fr, Dres_image dres_image,Dres_det dres_associate,Tracker &tracker,Opt opt)
{
	Dres_det dres_associate;
	uvec index_det;
	// occluded
	if(tracker.state == 3)
	{
		 tracker.streak_occluded = tracker.streak_occluded + 1;
		 //find a set of detections for association
		 generate_association_index(dres_associate,index_det,tracker, fr, dres_associate);
		 tracker = MDP_value(tracker, fr, dres_image, dres_associate, index_det);
		 if(tracker.state == 2)
		 {
			 tracker.streak_occluded = 0;
		 }
	}
}
