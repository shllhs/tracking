#include"dres_det.h"
Dres_det concatenate_dres(Dres_det dres1,Dres_det dres2)
{
	Dres_det dres_new;
	if(dres2.fr.is_empty())
		dres_new = dres1;
	else 
	{
		dres_new.fr = join_cols(dres1.fr,dres2.fr);
		dres_new.h = join_cols(dres1.h,dres2.h);
		dres_new.x = join_cols(dres1.x,dres2.x);
		dres_new.y = join_cols(dres1.y,dres2.y);
		dres_new.w = join_cols(dres1.w,dres2.w);
		dres_new.h = join_cols(dres1.h,dres2.h);
		dres_new.id = join_cols(dres1.id,dres2.id);
		dres_new.r = join_cols(dres1.r,dres2.r);
		dres_new.state = join_cols(dres1.state,dres2.state);
	}
	return dres_new;
}