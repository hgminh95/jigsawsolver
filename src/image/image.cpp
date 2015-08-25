#include "image.hpp"

namespace JigsawSolver {
	void Color::setColor(int _red, int _green, int _blue) { 
		red = _red;
		green = _green;
		blue = _blue;
	}

	Image::Image() {
		width = 0;
		height = 0;
		maxColor = 0;
	}

	Image::~Image() {
		bitmap.clear();
	}

	void Image::exportToImage(Image &img, int leftTop_x, int leftTop_y, int rightBot_x, int rightBot_y) {
		int _height = rightBot_x - leftTop_x;
		int _width = rightBot_y - leftTop_y;

		img.resize(_height, _width);

		for (int i = leftTop_x; i < rightBot_x; i++)
			for (int j = leftTop_y; j < rightBot_y; j++)
				img.bitmap[i - leftTop_x][j - leftTop_y] = bitmap[i][j];
	}

	void Image::clear() {
		bitmap.clear();
		width = 0;
		height = 0;
	}

	void Image::resize(int _height, int _width) {
		clear();
		height = _height;
		width = _width;

		bitmap.resize(_height);
		for (int i = 0; i < _height; i++) bitmap[i].resize(_width);
	}

	void Image::importFrom(const char* matrix, int _height, int _width, int _maxColor) {
		resize(_height, _width);

		maxColor = _maxColor;
		int bytePerSample = maxColor <= 256 ? 1 : 2;

		for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		if (bytePerSample == 1)
			bitmap[i][j].setColor((unsigned char) matrix[(i * width + j) * 3],
			(unsigned char) matrix[(i * width + j) * 3 + 1],
			(unsigned char) matrix[(i * width + j) * 3 + 2]
			);
		else
			bitmap[i][j].setColor((unsigned char) matrix[(i * width + j) * 6] * 256 + (unsigned char) matrix[(i * width + j) * 6 + 1],
			(unsigned char) matrix[(i * width + j) * 6 + 2] * 256 + (unsigned char) matrix[(i * width + j) * 6 + 3],
			(unsigned char) matrix[(i * width + j) * 6 + 4] * 256 + (unsigned char) matrix[(i * width + j) * 6 + 5]
			);
	}

	void PPMImage::exportTo(const char* outputPath, int leftTop_x, int leftTop_y, int rightBot_x, int rightBot_y) {
		std::ofstream outputFile(outputPath, std::ios::binary);

		int _height = rightBot_x - leftTop_x;
		int _width = rightBot_y - leftTop_y;

		outputFile << "P6" << char(10);
		outputFile << "# " << nRows << " " << nColumns << char(10);
		outputFile << "# " << maxSelections << char(10);
		outputFile << "# " << selectCost << " " << swapCost << char(10);
		outputFile << _width << " " << _height << char(10);
		outputFile << maxColor << char(10);

		int bytePerSamples = maxColor > 255 ? 2 : 1;

		for (int i = leftTop_x; i < rightBot_x; i++)
		for (int j = leftTop_y; j < rightBot_y; j++) {
			char *tmp = toChar(bitmap[i][j].red, bytePerSamples);
			outputFile.write(tmp, bytePerSamples);
			delete[] tmp;

			tmp = toChar(bitmap[i][j].green, bytePerSamples);
			outputFile.write(tmp, bytePerSamples);
			delete[] tmp;

			tmp = toChar(bitmap[i][j].blue, bytePerSamples);
			outputFile.write(tmp, bytePerSamples);
			delete[] tmp;
		}

		outputFile.close();
	}

	bool isNumber(char x) {
		return x >= '0' && x <= '9';
	}

	char* read(char* buffer, int& integer) {
		int i = 0;
		while (!isNumber(buffer[i])) i++;

		std::string s = "";
		while (isNumber(buffer[i])) {
			s = s + buffer[i];
			i++;
		}

		integer = toInteger(s);

		return buffer + i;
	}

	char* read_ignore_comment(char* buffer, int& integer) {
		int i = 0;
		while (!isNumber(buffer[i])){
			if (buffer[i] == '#')
			while (buffer[i] != 0 && buffer[i] != 13 && buffer[i] != 10) i++;
			else i++;
		}

		std::string s = "";
		while (isNumber(buffer[i])) {
			s = s + buffer[i];
			i++;
		}

		integer = toInteger(s);

		return buffer + i;
	}

	void PPMImage::importFrom(const char *inputPath) {
		std::ifstream inputFile(inputPath, std::fstream::binary);

		inputFile.seekg(0, inputFile.end);
		long long length = inputFile.tellg();
		inputFile.seekg(0, inputFile.beg);

		char *buffer = new char[length];
		inputFile.read(buffer, length);
		inputFile.close();

		char *crrPtr = buffer;
		crrPtr = read(crrPtr, nRows);

		crrPtr = read(crrPtr, nColumns);
		crrPtr = read(crrPtr, nRows);
		crrPtr = read(crrPtr, maxSelections);
		crrPtr = read(crrPtr, selectCost);
		crrPtr = read(crrPtr, swapCost);
		crrPtr = read(crrPtr, width);
		crrPtr = read(crrPtr, height);
		crrPtr = read(crrPtr, maxColor);

		crrPtr++;
		Image::importFrom(crrPtr, height, width, maxColor);

		delete[] buffer;
	}

	void PPMImage::importFromWithoutComment(const char *inputPath) {
		std::ifstream inputFile(inputPath, std::fstream::binary);

		inputFile.seekg(0, inputFile.end);
		long long length = inputFile.tellg();
		inputFile.seekg(0, inputFile.beg);

		char *buffer = new char[length];
		inputFile.read(buffer, length);
		inputFile.close();

		char *crrPtr = buffer;
		crrPtr = read_ignore_comment(crrPtr, nRows);
		crrPtr = read_ignore_comment(crrPtr, width);
		crrPtr = read_ignore_comment(crrPtr, height);
		crrPtr = read_ignore_comment(crrPtr, maxColor);

		crrPtr++;
		Image::importFrom(crrPtr, height, width, maxColor);

		delete[] buffer;
	}

	bool operator== (Image& image1, Image& image2) {
		if (image1.height != image2.height || image1.width != image2.width) return false;

		for (int i = 0; i < image1.height; i++)
			for (int j = 0; j < image1.width; j++)
				if (image1.bitmap[i][j].red != image2.bitmap[i][j].red
					|| image1.bitmap[i][j].green != image2.bitmap[i][j].green
					|| image1.bitmap[i][j].blue != image2.bitmap[i][j].blue) return false;

		return true;
	}
}
