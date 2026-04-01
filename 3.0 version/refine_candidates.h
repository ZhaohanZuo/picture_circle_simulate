#pragma once
#include "mountain_climbing.h"
#include <omp.h>
void refine_candidates(size_t, int, const Mat&, const Mat&, vector<circledata>&);