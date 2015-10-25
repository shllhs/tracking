#include "tracker.h"
#include "dres_image.h"
#include "dres_det.h"
#include "util.h"
using namespace arma;
void MDP_value(Tracker tracker,int frame_id,Dres_image dres_image,Dres_det dres_det,uvec index_det)
{
	Dres_det dres_one;
	int label;
	//tracked, decide to tracked or occluded
	if(tracker.state == 2)
	{
		//extract features with LK tracking
		uvec f = MDP_feature_tracked(frame_id, dres_image, dres_det, tracker);

		//build the dres structure
		if(bb_isdef(tracker.bb))
		{
			dres_one.fr = frame_id;
			dres_one.id = tracker.target_id;
			dres_one.x = tracker.bb(1);
			dres_one.y = tracker.bb(2);
			dres_one.w = tracker.bb(3) - tracker.bb(1);
			dres_one.h = tracker.bb(4) - tracker.bb(2);
			dres_one.r = 1;
		}
		else
		{
			uvec index = ones<uvec>(1);
			index(1) =  tracker.dres.fr.size();
			dres_one = sub(tracker.dres,index);
			dres_one.fr = frame_id;
			dres_one.id = tracker.target_id;
		}

		if(f(1) == 1 && f(2) > tracker.threshold_box)
		{
			label = 1;
		}
		else
		{
			label = -1;
		}

		//make a decision
		if(label>0)
		{
			tracker.state = 2;
			dres_one.state = 2;
			tracker.dres = concatenate_dres(tracker.dres, dres_one);
			//update LK tracker
			tracker = LK_update(frame_id, tracker, dres_image.Igray{frame_id}, dres_det, 0);
		}
		else
		{
			tracker.state = 3;
			dres_one.state = 3;
			tracker.dres = concatenate_dres(tracker.dres, dres_one);  
		}
		tracker.prev_state = 2;
	}
	else if(tracker.state == 3)
	{
		//association
		if(index_det.size() == 0)
		{
			label = -1;
			uvec f = zeros<uvec>(10);
		}
		else
		{
			// extract features with LK association

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