#include"dres_det.h"
#include"tracker.h"
Dres_det generate_results(Col<Tracker> trackers)
{
	Dres_det dres_all;
	for(int i = 0;i<trackers.size();i++)
	{
		Tracker tracker = trackers(i);
		if(dres_all.fr.is_empty())
			dres_all = tracker.dres;
		else 
			dres_all = concatenate_dres(dres_all, tracker.dres);
	}
	return dres_all;
}