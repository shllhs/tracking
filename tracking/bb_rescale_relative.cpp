#include "bbox.h"
#include<utility>
using namespace std;
Bbox bb_rescale_relative(Bbox BB,pair<double,double>s)
{
	double s1 = 0.5*(s.first-1)*(BB.x2-BB.x1+1);
	double s2 = 0.5*(s.second-1)*(BB.y2-BB.y1+1);

	Bbox bb;
	bb.x1 = BB.x1-s1;
	bb.y1 = BB.y1-s2;
	bb.x2 = BB.x2+s1;
	bb.y2 = BB.y2+s2;
	return bb;
}