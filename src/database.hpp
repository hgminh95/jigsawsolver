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
    double **compatibility[4];
    int *bestBuddies[4];

    int mRowsCount, mColumnsCount;

    static bool instanceFlag;
    static Database *database;
    Database();

    void alloc();

  public:
    ~Database();

    static Database *getInstance();

    JigsawSolver::PPMImage originalImage;
    JigsawSolver::Image *piece;
    int selectedMetric;

    int getRowsCount();
    int getColumnsCount();

    bool containPiece(int pieceIndex) const;
    void cutImageIntoPieces();
    void calculateBestBuddies();

    void importFromImageFile(std::string path, int rowsCount, int columnsCount);

    double getCompatibility(int piece1, int piece2, int direction) const;

    bool isBestBuddy(int piece1, int piece2, int direction) const;
    int getBestBuddyOf(int piece, int direction) const;

   void requestCalculateMetric(const std::string& type);
  };
}

#endif
