#include "dres_image.h"
#include<algorithm>
using namespace std;

double calc_overlap(Dres_image dres1,int f1,Dres_image dres2,int f2)
{
	double cx1 = dres1.x(f1);
	double cx2 = dres1.x(f1)+dres1.w(f1)-1;
	double cy1 = dres1.y(f1);
	double cy2 = dres1.y(f1)+dres1.h(f1)-1;

	double gx1 = dres2.x(f2);
	double gx2 = dres2.x(f2)+dres2.w(f2)-1;
	double gy1 = dres2.y(f2);
	double gy2 = dres2.y(f2)+dres2.h(f2)-1;

	double ca = dres1.h(f1)*dres1.w(f1); 
	double ga = dres2.h(f2)*dres2.w(f2);

	//find the overlapping area
	double xx1 = max(cx1, gx1);
	double yy1 = max(cy1, gy1);
	double xx2 = min(cx2, gx2);
	double yy2 = min(cy2, gy2);
	double w = xx2-xx1+1;
	double h = yy2-yy1+1;

	if(w>0&&h>0)
	{
		double inter = w*h;
		double u = ca+ga-w*h;
		double ov = inter/u;
		double ov_n1 = inter/ca;
		double ov_n2 = inter/ga;
	}
	return ga;
}