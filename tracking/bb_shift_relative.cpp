#include<armadillo>
#include<utility>
using namespace std;
using namespace arma;

mat bb_shift_relative(mat bb,pair<double,double>shift)
{
	bb.row(1) = bb.row(1)+(bb.row(3)-bb.row(1)+1)*shift.first;
	bb.row(2) = bb.row(2)+(bb.row(4)-bb.row(2)+1)*shift.second;
	bb.row(3) = bb.row(3)+(bb.row(3)-bb.row(1)+1)*shift.first;
	bb.row(4) = bb.row(4)+(bb.row(4)-bb.row(2)+1)*shift.second;
}