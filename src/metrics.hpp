#ifndef __METRICS_H__
#define __METRICS_H__

#include "image/image.hpp"
#include <cmath>
#include <algorithm>

long double lpq(JigsawSolver::Image& piece1, JigsawSolver::Image& piece2, JigsawSolver::Direction direction);

long double prediction(JigsawSolver::Image& piece1, JigsawSolver::Image& piece2, JigsawSolver::Direction direction);

long double median(long double *begin, long double *end);

void initializeConfidence(long double *begin, long double *end);

#endif
