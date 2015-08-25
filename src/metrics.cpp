#include "metrics.hpp"

long double lpq(PROCON::Image& piece1, PROCON::Image& piece2, PROCON::Direction direction){
	long double res = 0.0;

	int _height = piece1.height;
	int _width = piece1.width;

	if (direction == PROCON::Direction::left){
		for (int i = 0; i < _height; i++){
			res = res + pow(abs(piece1.bitmap[i][0].red - piece2.bitmap[i][_width - 1].red), 3.0 / 10);
			res = res + pow(abs(piece1.bitmap[i][0].green - piece2.bitmap[i][_width - 1].green), 3.0 / 10);
			res = res + pow(abs(piece1.bitmap[i][0].blue - piece2.bitmap[i][_width - 1].blue), 3.0 / 10);
		}
	}

	if (direction == PROCON::Direction::right){
		for (int i = 0; i < _height; i++){
			res = res + pow(abs(piece1.bitmap[i][_width - 1].red - piece2.bitmap[i][0].red), 3.0 / 10);
			res = res + pow(abs(piece1.bitmap[i][_width - 1].green - piece2.bitmap[i][0].green), 3.0 / 10);
			res = res + pow(abs(piece1.bitmap[i][_width - 1].blue - piece2.bitmap[i][0].blue), 3.0 / 10);
		}
	}

	if (direction == PROCON::Direction::up){
		for (int i = 0; i < _width; i++){
			res = res + pow(abs(piece1.bitmap[0][i].red - piece2.bitmap[_height - 1][i].red), 3.0 / 10);
			res = res + pow(abs(piece1.bitmap[0][i].green - piece2.bitmap[_height - 1][i].green), 3.0 / 10);
			res = res + pow(abs(piece1.bitmap[0][i].blue - piece2.bitmap[_height - 1][i].blue), 3.0 / 10);
		}
	}

	if (direction == PROCON::Direction::down){
		for (int i = 0; i < _width; i++){
			res = res + pow(abs(piece1.bitmap[_height - 1][i].red - piece2.bitmap[0][i].red), 3.0 / 10);
			res = res + pow(abs(piece1.bitmap[_height - 1][i].green - piece2.bitmap[0][i].green), 3.0 / 10);
			res = res + pow(abs(piece1.bitmap[_height - 1][i].blue - piece2.bitmap[0][i].blue), 3.0 / 10);
		}
	}

	return pow(res, 5.0 / 24);
}

long double prediction(PROCON::Image& piece1, PROCON::Image& piece2, PROCON::Direction direction){
	long double res = 0.0;

	int _height = piece1.height;
	int _width = piece1.width;

	if (direction == PROCON::Direction::left){
		for (int i = 0; i < _height; i++){
			long double tmp1 = 2 * piece1.bitmap[i][0].red - piece1.bitmap[i][1].red - piece2.bitmap[i][_width - 1].red;
			res = res + pow(abs(tmp1), 3.0 / 10);

			tmp1 = 2 * piece1.bitmap[i][0].green - piece1.bitmap[i][1].green - piece2.bitmap[i][_width - 1].green;
			res = res + pow(abs(tmp1), 3.0 / 10);

			tmp1 = 2 * piece1.bitmap[i][0].blue - piece1.bitmap[i][1].blue - piece2.bitmap[i][_width - 1].blue;
			res = res + pow(abs(tmp1), 3.0 / 10);
		}
	}
	if (direction == PROCON::Direction::right){
		for (int i = 0; i < _height; i++){
			long double tmp1 = 2 * piece1.bitmap[i][_width - 1].red - piece1.bitmap[i][_width - 2].red - piece2.bitmap[i][0].red;
			res = res + pow(abs(tmp1), 3.0 / 10);

			tmp1 = 2 * piece1.bitmap[i][_width - 1].green - piece1.bitmap[i][_width - 2].green - piece2.bitmap[i][0].green;
			res = res + pow(abs(tmp1), 3.0 / 10);

			tmp1 = 2 * piece1.bitmap[i][_width - 1].blue - piece1.bitmap[i][_width - 2].blue - piece2.bitmap[i][0].blue;
			res = res + pow(abs(tmp1), 3.0 / 10);
		}
	}
	if (direction == PROCON::Direction::up){
		for (int i = 0; i < _width; i++){
			long double tmp1 = 2 * piece1.bitmap[0][i].red - piece1.bitmap[1][i].red - piece2.bitmap[_height - 1][i].red;
			res = res + pow(abs(tmp1), 3.0 / 10);

			tmp1 = 2 * piece1.bitmap[0][i].green - piece1.bitmap[1][i].green - piece2.bitmap[_height - 1][i].green;
			res = res + pow(abs(tmp1), 3.0 / 10);

			tmp1 = 2 * piece1.bitmap[0][i].blue - piece1.bitmap[1][i].blue - piece2.bitmap[_height - 1][i].blue;
			res = res + pow(abs(tmp1), 3.0 / 10);
		}
	}
	if (direction == PROCON::Direction::down){
		for (int i = 0; i < _width; i++){
			long double tmp1 = 2 * piece1.bitmap[_height - 1][i].red - piece1.bitmap[_height - 2][i].red - piece2.bitmap[0][i].red;
			res = res + pow(abs(tmp1), 3.0 / 10);

			tmp1 = 2 * piece1.bitmap[_height - 1][i].green - piece1.bitmap[_height - 2][i].green - piece2.bitmap[0][i].green;
			res = res + pow(abs(tmp1), 3.0 / 10);

			tmp1 = 2 * piece1.bitmap[_height - 1][i].blue - piece1.bitmap[_height - 2][i].blue - piece2.bitmap[0][i].blue;
			res = res + pow(abs(tmp1), 3.0 / 10);
		}
	}

	return pow(res, 5.0 / 24);
}

long double median(long double *begin, long double *end){
	int n = end - begin;
	if ((n + 1) % 2 == 0) return *(begin + (n + 1) / 2 - 1);

	return (*(begin + n / 2) + *(begin + n / 2 - 1)) / 2.0;
}

void initializeConfidence(long double *begin, long double *end){
	int n = end - begin;
	long double *sortedConfidence = new long double[n];

	long double *it;
	int i = 0;
	for (it = begin; it != end; it++){
		sortedConfidence[i] = *it;
		i++;
	}

	std::sort(sortedConfidence, sortedConfidence + n);

	long double firstQuartile = median(sortedConfidence, sortedConfidence + (int)ceil(n / 2));

	for (it = begin; it != end; it++)
		*it = exp(-(*it) / firstQuartile);

	delete[] sortedConfidence;
}
