#include "database.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace JigsawSolver;

double abs(double a) {
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
}


Database::~Database() {
  instanceFlag = false;
}

void Database::importFromImageFile(std::string path, int rowsCount, int columnsCount) {
  mRowsCount = rowsCount;
  mColumnsCount = columnsCount;
  alloc();

  std::cout << "Get information about image: " << path << " ..." << std::flush;
  originalImage.importFrom(path.c_str());
  cutImageIntoPieces();
  std::cout << "Done." << std::endl;
}

double Database::getCompatibility(int piece1, int piece2, int direction) const {
  if (!containPiece(piece1) || !containPiece(piece2)) return INF;

  return compatibility[direction][piece1][piece2];
}

bool Database::isBestBuddy(int piece1, int piece2, int direction) const {
  if (!containPiece(piece1) || !containPiece(piece2)) return false;

  return getBestBuddyOf(piece1, direction) == piece2;
}

int Database::getBestBuddyOf(int pieceIndex, int direction) const {
  if (!containPiece(pieceIndex)) return -1;

  return bestBuddies[direction][pieceIndex];
}


bool Database::containPiece(int pieceIndex) const {
  return pieceIndex >= 0 && pieceIndex < mRowsCount * mColumnsCount;
}

void Database::requestCalculateMetric(const std::string& type) {
  for (int i = 0; i < mRowsCount * mColumnsCount; i++)
    for (int j = 0; j < mRowsCount * mColumnsCount; j++) {
      if (type == "LPQ") {
        compatibility[0][i][j] = lpq(piece[i], piece[j], JigsawSolver::Direction::left);
        compatibility[1][i][j] = lpq(piece[i], piece[j], JigsawSolver::Direction::right);
        compatibility[2][i][j] = lpq(piece[i], piece[j], JigsawSolver::Direction::up);
        compatibility[3][i][j] = lpq(piece[i], piece[j], JigsawSolver::Direction::down);
      }
      else if (type == "PREDICTION") {
        compatibility[0][i][j] = prediction(piece[i], piece[j], JigsawSolver::Direction::left);
        compatibility[1][i][j] = prediction(piece[i], piece[j], JigsawSolver::Direction::right);
        compatibility[2][i][j] = prediction(piece[i], piece[j], JigsawSolver::Direction::up);
        compatibility[3][i][j] = prediction(piece[i], piece[j], JigsawSolver::Direction::down);
      }
    }

  calculateBestBuddies();
}

void Database::calculateBestBuddies() {
  // init
  for (int h = 0; h < 4; h++)
    for (int i = 0; i < mRowsCount * mColumnsCount; i++)
        bestBuddies[h][i] = -1;

  for (int h = 0; h < 4; h++)
    for (int i = 0; i < mRowsCount * mColumnsCount; i++) {
      double _min = 100000000.0;
      for (int j = 0; j < mRowsCount * mColumnsCount; j++)
        if (compatibility[h][i][j] < _min)
          _min = compatibility[h][i][j];

      for (int j = 0; j < mRowsCount * mColumnsCount; j++)
        if (abs(compatibility[h][i][j] - _min) < EPS) {
          bestBuddies[h][i] = j;
        }
  }

  for (int h = 0; h < 4; h++) {
    int hh;
    if (h == 0 || h == 1) hh = 1 - h;
    if (h == 2 || h == 3) hh = 5 - h;
    for (int i = 0; i < mRowsCount * mColumnsCount; i++) {
      int other = bestBuddies[h][i];
      if (other == -1) continue;
      if (bestBuddies[hh][other] != i) {
        bestBuddies[h][i] = -1;
      }
    }
  }
}

void Database::cutImageIntoPieces() {
  if (originalImage.height % mRowsCount != 0 ||
      originalImage.width % mColumnsCount != 0) {
    std::cout << "ERROR: Invalid size";
    exit(0);
  }

  int piece_height = originalImage.height / mRowsCount;
  int piece_width = originalImage.width / mColumnsCount;

  for (int i = 0; i < mRowsCount; i++)
    for (int j = 0; j < mColumnsCount; j++)
      originalImage.exportToImage(piece[i * mColumnsCount + j], i * piece_height, j * piece_width, (i + 1) * piece_height, (j + 1) * piece_width);
}

void Database::alloc() {
  for (int i = 0; i < 4; i++) {
    compatibility[i] = new double *[mRowsCount * mColumnsCount];
    for (int j = 0; j < mRowsCount * mColumnsCount; j++) {
      compatibility[i][j] = new double [mRowsCount * mColumnsCount];
    }
  }

  for (int i = 0; i < 4; i++)
    bestBuddies[i] = new int [mRowsCount * mColumnsCount];

  piece = new JigsawSolver::Image[mRowsCount * mColumnsCount];
}
