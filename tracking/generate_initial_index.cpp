#include"dres_det.h"
#include"tracker.h"
#include"util.h"
#include<armadillo>
using namespace arma;
int generate_initial_index(uvec &index_det,Col<Tracker>trackers,Dres_det dres_det)
{
	if(dres_det.fr.is_empty())
	{
		index_det.reset();
		return 0;
	}

	// collect dres from trackers
	Dres_det dres_track;
	for(int i = 0;i<trackers.size();i++)
	{
		Tracker tracker = trackers(i);
		Dres_det dres = sub(tracker.dres,tracker.dres.fr.size());

		if(tracker.state == 2)
		{
			if(dres_track.fr.is_empty())
				dres_track = dres;
			else
				dres_track = concatenate_dres(dres_track,dres);
		}
	}

	// nms
	// bbox = [dres_det.x dres_det.y dres_det.x+dres_det.w dres_det.y+dres_det.h dres_det.r];
	// index_nms = nms(bbox, 0.5);
	// dres_det = sub(dres_det, index_nms);

	// compute overlaps
	int num_det = dres_det.fr.size();
	int num_track;
	if(dres_track.fr.is_empty())
		num_track = 0;
	else
		num_track = dres_track.fr.size();
	if(num_track!=0)
	{
		vec o1 = zeros<vec>(num_det);
		vec o2 = zeros<vec>(num_det);
		vec o,oo;
		for(int i = 0;i<num_det;i++)
		{
			calc_overlap(o,oo,dres_det, i, dres_track,linspace<uvec>(0,num_track-1,1));
			o1(i) = max(o);
			o2(i) = sum(oo);
		}
		index_det = find(o1<0.5 && o2<0.5);
	}
	else 
		index_det = linspace<uvec>(0,num_det-1,1);
}