#include"dres_det.h"
#include"dres_det.h"
#include"tracker.h"
#include<armadillo>
#include<algorithm>
using namespace arma;
int generate_association_index(Dres_det &dres_det,uvec &index_det,vec &ctrack,Tracker tracker,int frame_id,Dres_det dres_det)
{
	ctrack = apply_motion_prediction(frame_id, tracker);

	int num_det = dres_det.fr.size();
	mat cdets = join_rows(dres_det.x + dres_det.w/2, dres_det.y + dres_det.h/2);

	//compute distances and aspect ratios
	vec distances = zeros<vec>(num_det);
	vec ratios = zeros<vec>(num_det);
	for(int i = 0;i<num_det;i++)
	{
		
		distances(i) = norm(cdets.row(i) - ctrack,2) / (*(tracker.dres.w.end()-1));

		double ratio = (*tracker.dres.h.end()-1) / dres_det.h(i);
		ratios(i) = std::min(ratio, 1/ratio);
	}

	index_det = find(distances < tracker.threshold_dis && ratios > tracker.threshold_ratio);
	dres_det.ratios = ratios;
	dres_det.distances = distances;
}