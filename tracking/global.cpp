#include <direct.h>
#include "opt.h";

int global(Opt &tmp)
{
	//opt *tmp = new opt();
	tmp.mot = "../../evaluation/data/";
	tmp.mot2d = "2DMOT2015";
	tmp.results = "results";

	tmp.mot2d_train_seqs[0] = "TUD-Stadtmitte";
	tmp.mot2d_train_seqs[1] = "TUD-Campus";
	tmp.mot2d_train_seqs[2] = "PETS09-S2L1";
	tmp.mot2d_train_seqs[3] = "ETH-Bahnhof";
	tmp.mot2d_train_seqs[4] = "ETH-Sunnyday";
	tmp.mot2d_train_seqs[5] = "ETH-Pedcross2";
	tmp.mot2d_train_seqs[6] = "ADL-Rundle-6";
	tmp.mot2d_train_seqs[7] = "ADL-Rundle-8";
	tmp.mot2d_train_seqs[8] = "KITTI-13";
	tmp.mot2d_train_seqs[9] = "KITTI-17";
	tmp.mot2d_train_seqs[10] = "Venice-2";
	
	tmp.mot2d_train_nums[0] = 179;
	tmp.mot2d_train_nums[1] = 71;
	tmp.mot2d_train_nums[2] = 795;
	tmp.mot2d_train_nums[3] = 1000;
	tmp.mot2d_train_nums[4] = 354;
	tmp.mot2d_train_nums[5] = 840;
	tmp.mot2d_train_nums[6] = 525;
	tmp.mot2d_train_nums[7] = 654;
	tmp.mot2d_train_nums[8] = 340;
	tmp.mot2d_train_nums[9] = 145;
	tmp.mot2d_train_nums[10] = 600;

	tmp.mot2d_test_seqs[0] = "TUD-Crossing";
	tmp.mot2d_test_seqs[1] = "PETS09-S2L2";
	tmp.mot2d_test_seqs[2] = "ETH-Jelmoli";
	tmp.mot2d_test_seqs[3] = "ETH-Linthescher";
	tmp.mot2d_test_seqs[4] = "ETH-Crossing";
	tmp.mot2d_test_seqs[5] = "AVG-TownCentre";
	tmp.mot2d_test_seqs[6] = "ADL-Rundle-1";
	tmp.mot2d_test_seqs[7] = "ADL-Rundle-3";
	tmp.mot2d_test_seqs[8] = "KITTI-16";
	tmp.mot2d_test_seqs[9] = "KITTI-19";
	tmp.mot2d_test_seqs[10] = "Venice-1";

	tmp.mot2d_test_nums[0] = 201;
	tmp.mot2d_test_nums[1] = 436;
	tmp.mot2d_test_nums[2] = 440;
	tmp.mot2d_test_nums[3] = 1194;
	tmp.mot2d_test_nums[4] = 219;
	tmp.mot2d_test_nums[5] = 450;
	tmp.mot2d_test_nums[6] = 500;
	tmp.mot2d_test_nums[7] = 625;
	tmp.mot2d_test_nums[8] = 209;
	tmp.mot2d_test_nums[9] = 1059;
	tmp.mot2d_test_nums[10] = 450;

	int flag = mkdir(tmp.results.c_str());
	if(flag)
	{
		printf("unable to create folder\n");
		return -1;
	}

	tmp.num = 10;
	tmp.fb_factor = 30;
	tmp.threshold_ratio = 0.6;
	tmp.threshold_dis = 3;
	tmp.threshold_box = 0.8;
	tmp.std_box = make_pair(30,60);
	tmp.margin_box = make_pair(5,2);
	tmp.enlarge_box = make_pair(5,3);
	tmp.level_track = 1;
	tmp.level = 1;
	tmp.max_ratio = 0.9;
	tmp.min_vnorm = 0.2;
	tmp.overlap_box = 0.5;
	tmp.patchsize = make_pair(24,12);
	tmp.weight_tracking = 1;
	tmp.weight_association = 1;

	tmp.overlap_occ = 0.7;
	tmp.overlap_pos = 0.5;
	tmp.overlap_neg = 0.2;
	tmp.overlap_sup = 0.7;

	tmp.max_iter = 10000;
	tmp.max_count = 10;
	tmp.max_pass = 2;

	tmp.max_occlusion = 50;
	tmp.exit_threshold = 0.95;
	tmp.tracked = 5;

	return 0;
}