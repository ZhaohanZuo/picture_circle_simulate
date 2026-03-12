/*#include "calc_error.h"
#include "mountain_climbing.h"
#include "convert.h"
using namespace std;
using namespace cv;
int main() {
	//RNG rng(getTickCount());
	int cirnum, rows, cols;
	circledata tempcir;
	vector<circledata> cir;
	cin >> cirnum;//设定画圆个数
	Mat imgu = imread("D:\\1.jpeg");
	if (imgu.empty()) {
		cout << "图片加载失败" << endl;
		return 0;
	}
	rows = imgu.rows;
	cols = imgu.cols;
	Scalar car = mean(imgu);
	Mat carvanu(rows, cols, CV_8UC3, car);
	Mat carvan = u2f(carvanu);
	Mat final;
	for (int i = 0;i < cirnum;i++) {
		for (int j = 0;j < 200;j++) {
			cir.push_back(calc_error(rows, cols, carvan));
			for (int k = 0;k < 8;k++) {
				tempcir = (climb(cir.back(), carvan));
				if (tempcir.err < cir.back().err) {
					cir.pop_back();
					cir.push_back(tempcir);
				}
			}
		}
		std::sort(cir.begin(), cir.end(), [](circledata a, circledata b)
			{return a.err < b.err;});//按mse升序排列
		cir.resize(80);//保留前80个
		for (int j = 0;j < 80;j++) {//再次爬山
			for (int k = 0;k < 16;k++) {
				tempcir = (climb(cir[j], carvan));
				if (tempcir.err < cir[j].err) {
					cir[j] = tempcir;
				}
			}
		}
		std::sort(cir.begin(), cir.end(), [](circledata a, circledata b)
			{return a.err < b.err;});//按mse升序排列
		cir.resize(30);//保留前30个
		for (int j = 0;j < 30;j++) {//再次爬山
			for (int k = 0;k < 40;k++) {
				tempcir = (climb(cir[j], carvan));
				if (tempcir.err < cir[j].err) {
					cir[j] = tempcir;
				}
			}
		}
		std::sort(cir.begin(), cir.end(), [](circledata a, circledata b)
			{return a.err < b.err;});//按mse升序排列
		cir.resize(10);//保留前10个
		for (int j = 0;j < 10;j++) {//再次爬山
			for (int k = 0;k < 80;k++) {
				tempcir = (climb(cir[j], carvan));
				if (tempcir.err < cir[j].err) {
					cir[j] = tempcir;
				}
			}
		}
		std::sort(cir.begin(), cir.end(), [](circledata a, circledata b)
			{return a.err < b.err;});//按mse升序排列
		cir.resize(1);//选出第一个
		circle(carvan, Point(cir[0].x, cir[0].y), cir[0].rad,
			Scalar(cir[0].b, cir[0].g, cir[0].r),-1);
		cir.clear();//清空vector数组
	}
	final = f2u(carvan);
	if (!final.empty()) {
		cv::imwrite("output.jpg", final);
	}
	//imwrite("D:\\output.png", final);
	return 0;
}
cpp picture_circle_simulate\main.cpp
#include "calc_error.h"
#include "mountain_climbing.h"
#include "convert.h"
#include <iostream>
using namespace std;
using namespace cv;
int main() {
	try {
		//RNG rng(getTickCount());
		int cirnum, rows, cols;
		circledata tempcir;
		vector<circledata> cir;
		if(!(cin >> cirnum)) {
			cerr << "读取 cirnum 失败" << endl;
			return -1;
		}
		Mat imgu = imread("D:\\1.jpeg");
		if (imgu.empty()) {
			cout << "图片加载失败" << endl;
			return 0;
		}
		rows = imgu.rows;
		cols = imgu.cols;
		Scalar car = mean(imgu);
		Mat carvanu(rows, cols, CV_8UC3, car);
		Mat carvan = u2f(carvanu);
		if (carvan.empty()) {
			cerr << "u2f 返回空 Mat" << endl;
			return -1;
		}
		// 诊断输出
		cerr << "carvan: rows=" << carvan.rows << " cols=" << carvan.cols << " type=" << carvan.type() << endl;
		Mat final;
		for (int i = 0;i < cirnum;i++) {
			for (int j = 0;j < 200;j++) {
				circledata cd = calc_error(rows, cols, carvan);
				// 简单验证返回的数据
				if (cd.rad <= 0 || cd.x < 0 || cd.y < 0) {
					cerr << "calc_error 返回非法 circledata: rad=" << cd.rad << " x=" << cd.x << " y=" << cd.y << endl;
					continue;
				}
				cir.push_back(cd);
				for (int k = 0;k < 8;k++) {
					tempcir = (climb(cir.back(), carvan));
					if (tempcir.err < cir.back().err) {
						cir.pop_back();
						cir.push_back(tempcir);
					}
				}
			}
			std::sort(cir.begin(), cir.end(), [](circledata a, circledata b)
				{return a.err < b.err;});//按mse升序排列
			cir.resize(80);//保留前80个
			for (int j = 0;j < 80;j++) {//再次爬山
				for (int k = 0;k < 16;k++) {
					tempcir = (climb(cir[j], carvan));
					if (tempcir.err < cir[j].err) {
						cir[j] = tempcir;
					}
				}
			}
			std::sort(cir.begin(), cir.end(), [](circledata a, circledata b)
				{return a.err < b.err;});//按mse升序排列
			cir.resize(30);//保留前30个
			for (int j = 0;j < 30;j++) {//再次爬山
				for (int k = 0;k < 40;k++) {
					tempcir = (climb(cir[j], carvan));
					if (tempcir.err < cir[j].err) {
						cir[j] = tempcir;
					}
				}
			}
			std::sort(cir.begin(), cir.end(), [](circledata a, circledata b)
				{return a.err < b.err;});//按mse升序排列
			cir.resize(10);//保留前10个
			for (int j = 0;j < 10;j++) {//再次爬山
				for (int k = 0;k < 80;k++) {
					tempcir = (climb(cir[j], carvan));
					if (tempcir.err < cir[j].err) {
						cir[j] = tempcir;
					}
				}
			}
			std::sort(cir.begin(), cir.end(), [](circledata a, circledata b)
				{return a.err < b.err;});//按mse升序排列
			cir.resize(1);//选出第一个
			// 绘制前再验证
			if (cir.empty()) {
				cerr << "没有候选圆" << endl;
			} else {
				auto &c = cir[0];
				if (c.rad > 0 && c.x >= 0 && c.x < cols && c.y >= 0 && c.y < rows) {
					circle(carvan, Point(c.x, c.y), c.rad,
						Scalar(c.b, c.g, c.r),-1);
				} else {
					cerr << "跳过非法圆: x="<<c.x<<" y="<<c.y<<" rad="<<c.rad<<endl;
				}
			}
			cir.clear();//清空vector数组
		}
		final = f2u(carvan);
		if (!final.empty()) {
			cv::imwrite("output.jpg", final);
		}
		return 0;
	} catch (const cv::Exception &e) {
		cerr << "OpenCV 异常: " << e.what() << endl;
		return -1;
	} catch (const std::exception &e) {
		cerr << "Std 异常: " << e.what() << endl;
		return -1;
	} catch (...) {
		cerr << "未知异常" << endl;
		return -1;
	}
}*/
#include "calc_error.h"
#include "mountain_climbing.h"
#include "convert.h"
#include <iostream>
using namespace std;
using namespace cv;

int main() {
	try {
		//Mat imgu;
		int cirnum, rows, cols;
		circledata tempcir;
		vector<circledata> cir;
		if (!(cin >> cirnum) || cirnum <= 0) {
			cerr << "Failed to read cirnum." << endl;
			return -1;
		}
		Mat imgu = imread("D:\\1.jpg");
		if (imgu.empty()) {
			cout << "Failed to load image." << endl;
			return 0;
		}
		//cvtColor(imgubgr, imgu, COLOR_BGR2BGRA);
		rows = imgu.rows;
		cols = imgu.cols;
		Scalar car = mean(imgu);
		Mat carvanu(rows, cols, CV_8UC3, car);
		//imwrite("d:\\test.png", carvanu);
		//return 0;
		Mat carvan = u2f(carvanu);//f
		Mat img = u2f(imgu);
		cout << "Debug Info:" << endl;
		cout << "carvanu type: " << carvanu.type() << " (should be 16 for CV_8UC3)" << endl;
		cout << "carvanu elemSize: " << carvanu.elemSize() << endl;
		cout << "carvan type: " << carvan.type() << " (should be 21 for CV_32FC3)" << endl;
		cout << "carvan elemSize: " << carvan.elemSize() << endl;
		if (carvan.empty()) {
			cerr << "u2f returned an empty Mat." << endl;
			return -1;
		}

		auto refine_candidates = [&](size_t keepCount, int climbCount) {
			if (cir.empty()) {
				return;
			}

			sort(cir.begin(), cir.end(), [](const circledata& a, const circledata& b) {
				return a.err < b.err;
				});
			if (cir.size() > keepCount) {
				cir.resize(keepCount);
			}
//#pragma omp parallel for
			for (int index = 0; index < cir.size(); ++index) {
				for (int step = 0; step < climbCount; ++step) {
					tempcir = climb(cir[index], carvan, img);
					if (tempcir.err < cir[index].err) {
						cir[index] = tempcir;
					}
				}
			}
			};

		Mat final;
//#pragma omp parallel for
		for (int i = 0; i < cirnum; i++) {
			cout << "=== Processing Circle " << (i + 1) << " / " << cirnum << " ===" << endl;
			cir.clear();
			cout << "  Generating initial candidates..." << flush;
			for (int j = 0; j < 200; j++) {
				circledata cd = calc_error(rows, cols, carvan, img);
				if (cd.rad <= 0 || cd.x < 0 || cd.x >= cols || cd.y < 0 || cd.y >= rows || cd.diff.empty()) {
					continue;
				}
				cir.push_back(cd);
				for (int k = 0; k < 8; k++) {
					tempcir = (climb(cir.back(), carvan, img));
					if (tempcir.err < cir.back().err) {
						cir.back() = tempcir;
					}
				}
			}

			if (cir.empty()) {
				continue;
			}
			cout << "  Refining candidates..." << flush;
			refine_candidates(80, 16);
			cout << "." << flush;
			refine_candidates(30, 40);
			cout << "." << flush;
			refine_candidates(10, 80);
			cout << " Done." << endl;
			sort(cir.begin(), cir.end(), [](const circledata& a, const circledata& b) {
				return a.err < b.err;
				});

			const circledata& best = cir.front();
			cout << "  Best found: (x=" << best.x << ", y=" << best.y 
				<< ", rad=" << best.rad << ", err=" << best.err << ", b=" 
				<< best.b * 65535 << ", g=" << best.g * 65535 << ", r=" << best.r * 65535 << ", alpha="
				<< best.alpha<< endl;
			if (best.rad > 0/* && best.x >= 0 && best.x < cols && best.y >= 0 && best.y < rows*/) {
				/*circle(carvan, Point(best.x, best.y), best.rad,
					Scalar(best.b, best.g, best.r), -1);*/
				Mat mask = Mat::zeros(carvan.size(), CV_8UC1);
				circle(mask, Point(best.x, best.y), best.rad, Scalar(255), -1);//生成掩码
				Rect rect = boundingRect(mask);
				for (int y = rect.y;y < rect.y + rect.height;y++) {//进行alpha叠加
					if (y < 0 || y >= carvan.rows) continue;
					const uchar* maskrow = mask.ptr<uchar>(y);
					float* carvanrow = carvan.ptr<float>(y);
					//float* tempcarrow = tempcar.ptr<float>(y);
					for (int x = rect.x;x < rect.x + rect.width;x++) {
						if (x < 0 || x >= carvan.cols) continue;
						if (maskrow[x]) {//判断该像素是否在圆上
							int idx = x * 3;
							carvanrow[idx] = best.b * best.alpha + carvanrow[idx] * (1 - best.alpha);
							carvanrow[idx + 1] = best.g * best.alpha + carvanrow[idx + 1] * (1 - best.alpha);
							carvanrow[idx + 2] = best.r * best.alpha + carvanrow[idx + 2] * (1 - best.alpha);
						}
					}
				}
			}
		}
		cout << "All circles processed. Writing image..." << endl;
		final = f2u(carvan);
		if (!final.empty()) {
			cv::imwrite("D:\\output.jpg", final);
		}
		return 0;
	}
	catch (const cv::Exception& e) {
		cerr << "OpenCV exception: " << e.what() << endl;
		return -1;
	}
	catch (const std::exception& e) {
		cerr << "Standard exception: " << e.what() << endl;
		return -1;
	}
	catch (...) {
		cerr << "Unknown exception." << endl;
		return -1;
	}
}
