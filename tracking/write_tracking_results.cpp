#include"dres_det.h"
#include<cstring>
#include<string>
using namespace std;

int write_tracking_results(string filename,Dres_det dres,int threshold)
{
	//compute target lengths
	int num = max(dres.id);
	uvec len = zeros<uvec>(num);
	for(int i = 0;i<num;i++)
	{
		uvec ind = find(dres.id == i && dres.state == 2);
		len(i) = ind.size();
	}

	FILE *fp = fopen(filename.c_str(),"w");
	
	int N = dres.x.size();
	for(int i = 0;i<N;i++)
	{
		 //<frame>, <id>, <bb_left>, <bb_top>, <bb_width>, <bb_height>, <conf>, <x>, <y>, <z>
		if(len(dres.id(i)) > threshold && dres.state(i) == 2)
		{
			fprintf(fp, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f\n",dres.fr(i), dres.id(i), dres.x(i), dres.y(i), dres.w(i), dres.h(i), -1, -1, -1, -1);
		}
	}
	return 1;
}