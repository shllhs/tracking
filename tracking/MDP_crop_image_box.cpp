#include "tracker.h"
#include "dres_det.h"
#include "bbox.h"
#include<opencv2/core/core.hpp>
#include<armadillo>

Dres_det MDP_crop_image_box(Dres_det dres_det,cv::Mat I,Tracker tracker)
{
	Dres_det dres;
	int num = dres.fr.size();

	for(int i = 1;i<=num;i++)
	{
		Bbox BB;
		BB.x1 = dres.x(i);
		BB.y1 = dres.y(i);
		BB.x2 = dres.x(i)+dres.w(i);
		BB.y2 = dres.x(i)+dres.h(i);


	}
	return dres;
}