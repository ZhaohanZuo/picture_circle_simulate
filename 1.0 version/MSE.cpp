#include "MSE.h"
pair<double, Mat> mse(Mat carvan, Mat tempcar, Mat diff, double err,
	int x, int y, int rad){
	double newerr = 0, olderr = 0;
	unsigned int totalpix = 0;
	Mat newdiff = diff.clone();
	Mat mask = Mat::zeros(carvan.size(), CV_8UC1);
	circle(mask, Point(x, y), rad, Scalar(255), -1);//生成掩码
	Rect rect = boundingRect(mask);//构造最小矩形
	for (int y = rect.y;y < rect.y + rect.height;y++) {//获取图像地址
		const uchar* maskrow = mask.ptr<uchar>(y);
		const float* carvanrow = carvan.ptr<float>(y);
		const float* tempcarrow = tempcar.ptr<float>(y);
		float* newdiffrow = newdiff.ptr<float>(y);
		for (int x = rect.x;x < rect.x + rect.width;x++) {
			if (maskrow[x]) {//判断该像素是否被更新
				int idx = x * 3;
				olderr += newdiffrow[idx] + newdiffrow[idx + 1] + newdiffrow[idx + 2];//计算原误差
				float err0 = carvanrow[idx] - tempcarrow[idx];
				float err1 = carvanrow[idx + 1] - tempcarrow[idx + 1];
				float err2 = carvanrow[idx + 2] - tempcarrow[idx + 2];
				newdiffrow[idx] = err0 * err0;//更新diff
				newdiffrow[idx + 1] = err1 * err1;
				newdiffrow[idx + 2] = err2 * err2;
				newerr += err0 * err0 + err1 * err1 + err2 * err2;//计算新误差
				totalpix++;//计算更新像素数
			}
		}
	}
	err += (newerr - olderr) / totalpix;
	return { err, newdiff };
}
