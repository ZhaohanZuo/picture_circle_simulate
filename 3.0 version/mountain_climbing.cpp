#include "mountain_climbing.h"

circledata climb(const circledata& cir, const Mat& carvan, const Mat& img) {
	circledata newcir = cir;
	//static thread_local Mat tempcar_thread = carvan.clone();
	//if (tempcar_thread.empty() || tempcar_thread.size() != carvan.size()) {
	//	tempcar_thread = carvan.clone(); // 首次分配时申请内存
	//}
	//else {
	//	// 只拷贝数据，不重新申请内存
	//	carvan.copyTo(tempcar_thread);
	//}
	//static thread_local Mat mask_thread;//同理
	//if (mask_thread.empty() || mask_thread.size() != carvan.size()) {
	//	Mat mask_thread = Mat::zeros(carvan.size(), CV_8UC1);
	//}
	//else {
	//	mask_thread.setTo(Scalar(0));
	//}
	random_device rd;//随机数生成
	static thread_local mt19937 engine(rd());
	//static thread_local uniform_real_distribution<float> dist(-6, 6);
	//static thread_local uniform_int_distribution<int> dist1(0, 6);
	static thread_local std::uniform_real_distribution<double> pos_rad_dist(-0.3f, 0.3f);
	// 颜色:微调幅度非常小
	static thread_local std::uniform_real_distribution<double> color_dist(-0.005f, 0.005f);
	// 透明度:微调幅度适中
	static thread_local std::uniform_real_distribution<double> alpha_dist(-0.005f, 0.005f);

	newcir.x += pos_rad_dist(engine)*5;//微调圆
	newcir.y += pos_rad_dist(engine)*5;
	newcir.b += color_dist(engine)*5;
	newcir.g += color_dist(engine)*5;
	newcir.r += color_dist(engine)*5;
	newcir.rad += pos_rad_dist(engine)*5;
	newcir.alpha += alpha_dist(engine)*5;
	newcir.alpha = min(max(newcir.alpha, 0.01), 1.0)*5;
	newcir.b = min(max(newcir.b, 0.0), 1.0);
	newcir.g = min(max(newcir.g, 0.0), 1.0);
	newcir.r = min(max(newcir.r, 0.0), 1.0);
	newcir.x = min(max(newcir.x, 0.0), static_cast<double>(carvan.cols - 1));
	newcir.y = min(max(newcir.y, 0.0), static_cast<double>(carvan.rows - 1));
	newcir.rad = max(newcir.rad, 0.1);
	//circle(tempcar, Point(newcir.x, newcir.y), newcir.rad,
	//	Scalar(newcir.b, newcir.g, newcir.r),-1);//画出新圆
	//circle(mask_thread, Point(newcir.x, newcir.y), newcir.rad, Scalar(255), -1);//生成掩码
	//Rect rect = boundingRect(mask_thread);
	//for (int y = rect.y;y < rect.y + rect.height;y++) {//进行alpha叠加
	//	const uchar* maskrow = mask_thread.ptr<uchar>(y);
	//	const float* carvanrow = carvan.ptr<float>(y);
	//	float* tempcarrow = tempcar_thread.ptr<float>(y);
	//	for (int x = rect.x;x < rect.x + rect.width;x++) {
	//		if (maskrow[x]) {//判断该像素是否在圆上
	//			int idx = x * 3;
	//			tempcarrow[idx] = newcir.b * newcir.alpha + carvanrow[idx] * (1 - newcir.alpha);
	//			tempcarrow[idx + 1] = newcir.g * newcir.alpha + carvanrow[idx + 1] * (1 - newcir.alpha);
	//			tempcarrow[idx + 2] = newcir.r * newcir.alpha + carvanrow[idx + 2] * (1 - newcir.alpha);
	//		}
	//	}
	//}
	auto result = mse(img, carvan, cir.diff, newcir.globalerr, newcir.alpha,
		newcir.b, newcir.g, newcir.r, newcir.x, newcir.y, newcir.rad);//计算新圆mse
	newcir.err = result;
	return newcir;
}
//#include "mountain_climbing.h"
//#include "MSE.h"
//#include <random>
//#include <algorithm>
//
//circledata climb(const circledata& cir, const cv::Mat& carvan, const cv::Mat& img) {
//	circledata newcir = cir;
//
//	static thread_local std::mt19937 engine(std::random_device{}());
//
//	// 合理的扰动范围
//	static thread_local std::uniform_real_distribution<float> pos_rad_dist(-0.5f, 0.5f);
//	static thread_local std::uniform_real_distribution<float> color_dist(-0.01f, 0.01f);
//	static thread_local std::uniform_real_distribution<float> alpha_dist(-0.01f, 0.01f);
//
//	// 生成扰动
//	newcir.x += pos_rad_dist(engine);
//	newcir.y += pos_rad_dist(engine);
//	newcir.rad += pos_rad_dist(engine);
//	newcir.b += color_dist(engine);
//	newcir.g += color_dist(engine);
//	newcir.r += color_dist(engine);
//	newcir.alpha += alpha_dist(engine);
//
//	// 【修复】手动实现边界限制，不用 std::clamp
//	// 限制透明度：0.01 ~ 1.0
//	newcir.alpha = std::max(0.01f, std::min(newcir.alpha, 1.0f));
//	// 限制颜色：0.0 ~ 1.0
//	newcir.b = std::max(0.0f, std::min(newcir.b, 1.0f));
//	newcir.g = std::max(0.0f, std::min(newcir.g, 1.0f));
//	newcir.r = std::max(0.0f, std::min(newcir.r, 1.0f));
//
//	// 位置和半径越界检查
//	if (newcir.rad <= 0.5f ||
//		newcir.x < 0.0f || newcir.x >= static_cast<float>(carvan.cols) ||
//		newcir.y < 0.0f || newcir.y >= static_cast<float>(carvan.rows)) {
//		return cir; // 越界返回旧圆
//	}
//
//	// 计算新误差
//	newcir.err = mse(img, carvan, newcir.alpha, newcir.b, newcir.g, newcir.r, newcir.x, newcir.y, newcir.rad);
//
//	// 如果新误差无效，返回旧圆
//	if (newcir.err >= 1e9f) {
//		return cir;
//	}
//
//	return newcir;
//}