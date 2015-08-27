#ifndef __DATABASE_H__
#define __DATABASE_H__

#define INF 100000000
#define EPS .000001

#include <vector>
#include <string>
#include "metrics.hpp"
#include "image/image.hpp"

namespace JigsawSolver {
	class Database {
	private:
		std::string name[10];
		double compatibility[10][266][266][4];
		bool isInititate[10];
		int isBestBuddies[10][266][266][4];
		int bestBuddies[10][256][4];

		int mRowsCount, mColumnsCount;

		static bool instanceFlag;
		static Database *database;
		Database();

	public:
		~Database();

		static Database *getInstance();

		JigsawSolver::PPMImage originalImage;
		JigsawSolver::Image piece[256];
		int selectedMetric;

		int getRowsCount();
		int getColumnsCount();

		bool containPiece(int pieceIndex) const;
		void cutImageIntoPieces();
		void calculateBestBuddies(int type);

		void importFromImageFile(std::string path, int rowsCount, int columnsCount);

		double getCompatibility(const std::string& type, int piece1, int piece2, int direction) const;
		double getCompatibility(int piece1, int piece2, int direction) const;

		bool isBestBuddy(int piece1, int piece2, int direction) const;
		int getBestBuddyOf(int piece, int direction) const;

		void requestCalculateMetric(const std::string& type);
	};
}

#endif
