#ifndef METRICS_H
#define METRICS_H

#include "image.h"
#include <cmath>
#include <algorithm>

long double lpq(PROCON::Image& piece1, PROCON::Image& piece2, PROCON::Direction direction);

long double prediction(PROCON::Image& piece1, PROCON::Image& piece2, PROCON::Direction direction);

long double median(long double *begin, long double *end);

void initializeConfidence(long double *begin, long double *end);

#endif