#include "convert.h"
Mat u2f(Mat src) {
	Mat dst;
	src.convertTo(dst, CV_32F, 1.0/ 65536.0);
	return dst;
}
Mat f2u(Mat src) {
	Mat dst;
	src.convertTo(dst, CV_8U, 65536.0);
	return dst;
}