#include "bbox.h"
#include<utility>
using namespace std;
Bbox bb_shift_absolute(Bbox bb,pair<int,int>shift)
{
	bb.x1+=shift.first;
	bb.y1+=shift.second;
	bb.x2+=shift.first;
	bb.y2+=shift.second;
}