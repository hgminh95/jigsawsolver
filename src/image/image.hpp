#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>
#include <string>
#include "../utility/list.hpp"
#include "../utility/convert.hpp"

namespace JigsawSolver {
	const int hx[] = { 0, 0, -1, 1 };
	const int hy[] = { -1, 1, 0, 0 };

	enum Direction { left, right, up, down };

	struct Color {
		int red, green, blue;

		void setColor(int _red, int _green, int _blue);
	};

	class Image {
	public:
		List < List<Color> > bitmap;
		int width, height;
		int maxColor;

		Image();
		~Image();

		void exportToImage(Image &img, int leftTop_x, int leftTop_y, int rightBot_x, int rightBot_y);
		void clear();
		void resize(int _width, int _height);
		void importFrom(const char* matrix, int _width, int _height, int _maxColor);
	};

	class PPMImage : public Image {
	public:
		void exportTo(const char *outputPath, int leftTop_x, int leftTop_y, int rightBot_x, int rightBot_y);
		void importFrom(const char *inputPath);
	};
}

#endif
