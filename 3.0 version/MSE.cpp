#include "MSE.h"
double mse(const Mat& img, const Mat& carvan,const Mat& diff, double globalerr, double alpha,
	double b, double g, double r, double x_center, double y_center, double rad){
	double olderr = 0;
	double newerr = 0;
	auto imgsize = img.size();
	unsigned int totalpix = static_cast<unsigned int>(imgsize.width)*
		static_cast<unsigned int>(imgsize.height);
	const int x_min = max(0.0, x_center - rad);
	const int x_max = min(static_cast<double>(imgsize.width - 1), x_center + rad);
	const int y_min = max(0.0, y_center - rad);
	const int y_max = min(static_cast<double>(imgsize.height - 1), y_center + rad);
	const double radf = static_cast<double>(rad);
	const double rad_sq = radf * radf;
	const double minus_alpha = 1.0 - alpha;
	//Mat newdiff = diff.clone();
	//Mat mask = Mat::zeros(img.size(), CV_8UC1);
	//circle(mask, Point(x, y), rad, Scalar(255), -1);//生成掩码
	//Rect rect = boundingRect(mask);//构造最小矩形
	int pix = 0;
	for (int y = y_min;y <= y_max;y++) {//获取图像地址
		//const uchar* maskrow = mask.ptr<uchar>(y);
		const double* imgrow = img.ptr<double>(y);
		const double* tempcarrow = carvan.ptr<double>(y);
		const double* newdiffrow = diff.ptr<double>(y);
		for (int x = x_min;x <= x_max;x++) {
			const double dx = static_cast<double>(x - x_center);
			const double dy = static_cast<double>(y - y_center);
			
			if (dx * dx + dy * dy < rad_sq) {//判断该像素是否被更新
				int idx = x * 3;
				const double expect_b = b * alpha + tempcarrow[idx] * minus_alpha;
				const double expect_g = g * alpha + tempcarrow[idx + 1] * minus_alpha;
				const double expect_r = r * alpha + tempcarrow[idx + 2] * minus_alpha;
				olderr += newdiffrow[idx] + newdiffrow[idx + 1] + newdiffrow[idx + 2];//计算原误差
				double err0 = imgrow[idx] - expect_b;
				double err1 = imgrow[idx + 1] - expect_g;
				double err2 = imgrow[idx + 2] - expect_r;
				//newdiffrow[idx] = err0 * err0;//更新diff
//				newdiffrow[idx + 1] = err1 * err1;
//				newdiffrow[idx + 2] = err2 * err2;
				newerr += err0 * err0 + err1 * err1 + err2 * err2;//计算新误差
				
			}
		}
	}
	return (globalerr + (newerr - olderr) / (imgsize.area()*img.channels()));
}
