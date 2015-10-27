#include "opt.h"
#include "tracker.h"
#include "dres_det.h"
#include "dres_image.h"
#include "util.h"
#include<string>
#include<vector>
#include<iostream>
#include<cstdio>
#include<opencv2/core/core.hpp>
#include<armadillo>
using namespace std;
using namespace arma;
const int SIZE = 100000;

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
	Dres_det dres_track;

	//intialize tracker
	cv::Mat I = dres_image.I[1];
	tracker = MDP_initialize_test(tracker,I.rows,I.cols,dres_det,is_show);

	// for each frame
	Col<Tracker>trackers;
	int id = 0;

	Dres_det dres_associate;
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
					generate_initial_index(index,trackers(index_track(span(1,i-1))),dres);
					dres_associate = sub(dres,index);
					int flag = associate(fr, dres_image,  dres_associate, trackers(ind), opt);
					if(flag!=1)printf("associate failed!\n");
				}
			}
			else if(trackers(ind).state == 3)
			{
				generate_initial_index(index,trackers(index_track(span(1,i-1))),dres);
				dres_associate = sub(dres,index);
				int flag = associate(fr, dres_image,  dres_associate, trackers(ind), opt);
				if(flag!=1)printf("associate failed!\n");
			}
		}

		//find detections for initialization
		generate_initial_index(index,trackers, dres);
		Dres_det dres_one;

		for(int i = 1;i<=index.size();i++)
		{
			// extract features
			dres_one = sub(dres, (uvec)index(i));
			mat f = MDP_feature_active(tracker, dres_one);
			// prediction
			int label = svmpredict(1, f, tracker.w_active, '-q');
			// make a decision
			if(label<0)continue;
			//reset tracker
			tracker.prev_state = 1;
			tracker.state = 1;            
			id = id + 1;
        
			Col<Tracker> tmp;
			tmp<<initialize(fr, dres_image, id, dres, index(i), tracker);
			trackers.insert_rows(trackers.size(),tmp);
		}

		//resolve tracker conflict
		int flag = resolve(trackers, dres, opt);    
		if(flag!=0)
			printf("resolve tracker conflict failed!\n");
		
		dres_track = generate_results(trackers);
	}
	//write tracking results
	string filename = opt.results+"/"+seq_name+".txt";
	printf("write results: %s\n", filename.c_str());
	write_tracking_results(filename, dres_track, opt.tracked);
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

		vec ov;
		calc_overlap(ov,tracker.dres,tracker.dres.fr.size(),dres_image,fr);
		if(ov(1)<opt.exit_threshold)
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
		 int flag = generate_association_index(dres_associate,index_det,tracker, fr, dres_associate);
		 if(flag!=0)
			 printf("generate association index failed!\n");

		 tracker = MDP_value(tracker, fr, dres_image, dres_associate, index_det);
		 if(tracker.state == 2)
		 {
			 tracker.streak_occluded = 0;
		 }

		 if(tracker.streak_occluded>opt.max_occlusion)
		 {
			 tracker.state = 0;
			 if(opt.is_text)
			 {
				printf("target %d exits due to long time occlusion\n", tracker.target_id);
			 }
		 }

		 //check if target outside image
		 vec ov;
		 calc_overlap(ov,tracker.dres,tracker.dres.fr.size(), dres_image, fr);
		 if(ov(1)<opt.exit_threshold)
		 {
			 if(opt.is_text)
			 {
				printf("target outside image by checking boarders\n");
			 }
			 tracker.state = 0;
		 }
	}
}

//resolve conflict between trackers
int resolve(Col<Tracker> trackers,Dres_det dres_det,Opt opt)
{
	Dres_det dres_track;
	for(int i = 1;i<=trackers.size();i++)
	{
		Tracker tracker = trackers(i);
		Dres_det dres = sub(tracker.dres,tracker.dres.fr.size());

		if(tracker.state == 2)
		{
			if(dres_track.fr.is_empty())
				dres_track = dres;
			else 
				dres_track = concatentate_dres(dres_track,dres);
		}
	}

	//compute overlaps
	int num_det = dres_det.fr.size();
	int num_track;
	if(dres_track.fr.is_empty())
		num_track = 0;
	else 
		num_track = dres_track.fr.size();

	uvec flag = zeros<uvec>(num_track);
	for(int i = 1;i<=num_track;i++)
	{
		vec o,oo;
		calc_overlap(oo,o,dres_track,i,dres_track,linearspace<uvec>(1,num_track,1));
		o(i) = 0;
		o(flag == 1) = 0;
		int ind;
		double mo = o.max(ind);
		if(mo>opt.overlap_sup)
		{
			int num1 = trackers(dres_track.id(i)).streak_tracked;
			int num2 = trackers(dres_track.id(ind)).streak_tracked;
			calc_overlap(o,dres_track, i, dres_det,linspace<uvec>(1,num_det,1));
			calc_overlap(oo,dres_track, ind, dres_det,linspace<uvec>(1,num_det,1));
			double o1 = max(o);
			double o2 = max(oo);

			int sup;
			if(num1>num2)
				sup = ind;
			else if(num1<num2)
				sup = i;
			else
			{
				if(o1>o2)
					sup = ind;
				else
					sup = i;
			}

			trackers(dres_track.id(sup)).state = 3;
			int num =  trackers(dres_track.id(sup)).dres.state.size();
			trackers(dres_track.id(sup)).dres.state(num) = 3;
			if(opt.is_text)
			{
				printf("target %d suppressed\n", dres_track.id(sup));
			}
			flag(sup) = 1;
		}
	}
	return 0;
}