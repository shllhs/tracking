#ifndef DRES_IMAGE_H
#define DRES_IMAGE_H

#include<opencv2/core/core.hpp>
#include<armadillo>
struct Dres_image
{
	arma::vec x;
	arma::vec y;
	arma::vec w;
	arma::vec h;
	arma::Col<cv::Mat> I;
	arma::Col<cv::Mat> Igray;
}
#endif