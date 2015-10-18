#include<opencv2/core/core.hpp>
#include<cmath>
#include<algorithm>
#include "bbox.h"
using namespace cv;
cv::Mat im_crop(Mat img,Bbox bb)
{
	int w = bb.x2-bb.x1+1;
	int h = bb.y2-bb.y1+1;
	cv::Mat I = Mat::zeros(w,h,CV_8UC1);
	int x1 = 1>bb.x1?1:bb.x1;
	int y1 = 1>bb.y1?1:bb.y1;
	int x2 = img.rows<bb.x2?img.rows:bb.x2;
	int y2 = img.cols<bb.y2?img.cols:bb.y2;

	Rect rect(x1,y1,x2-x1+1,y2-y1+1);
	Mat patch = img(rect);

	//Rect rect1(x1-bb.x1,y1-bb.y1,x2-x1+1,y2-y1+1);
	//I(rect) = patch;
	return patch;
}