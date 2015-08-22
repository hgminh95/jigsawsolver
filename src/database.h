#ifndef DATABASE_H
#define DATABASE_H

#define INF 100000000
#define EPS .000001

#include <vector>
#include <string>
#include "metrics.h"
#include "image.h"

class Database
{
public:
	std::string name[10];
	double compatibility[10][266][266][4];
	bool isInititate[10];
	int selectedMetric;

	int isBestBuddies[10][266][266][4];
	int bestBuddies[10][256][4];

	PROCON::PPMImage originalImage;
	PROCON::Image piece[256];

	bool containPiece(int pieceIndex) const;
	void cutImageIntoPieces();
	void calculateBestBuddies(int type);

	int nRows, nColumns;

	Database();
	~Database();

	void importFromImageFile(std::string path);

	double getCompatibility(const std::string& type, int piece1, int piece2, int direction) const;
	double getCompatibility(int piece1, int piece2, int direction) const;
	
	bool isBestBuddy(int piece1, int piece2, int direction) const;
	int getBestBuddyOf(int piece, int direction) const;

	void requestCalculateMetric(const std::string& type);

	void exportToFile(const std::string& path);
	void importFromDatabaseFile(const std::string& path);
};

#endif