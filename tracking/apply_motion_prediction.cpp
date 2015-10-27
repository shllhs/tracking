#include"tracker.h"
#include"dres_det.h"
#include<armadillo>
using namespace arma;
vec apply_motion_prediction(int fr_current,Tracker tracker)
{
	//apply motion model and predict next location
	Dres_det dres = tracker.dres;
	uvec index = find(dres.state == 2);
	dres = sub(dres, index);
	vec cx = dres.x + dres.w/2;
	vec cy = dres.y + dres.h/2;
	uvec fr = dres.fr;

	// only use the past 10 frames
	int num = fr.size();
	int K = 10;
	if(num > K)
	{
		cx = cx(linspace<uvec>(num-K,num-1));
		cy = cy(linspace<uvec>(num-K,num-1));
		fr = fr(linspace<uvec>(num-K,num-1));
	}

	// compute velocity
	double vx = 0;
	double vy = 0;
	int num = cx.size();
	int count = 0;
	for(int j = 1;j<num;j++)
	{
		 vx = vx + (cx(j)-cx(j-1)) / (fr(j) - fr(j-1));
		 vy = vy + (cy(j)-cy(j-1)) / (fr(j) - fr(j-1));
		 count = count + 1;
	}
	if(count!=0)
	{
		vx = vx / count;
		vy = vy / count;
	}

	double cx_new,cy_new;
	if(cx.is_empty() == 1)
	{
		dres = tracker.dres;
		cx_new = *(dres.x.end()-1) + *(dres.w.end()-1)/2;
		cy_new = *(dres.y.end()-1) + *(dres.h.end()-1)/2;
	}
	else
	{
		cx_new = *(cx.end()-1) + vx * (fr_current + 1 - *(fr.end()-1));
		cy_new = *(cy.end()-1) + vy * (fr_current + 1 - *(fr.end()-1));
	}
	vec prediction = zeros<vec>(2);
	prediction(0) = cx_new;
	prediction(1) = cy_new;
	return prediction;
}