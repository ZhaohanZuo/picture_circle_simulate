#include "calc_error.h"
circledata calc_error(int rows,int cols,Mat carvan, Mat img){
	circledata outp;
	Scalar s;
	float b, g, r;
	uchar alpha;
	Scalar color;
	Mat tempcar=carvan.clone();
	RNG rng(getTickCount());
	outp.rad = rng.uniform(1, rows);
	outp.x = rng.uniform(0, cols);
	outp.y = rng.uniform(0, rows);
	b = rng.uniform(0, 256);
	g = rng.uniform(0, 256);
	r = rng.uniform(0, 256);
	alpha = rng.uniform(0, 256);
	outp.alpha = alpha / 255;//归一化
	outp.b = b / 65535;
	outp.g = g / 65535;
	outp.r = r / 65535;
	//circle(tempcar, Point(outp.x, outp.y), outp.rad, 
		//Scalar(outp.b, outp.g, outp.r), -1);//随机画圆
	Mat mask = Mat::zeros(carvan.size(), CV_8UC1);
	circle(mask, Point(outp.x, outp.y), outp.rad, Scalar(255), -1);//生成掩码
	Rect rect = boundingRect(mask);
	for (int y = rect.y;y < rect.y + rect.height;y++) {//进行alpha叠加
		const uchar* maskrow = mask.ptr<uchar>(y);
		const float* carvanrow = carvan.ptr<float>(y);
		float* tempcarrow = tempcar.ptr<float>(y);
		for (int x = rect.x;x < rect.x + rect.width;x++) {
			if (maskrow[x]) {//判断该像素是否在圆上
				int idx = x * 3;
				tempcarrow[idx] = outp.b * outp.alpha + carvanrow[idx] * (1 - outp.alpha);
				tempcarrow[idx + 1] = outp.g * outp.alpha + carvanrow[idx + 1] * (1 - outp.alpha);
				tempcarrow[idx + 2] = outp.r * outp.alpha + carvanrow[idx + 2] * (1 - outp.alpha);
			}
		}
	}
	absdiff(tempcar, img, outp.diff);//计算diff
	outp.diff = outp.diff.mul(outp.diff);
	s = sum(outp.diff);
	outp.err = (s.val[0] + s.val[1] + s.val[2]) /
		(carvan.total() * carvan.channels());//计算mse
	return outp;
}