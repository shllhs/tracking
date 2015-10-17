#include "opt.h"
#include "tracker.h"
#include "global.h"
#include<string>
int MDP_test(int seq_idx,string seq_set,Tracker *track)
{
	Opt *opt = (Opt*)malloc(sizeof(Opt));
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
		seq_name = opt->mot2d_train_seqs[seq_idx];
		seq_num = opt->mot2d_train_nums[seq_idx];
	}
	else
	{
		seq_name = opt->mot2d_test_seqs[seq_idx];
		seq_num = opt->mot2d_test_nums[seq_idx];
	}


}