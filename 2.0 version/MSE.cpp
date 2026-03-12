#include "MSE.h"
pair<float, Mat> mse(Mat img, Mat tempcar, Mat diff, float err,
	int x, int y, int rad){
	float newerr = 0, olderr = 0;
	auto imgsize = img.size();
	unsigned int totalpix = static_cast<unsigned int>(imgsize.width)*
		static_cast<unsigned int>(imgsize.height);
	Mat newdiff = diff.clone();
	Mat mask = Mat::zeros(img.size(), CV_8UC1);
	circle(mask, Point(x, y), rad, Scalar(255), -1);//生成掩码
	Rect rect = boundingRect(mask);//构造最小矩形
	for (int y = rect.y;y < rect.y + rect.height;y++) {//获取图像地址
		const uchar* maskrow = mask.ptr<uchar>(y);
		const float* imgrow = img.ptr<float>(y);
		const float* tempcarrow = tempcar.ptr<float>(y);
		float* newdiffrow = newdiff.ptr<float>(y);
		for (int x = rect.x;x < rect.x + rect.width;x++) {
			if (maskrow[x]) {//判断该像素是否被更新
				int idx = x * 3;
				olderr += newdiffrow[idx] + newdiffrow[idx + 1] + newdiffrow[idx + 2];//计算原误差
				float err0 = imgrow[idx] - tempcarrow[idx];
				float err1 = imgrow[idx + 1] - tempcarrow[idx + 1];
				float err2 = imgrow[idx + 2] - tempcarrow[idx + 2];
				newdiffrow[idx] = err0 * err0;//更新diff
				newdiffrow[idx + 1] = err1 * err1;
				newdiffrow[idx + 2] = err2 * err2;
				newerr += err0 * err0 + err1 * err1 + err2 * err2;//计算新误差
			}
		}
	}
	err += (newerr - olderr) / (totalpix * 3);
	return { err, newdiff };
}