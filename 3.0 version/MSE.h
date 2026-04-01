#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<opencv2/opencv.hpp>
#include<utility>
using namespace std;
using namespace cv;
//struct circledata {
//	double err, b, g, r, alpha;
//	int x, y, rad;
//	Mat diff;
//};
struct circledata {
	double err, b, g, r, alpha, x, y, rad, globalerr;
	Mat diff;
	};
//pair<double, Mat> mse(const Mat&, const Mat&, const Mat&, double, int, int, int);
double mse(const Mat&, const Mat&, const Mat&, double, double, double, double, double, double, double, double);