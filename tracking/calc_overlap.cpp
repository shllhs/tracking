#include "dres_det.h"
#include<armadillo>
#include<algorithm>
using namespace std;
using namespace arma;

void calc_overlap(vec &ov,Dres_det dres1,int f1,Dres_det dres2,uvec f2)
{
	int n = f2.size();

	double cx1 = dres1.x(f1);
	double cx2 = dres1.x(f1)+dres1.w(f1)-1;
	double cy1 = dres1.y(f1);
	double cy2 = dres1.y(f1)+dres1.h(f1)-1;

	vec gx1 = dres2.x(f2);
	vec gx2 = dres2.x(f2)+dres2.w(f2)-1;
	vec gy1 = dres2.y(f2);
	vec gy2 = dres2.y(f2)+dres2.h(f2)-1;

	double ca = dres1.h(f1)*dres1.w(f1); 
	vec ga = dres2.h(f2)*dres2.w(f2);

	//find the overlapping area
	vec xx1 = zeros<vec>(n);
	vec yy1 = zeros<vec>(n);
	vec xx2 = zeros<vec>(n);
	vec yy2 = zeros<vec>(n);

	for(int i = 1;i<=n;i++)
	{
		xx1(i) = max(cx1,gx1(i));
		yy1(i) = max(cy1,gy1(i));
		xx2(i) = min(cx2,gx2(i));
		yy2(i) = min(cy2,gy2(i));
	}
	vec w = xx2-xx1+1;
	vec h = yy2-yy1+1;

	uvec inds = find((w>0)*(h>0));
	ov = zeros<vec>(n);
	if(inds.is_empty() == 0)
	{
		vec inter = w(inds)*h(inds);
		vec u = ca+ga(inds)-w(inds)*h(inds);
		ov(inds) = inter/u;
	}
}

void calc_overlap(vec &ov,vec &ov_n1,Dres_det dres1,int f1,Dres_det dres2,uvec f2)
{
	int n = f2.size();

	double cx1 = dres1.x(f1);
	double cx2 = dres1.x(f1)+dres1.w(f1)-1;
	double cy1 = dres1.y(f1);
	double cy2 = dres1.y(f1)+dres1.h(f1)-1;

	vec gx1 = dres2.x(f2);
	vec gx2 = dres2.x(f2)+dres2.w(f2)-1;
	vec gy1 = dres2.y(f2);
	vec gy2 = dres2.y(f2)+dres2.h(f2)-1;

	double ca = dres1.h(f1)*dres1.w(f1); 
	vec ga = dres2.h(f2)*dres2.w(f2);

	//find the overlapping area
	vec xx1 = zeros<vec>(n);
	vec yy1 = zeros<vec>(n);
	vec xx2 = zeros<vec>(n);
	vec yy2 = zeros<vec>(n);

	for(int i = 1;i<=n;i++)
	{
		xx1(i) = max(cx1,gx1(i));
		yy1(i) = max(cy1,gy1(i));
		xx2(i) = min(cx2,gx2(i));
		yy2(i) = min(cy2,gy2(i));
	}
	vec w = xx2-xx1+1;
	vec h = yy2-yy1+1;

	uvec inds = find((w>0)*(h>0));
	ov = zeros<vec>(n);
	ov_n1 = zeros<vec>(n);
	if(inds.is_empty() == 0)
	{
		vec inter = w(inds)*h(inds);
		vec u = ca+ga(inds)-w(inds)*h(inds);
		ov(inds) = inter/u;
		ov_n1(inds) = inter/ca;
	}
}