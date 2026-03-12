#include "MSE.h"
circledata climb(circledata cir, Mat carvan, Mat img) {
	circledata newcir = cir;
	Mat tempcar = carvan.clone();
	random_device rd;//随机数生成
	mt19937 engine(rd());
	uniform_int_distribution<int> dist(-6, 6);
	uniform_int_distribution<int> dist1(0, 6);
	float delta_b = static_cast<float>(dist(engine)) / 65535;
	float delta_g = static_cast<float>(dist(engine)) / 65535;
	float delta_r = static_cast<float>(dist(engine)) / 65535;
	int delta_x = dist(engine);
	int delta_y = dist(engine);
	int delta_rad = dist1(engine);
	float delta_alpha = static_cast<float>(dist(engine))/120;
	newcir.x = cir.x + delta_x;//微调圆
	newcir.y = cir.y + delta_y;
	newcir.b = cir.b + delta_b;
	newcir.g = cir.g + delta_g;
	newcir.r = cir.r + delta_r;
	newcir.rad = cir.rad + delta_rad;
	newcir.alpha = min(max(newcir.alpha + delta_alpha, 0.01f), 1.0f);
	//circle(tempcar, Point(newcir.x, newcir.y), newcir.rad,
	//	Scalar(newcir.b, newcir.g, newcir.r),-1);//画出新圆
	Mat mask = Mat::zeros(carvan.size(), CV_8UC1);
	circle(mask, Point(newcir.x, newcir.y), newcir.rad, Scalar(255), -1);//生成掩码
	Rect rect = boundingRect(mask);
	for (int y = rect.y;y < rect.y + rect.height;y++) {//进行alpha叠加
		const uchar* maskrow = mask.ptr<uchar>(y);
		const float* carvanrow = carvan.ptr<float>(y);
		float* tempcarrow = tempcar.ptr<float>(y);
		for (int x = rect.x;x < rect.x + rect.width;x++) {
			if (maskrow[x]) {//判断该像素是否在圆上
				int idx = x * 3;
				tempcarrow[idx] = newcir.b * newcir.alpha + carvanrow[idx] * (1 - newcir.alpha);
				tempcarrow[idx + 1] = newcir.g * newcir.alpha + carvanrow[idx + 1] * (1 - newcir.alpha);
				tempcarrow[idx + 2] = newcir.r * newcir.alpha + carvanrow[idx + 2] * (1 - newcir.alpha);
			}
		}
	}
	auto result = mse(img, tempcar, cir.diff, cir.err,
		newcir.x, newcir.y, newcir.rad);//计算新圆mse
	newcir.err = result.first;
	newcir.diff = result.second;
	return newcir;
}