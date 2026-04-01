#include "refine_candidates.h"
void refine_candidates(size_t keepCount, int climbCount, const Mat& carvan,
	const Mat& img, vector<circledata>& cir) {
	if (cir.empty()) {
		return;
	}

	sort(cir.begin(), cir.end(), [](const circledata& a, const circledata& b) {
		return a.err < b.err;
		});
	if (cir.size() > keepCount) {
		cir.resize(keepCount);
	}
#pragma omp parallel for shared(carvan, img ,cir)//开始并行并将两个Mat对象赋给每个线程
	for (int index = 0; index < cir.size(); ++index) {
		circledata local_best = cir[index];//定义线程内存储变量
		circledata tempcir_local;
		for (int step = 0; step < climbCount; ++step) {
			tempcir_local = climb(local_best, carvan, img);
			if (tempcir_local.err < local_best.err) {
				local_best = tempcir_local;
			}
		}
		cir[index] = local_best;
	}
	return;
}