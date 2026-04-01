#include "convert.h"
Mat u2f(Mat src) {
	Mat dst;
	src.convertTo(dst, CV_64F, 1.0/ 255.0);
	return dst;
}
Mat f2u(Mat src) {
	Mat dst;
	src.convertTo(dst, CV_8U, 255.0);
	return dst;
}