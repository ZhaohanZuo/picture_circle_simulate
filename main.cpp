#include "calc_error.h"
#include "convert.h"
#include "refine_candidates.h"
#include <iostream>
using namespace std;
using namespace cv;

int main() {
	int cirnum, rows, cols;
	vector<circledata> cir;
	vector<double> circle_processing_times;//计时
	if (!(cin >> cirnum) || cirnum <= 0) {
		cerr << "Failed to read cirnum." << endl;
		return -1;
	}
	Mat imgu = imread("D:\\1.jpg");
	if (imgu.empty()) {
		cout << "Failed to load image." << endl;
		return 0;
	}
	rows = imgu.rows;
	cols = imgu.cols;
	Scalar car = mean(imgu);
	Mat carvanu(rows, cols, CV_8UC3, car);
	Mat carvan = u2f(carvanu);//f
	Mat img = u2f(imgu);
	if (carvan.empty()) {
		cerr << "u2f returned an empty Mat." << endl;
		return -1;
	}

	//	auto refine_candidates = [&](size_t keepCount, int climbCount) {
	//		if (cir.empty()) {
	//			return;
	//		}
	//
	//		sort(cir.begin(), cir.end(), [](const circledata& a, const circledata& b) {
	//			return a.err < b.err;
	//			});
	//		if (cir.size() > keepCount) {
	//			cir.resize(keepCount);
	//		}
	//#pragma omp parallel for shared(carvan, img ,cir)//开始并行并将两个Mat对象赋给每个线程
	//		for (int index = 0; index < cir.size(); ++index) {
	//			circledata local_best = cir[index];//定义线程内存储变量
	//			circledata tempcir_local;
	//			for (int step = 0; step < climbCount; ++step) {
	//				tempcir_local = climb(local_best, carvan, img);
	//				if (tempcir_local.err < local_best.err) {
	//					local_best = tempcir_local;
	//				}
	//			}
	//			cir[index] = local_best;
	//		}
	//
	//		};
	//
	Mat final;
	for (int i = 0; i < cirnum; i++) {
		cout << "=== Processing Circle " << (i + 1) << " / " << cirnum << " ===" << endl;
		cir.clear();
		cout << "  Generating initial candidates..." << flush;
		double start_time = static_cast<double>(cv::getTickCount());//计时起点
#pragma omp parallel 
		{//开始并行运算
			vector<circledata> cir_private;//创建线程私有变量
			circledata tempcir_private;
#pragma omp for nowait//并行执行for循环
			for (int j = 0; j < 180; j++) {
				circledata cd = calc_error(rows, cols, carvan, img);
				if (cd.rad <= 0 || cd.x < 0 || cd.x >= cols || cd.y < 0 || cd.y >= rows) {
					continue;
				}
				cir_private.push_back(cd);
				for (int k = 0; k < 5; k++) {
					tempcir_private = (climb(cir_private.back(), carvan, img));
					if (tempcir_private.err < cir_private.back().err) {
						cir_private.back() = tempcir_private;
					}
				}
			}
#pragma omp critical
			{//将并行结果合并写入
				cir.insert(cir.end(), cir_private.begin(), cir_private.end());
			}
		}
		if (cir.empty()) {
			continue;
		}
		cout << "  Refining candidates..." << flush;
		refine_candidates(80, 8, carvan, img, cir);
		cout << "." << flush;
		refine_candidates(50, 25, carvan, img, cir);
		cout << "." << flush;
		refine_candidates(25, 80, carvan, img, cir);
		cout << "." << flush;
		refine_candidates(10, 128, carvan, img, cir);
		cout << " Done." << endl;
		sort(cir.begin(), cir.end(), [](const circledata& a, const circledata& b) {
			return a.err < b.err;
			});

		const circledata& best = cir.front();
		if (best.rad > 0) {
			//Mat mask = Mat::zeros(carvan.size(), CV_8UC1);
			//circle(mask, Point(best.x, best.y), best.rad, Scalar(255), -1);//生成掩码
			//Rect rect = boundingRect(mask);
			//for (int y = rect.y;y < rect.y + rect.height;y++) {//进行alpha叠加
			//	if (y < 0 || y >= carvan.rows) continue;
			//	const uchar* maskrow = mask.ptr<uchar>(y);
			//	float* carvanrow = carvan.ptr<float>(y);
			//	for (int x = rect.x;x < rect.x + rect.width;x++) {
			//		if (x < 0 || x >= carvan.cols) continue;
			//		if (maskrow[x]) {//判断该像素是否在圆上
			//			int idx = x * 3;
			//			carvanrow[idx] = best.b * best.alpha + carvanrow[idx] * (1 - best.alpha);
			//			carvanrow[idx + 1] = best.g * best.alpha + carvanrow[idx + 1] * (1 - best.alpha);
			//			carvanrow[idx + 2] = best.r * best.alpha + carvanrow[idx + 2] * (1 - best.alpha);
			//		}
			//	}
			//}
			auto imgsize = img.size();
			int x_min = max(0.0, best.x - best.rad);
			int x_max = min(static_cast<double>(imgsize.width - 1), best.x + best.rad);
			int y_min = max(0.0, best.y - best.rad);
			int y_max = min(static_cast<double>(imgsize.height - 1), best.y + best.rad);
			double radf = static_cast<double>(best.rad);
			double rad_sq = radf * radf;
			double minus_alpha = 1.0f - best.alpha;
			for (int y = y_min;y <= y_max;y++) {//获取图像地址
				//const uchar* maskrow = mask.ptr<uchar>(y);
				//const float* imgrow = img.ptr<float>(y);
				double* carvan_row = carvan.ptr<double>(y);
				//float* newdiffrow = newdiff.ptr<float>(y);
				for (int x = x_min;x <= x_max;x++) {
					double dx = static_cast<double>(x - best.x);
					double dy = static_cast<double>(y - best.y);
					if (dx * dx + dy * dy < rad_sq) {//判断该像素是否被更新
						int idx = x * 3;
						carvan_row[idx] = best.b * best.alpha + carvan_row[idx] * minus_alpha;
						carvan_row[idx + 1] = best.g * best.alpha + carvan_row[idx + 1] * minus_alpha;
						carvan_row[idx + 2] = best.r * best.alpha + carvan_row[idx + 2] * minus_alpha;
						//olderr += newdiffrow[idx] + newdiffrow[idx + 1] + newdiffrow[idx + 2];//计算原误差
						//float err0 = imgrow[idx] - expect_b;
						//float err1 = imgrow[idx + 1] - expect_g;
						//float err2 = imgrow[idx + 2] - expect_r;
						//newdiffrow[idx] = err0 * err0;//更新diff
						//newdiffrow[idx + 1] = err1 * err1;
						//newdiffrow[idx + 2] = err2 * err2;
						//err += err0 * err0 + err1 * err1 + err2 * err2;//计算新误差
					}
				}
			}
		}
		//Mat diff;
		//absdiff(carvan, img, diff);//计算diff
		//diff = diff.mul(diff);
		//Scalar s = sum(diff);
		//double final_err = (s.val[0] + s.val[1] + s.val[2]) /
		//	(carvan.total() * carvan.channels());//计算mse
		cout << "  Best found: x=" << best.x << ", y=" << best.y
			<< ", rad=" << best.rad << ", err=" << best.err << ", b="
			<< best.b * 65535 << ", g=" << best.g * 65535 << ", r=" << best.r * 65535 << ", alpha="
			<< best.alpha << endl;
		cir.clear();
		double end_time = static_cast<double>(cv::getTickCount());//计时结束
		double elapsed_time = (end_time - start_time) / cv::getTickFrequency();//计算用时
		cout << "  Time taken for Circle " << (i + 1) << ": "
			<< elapsed_time * 1000.0 << " ms" << endl;
		circle_processing_times.push_back(elapsed_time);
	}
	cout << "All circles processed. Writing image..." << endl;
	final = f2u(carvan);
	if (!final.empty()) {
		cv::imwrite("D:\\output.jpg", final);
	}
	cout << "Image writing complete!" << endl;
	cout << "Path:D:\\output.jpg" << endl;
	if (!circle_processing_times.empty()) {
		double total_time = 0.0;
		double max_time = circle_processing_times[0];
		double min_time = circle_processing_times[0];

		for (double t : circle_processing_times) {
			total_time += t;
			if (t > max_time) max_time = t;
			if (t < min_time) min_time = t;
		}

		cout << "========== Timing Summary ==========" << endl;
		cout << "Total circles processed: " << cirnum << endl;
		cout << "Total elapsed time:      " << total_time << " s" << endl;
		cout << "Average time per circle: " << (total_time / cirnum) * 1000.0 << " ms" << endl;
		cout << "Slowest circle:          " << max_time * 1000.0 << " ms" << endl;
		cout << "Fastest circle:          " << min_time * 1000.0 << " ms" << endl;
		cout << "=====================================" << endl;
	}
	return 0;
}