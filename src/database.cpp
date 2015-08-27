#include "database.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace JigsawSolver;

long double abs(long double a) {
	return a > 0 ? a : -a;
}

bool Database::instanceFlag = false;
Database *Database::database = NULL;

Database *Database::getInstance() {
	if (!instanceFlag) {
		database = new Database();
		instanceFlag = true;
	}
	return database;
}

int Database::getRowsCount() {
	return mRowsCount;
}

int Database::getColumnsCount() {
	return mColumnsCount;
}

Database::Database() {
	for (int i = 0; i < 10; i++) {
		name[i] = "";
		isInititate[i] = false;
	}

	name[0] = "LPQ";
	name[1] = "PREDICTION";
	name[2] = "GRADIENT";
}


Database::~Database() {
	instanceFlag = false;
}

void Database::importFromImageFile(std::string path, int rowsCount, int columnsCount) {
	mRowsCount = rowsCount;
	mColumnsCount = columnsCount;

	std::cout << "DATABASE INITITALIZING..." << std::endl;
	std::cout << "---From File: " << path << std::endl;
	originalImage.importFrom(path.c_str());

	std::cout << "---Cutting image into pieces...";
	cutImageIntoPieces();
	std::cout << "Done." << std::endl;

	std::cout << "---Calculating LPQ metrics...";
	requestCalculateMetric("LPQ");
	std::cout << "Done." << std::endl;

	std::cout << "---Calculating PREDICTION metric...";
	requestCalculateMetric("PREDICTION");
	std::cout << "Done." << std::endl;
}

double Database::getCompatibility(const std::string& type, int piece1, int piece2, int direction) const {
	if (!containPiece(piece1) || !containPiece(piece2)) return INF;

	for (int i = 0; i < 10; i++) {
		if (!isInititate[i]) continue;
		if (type != name[i]) continue;

		return compatibility[i][piece1][piece2][direction];
	}

	return INF;
}

double Database::getCompatibility(int piece1, int piece2, int direction) const {
	if (!containPiece(piece1) || !containPiece(piece2)) return INF;

	return compatibility[selectedMetric][piece1][piece2][direction];
}

bool Database::isBestBuddy(int piece1, int piece2, int direction) const {
	if (!containPiece(piece1) || !containPiece(piece2)) return false;
	return isBestBuddies[selectedMetric][piece1][piece2][direction] == 2;
}

int Database::getBestBuddyOf(int pieceIndex, int direction) const {
	if (!containPiece(pieceIndex)) return -1;

	return bestBuddies[selectedMetric][pieceIndex][direction];
}


bool Database::containPiece(int pieceIndex) const {
	return pieceIndex >= 0 && pieceIndex < mRowsCount * mColumnsCount;
}

void Database::requestCalculateMetric(const std::string& type) {
	for (int ii = 0; ii < 10; ii++) {
		if (type != name[ii]) continue;
		if (isInititate[ii]) return;

		// calculate metric "type"
		for (int i = 0; i < mRowsCount; i++)
		for (int j = 0; j < mColumnsCount; j++)
		for (int p = 0; p < mRowsCount; p++)
		for (int q = 0; q < mColumnsCount; q++){
			if (ii == 0) {
				compatibility[ii][i * mColumnsCount + j][p * mColumnsCount + q][0] = lpq(piece[i * mColumnsCount + j], piece[p * mColumnsCount + q], JigsawSolver::Direction::left);
				compatibility[ii][i * mColumnsCount + j][p * mColumnsCount + q][1] = lpq(piece[i * mColumnsCount + j], piece[p * mColumnsCount + q], JigsawSolver::Direction::right);
				compatibility[ii][i * mColumnsCount + j][p * mColumnsCount + q][2] = lpq(piece[i * mColumnsCount + j], piece[p * mColumnsCount + q], JigsawSolver::Direction::up);
				compatibility[ii][i * mColumnsCount + j][p * mColumnsCount + q][3] = lpq(piece[i * mColumnsCount + j], piece[p * mColumnsCount + q], JigsawSolver::Direction::down);
			}
			else if (ii == 1) {
				compatibility[ii][i * mColumnsCount + j][p * mColumnsCount + q][0] = prediction(piece[i * mColumnsCount + j], piece[p * mColumnsCount + q], JigsawSolver::Direction::left);
				compatibility[ii][i * mColumnsCount + j][p * mColumnsCount + q][1] = prediction(piece[i * mColumnsCount + j], piece[p * mColumnsCount + q], JigsawSolver::Direction::right);
				compatibility[ii][i * mColumnsCount + j][p * mColumnsCount + q][2] = prediction(piece[i * mColumnsCount + j], piece[p * mColumnsCount + q], JigsawSolver::Direction::up);
				compatibility[ii][i * mColumnsCount + j][p * mColumnsCount + q][3] = prediction(piece[i * mColumnsCount + j], piece[p * mColumnsCount + q], JigsawSolver::Direction::down);
			}
			else if (ii == 2) {

			}
		}

		calculateBestBuddies(ii);
	}

}

void Database::calculateBestBuddies(int type) {
	// init
	for (int i = 0; i < mRowsCount * mColumnsCount; i++)
		for (int j = 0; j < mRowsCount * mColumnsCount; j++)
			for (int h = 0; h < 4; h++)
				isBestBuddies[type][i][j][h] = 0;

	for (int i = 0; i < mRowsCount * mColumnsCount; i++)
		for (int h = 0; h < 4; h++) {
			long double _min = 100000000.0;
			for (int j = 0; j < mRowsCount * mColumnsCount; j++)
			if (compatibility[type][i][j][h] < _min) _min = compatibility[type][i][j][h];

			for (int j = 0; j < mRowsCount * mColumnsCount; j++)
			if (abs(compatibility[type][i][j][h] - _min) < EPS) {
				isBestBuddies[type][i][j][h]++;

				int hh;
				switch (h) {
					case 0: hh = 1; break;
					case 1: hh = 0; break;
					case 2: hh = 3; break;
					case 3: hh = 2; break;
				}

				isBestBuddies[type][j][i][hh]++;
			}
	}

	int cntBestBuddies = 0;
	for (int i = 0; i < mRowsCount * mColumnsCount; i++)
		for (int h = 0; h < 4; h++)
			for (int j = 0; j < mRowsCount * mColumnsCount; j++)
				if (isBestBuddies[type][i][j][h] == 2) {
					bestBuddies[type][i][h] = j;
					cntBestBuddies++;
				}
}

void Database::cutImageIntoPieces() {
	int piece_height = originalImage.height / mRowsCount;
	int piece_width = originalImage.width / mColumnsCount;

	for (int i = 0; i < mRowsCount; i++)
	for (int j = 0; j < mColumnsCount; j++)
		originalImage.exportToImage(piece[i * mColumnsCount + j], i * piece_height, j * piece_width, (i + 1) * piece_height, (j + 1) * piece_width);
}
