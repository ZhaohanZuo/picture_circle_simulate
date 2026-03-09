#include "MSE.h"
circledata climb(circledata cir, Mat carvan) {
	circledata newcir;
	Mat tempcar = carvan.clone();
	random_device rd;//随机数生成
	mt19937 engine(rd());
	uniform_int_distribution<int> dist(-6, 6);
	uniform_int_distribution<int> dist1(0, 6);
	double delta_b = static_cast<double>(dist(engine)) / 65535;
	double delta_g = static_cast<double>(dist(engine)) / 65535;
	double delta_r = static_cast<double>(dist(engine)) / 65535;
	int delta_x = dist(engine);
	int delta_y = dist(engine);
	int delta_rad = dist1(engine);
	newcir.x = cir.x + delta_x;//微调圆
	newcir.y = cir.y + delta_y;
	newcir.b = cir.b + delta_b;
	newcir.g = cir.g + delta_g;
	newcir.r = cir.r + delta_r;
	newcir.rad = cir.rad + delta_rad;
	circle(tempcar, Point(newcir.x, newcir.y), newcir.rad,
		Scalar(newcir.b, newcir.g, newcir.r),-1);//画出新圆
	auto result = mse(carvan, tempcar, cir.diff, cir.err,
		newcir.x, newcir.y, newcir.rad);//计算新圆mse
	newcir.err = result.first;
	newcir.diff = result.second;
	return newcir;
}