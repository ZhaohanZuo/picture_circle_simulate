#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<opencv2/opencv.hpp>
#include<utility>
using namespace std;
using namespace cv;
struct circledata {
	double err, b, g, r;
	int x, y, rad;
	Mat diff;
	uchar alpha;
};
pair<double, Mat> mse(Mat, Mat, Mat, double, int, int, int);
