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
	float err, b, g, r, alpha;
	int x, y, rad;
	Mat diff;
	//uchar ;
};
pair<float, Mat> mse(Mat, Mat, Mat, float, int, int, int);