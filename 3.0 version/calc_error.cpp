#include "calc_error.h"
circledata calc_error(int rows,int cols,const Mat& carvan, const Mat& img){
	circledata outp = {};
	Scalar s;
	//float b, g, r, alpha;
	//Scalar color;
	//Mat tempcar=carvan.clone();
	//static thread_local RNG rng(getTickCount());
	//outp.rad = rng.uniform(1, rows);
	//outp.x = rng.uniform(0, cols);
	//outp.y = rng.uniform(0, rows);
	//b = rng.uniform(0, 256);
	//g = rng.uniform(0, 256);
	//r = rng.uniform(0, 256);
	//alpha = rng.uniform(0, 256);
	//outp.alpha = alpha / 255;//归一化
	//outp.b = b / 65535;
	//outp.g = g / 65535;
	//outp.r = r / 65535;
	//static thread_local Mat tempcar_thread = carvan.clone();
	//if (tempcar_thread.empty() || tempcar_thread.size() != carvan.size()) {
	//	tempcar_thread = carvan.clone(); // 首次分配时申请内存
	//}
	//else {
	//	// 只拷贝数据，不重新申请内存
	//	carvan.copyTo(tempcar_thread);
	//}
	static thread_local std::mt19937 rng(std::random_device{}());
	static thread_local std::uniform_real_distribution<double> rad_dist(1.0, static_cast<double>(std::min(rows, cols)));
	static thread_local std::uniform_real_distribution<double> pos_x_dist(0.0, static_cast<double>(cols));
	static thread_local std::uniform_real_distribution<double> pos_y_dist(0.0, static_cast<double>(rows));
	static thread_local std::uniform_real_distribution<double> color_dist(0.0, 1.0); // 颜色直接在 0-1 之间生成
	static thread_local std::uniform_real_distribution<double> alpha_dist(0.01, 1.0); // 透明度初始值不要太低

	outp.rad = rad_dist(rng);
	outp.x = pos_x_dist(rng);
	outp.y = pos_y_dist(rng);
	outp.b = color_dist(rng); 
	outp.g = color_dist(rng);
	outp.r = color_dist(rng);
	outp.alpha = alpha_dist(rng);
	absdiff(carvan, img, outp.diff);//计算diff
	outp.diff = outp.diff.mul(outp.diff);
	s = sum(outp.diff);
	outp.err = (s.val[0] + s.val[1] + s.val[2]) /
		(carvan.total() * carvan.channels());//计算mse
	auto result = mse(img, carvan, outp.diff, outp.err, outp.alpha, outp.b, outp.g, outp.r, outp.x, outp.y, outp.rad);
	outp.err = result;
	outp.globalerr = outp.err;
	if (outp.err < 0) {
		// 如果触发了这个警告，说明你的 mse 分母肯定还是算错了！
		cerr << "Warning: Negative MSE clamped!" << std::endl;
		//outp.err = 0;
	}

	//circle(tempcar, Point(outp.x, outp.y), outp.rad, 
		//Scalar(outp.b, outp.g, outp.r), -1);//随机画圆
	//Mat mask = Mat::zeros(carvan.size(), CV_8UC1);
	//circle(mask, Point(outp.x, outp.y), outp.rad, Scalar(255), -1);//生成掩码
	//Rect rect = boundingRect(mask);
	//for (int y = rect.y;y < rect.y + rect.height;y++) {//进行alpha叠加
	//	const uchar* maskrow = mask.ptr<uchar>(y);
	//	const float* carvanrow = carvan.ptr<float>(y);
	//	float* tempcarrow = tempcar.ptr<float>(y);
	//	for (int x = rect.x;x < rect.x + rect.width;x++) {
	//		if (maskrow[x]) {//判断该像素是否在圆上
	//			int idx = x * 3;
	//			tempcarrow[idx] = outp.b * outp.alpha + carvanrow[idx] * (1 - outp.alpha);
	//			tempcarrow[idx + 1] = outp.g * outp.alpha + carvanrow[idx + 1] * (1 - outp.alpha);
	//			tempcarrow[idx + 2] = outp.r * outp.alpha + carvanrow[idx + 2] * (1 - outp.alpha);
	//		}
	//	}
	//}
	return outp;
}