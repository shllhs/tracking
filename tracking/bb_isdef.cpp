#include<armadillo>
using namespace arma;
const int inf = 1000000007;
int bb_isdef(vec bb)
{
	for(int i = 1;i<bb.size();i++)
	{
		if(bb(i)>inf||bb(i)<-inf)return 0;
	}
	return 1;
}