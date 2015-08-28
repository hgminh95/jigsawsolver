#include "segment.hpp"

#include "image/image.hpp"
#include "database.hpp"

using namespace JigsawSolver;

Segment::Segment(int rowsCount, int columnsCount) {
  mRowsCount = rowsCount;
  mColumnsCount = columnsCount;

  table = new int *[mRowsCount * 2];
  for (int i = 0; i < mRowsCount * 2; i++) {
    table[i] = new int[mColumnsCount * 2];
    for (int j = 0; j < mColumnsCount * 2; j++)
      table[i][j] = -1;
  }

  posOfPiece = new Position[mRowsCount * mColumnsCount];

  mPiecesCount = 0;
  isFixed = false;
  mLeftTop = Position(10000, 1000);
  mRightBot = Position(-1, -1);
}

Segment::~Segment() {
  for (int i = 0; i < mRowsCount * 2; i++)
    delete [] table[i];
  delete [] table;

  delete [] posOfPiece;
}

bool Segment::containPiece(int pieceIndex) const {
  if (pieceIndex < 0 || pieceIndex >= mRowsCount * mColumnsCount) return false;

  if (table[posOfPiece[pieceIndex].X][posOfPiece[pieceIndex].Y] != pieceIndex) return false;
  else return true;
}

bool Segment::isEmpty(Position pos) const {
  if (!isInside(pos)) return false;

  if (table[pos.X][pos.Y] == -1) return true;
  else return false;
}

Position Segment::getPosOfPiece(int pieceIndex) const {
  Position result;
  if (!containPiece(pieceIndex)) return result;

  return posOfPiece[pieceIndex];
}

void Segment::add(Position pos, int pieceIndex) {
  if (containPiece(pieceIndex)) return;
  if (!isInside(pos)) return;
  if (!isEmpty(pos)) return;

  table[pos.X][pos.Y] = pieceIndex;
  posOfPiece[pieceIndex] = pos;

  mPiecesCount++;

  updateBoundaries(pos);
}

void Segment::generate() {
  mLeftTop = Position(mRowsCount, mColumnsCount);
  mRightBot = Position(2 * mRowsCount - 1, 2 * mColumnsCount - 1);
  mPiecesCount = mRowsCount * mColumnsCount;

  for (int i = 0; i < mRowsCount; i++)
    for (int j = 0; j < mColumnsCount; j++) {
      table[mRowsCount + i][mColumnsCount + j] = i * mColumnsCount + j;
      posOfPiece[i * mColumnsCount + j] = Position(mRowsCount + i, mColumnsCount + j);
    }

  for (int i = 0; i < mRowsCount * mColumnsCount; i++) {
    int piece1 = rand() % (mRowsCount * mColumnsCount);
    int piece2 = rand() % (mRowsCount * mColumnsCount);

    swap(piece1, piece2);
  }
}

int Segment::nextTo(const Position& pos, int direction) const {
  if (!isInside(pos)) return -1;

  Position newPos;
  newPos.X = pos.X + JigsawSolver::hx[direction];
  newPos.Y = pos.Y + JigsawSolver::hy[direction];

  if (!isInside(newPos)) return -1;

  return table[newPos.X][newPos.Y];
}

int Segment::nextTo(int piece, int direction) const {
  if (!containPiece(piece)) return -1;
  return nextTo(posOfPiece[piece], direction);
}

int Segment::getPiece(Position pos) const {
  if (!isInside(pos)) return -1;
  return table[pos.X][pos.Y];
}

unsigned int Segment::count() const {
  return mPiecesCount;
}

void Segment::calculateFitnessValue() {
  fitness_value = 0;

  auto data = Database::getInstance();

  for (int i = mLeftTop.X; i <= mRightBot.X; i++)
    for (int j = mLeftTop.Y; j <= mRightBot.Y; j++) {
      if (j != mRightBot.Y)
        fitness_value += data->getCompatibility(table[i][j], table[i][j + 1], 1);
      if (i != mRightBot.X)
        fitness_value += data->getCompatibility(table[i][j], table[i + 1][j], 3);
      if (j != mLeftTop.Y)
        fitness_value += data->getCompatibility(table[i][j], table[i][j - 1], 0);
      if (i != mLeftTop.X)
        fitness_value += data->getCompatibility(table[i][j], table[i - 1][j], 2);
    }
}

void Segment::exportToImageFile(const std::string& path) const {
  JigsawSolver::PPMImage newImage;

  auto data = Database::getInstance();

  newImage.resize(data->originalImage.height, data->originalImage.width);

  newImage.maxColor = data->originalImage.maxColor;

  int piece_height = newImage.height / data->getRowsCount();
  int piece_width = newImage.width / data->getColumnsCount();

  for (int i = mLeftTop.X; i <= mRightBot.X; i++)
    for (int j = mLeftTop.Y; j <= mRightBot.Y; j++) {
      Position pos;
      pos.X = i - mLeftTop.X;
      pos.Y = j - mLeftTop.Y;

      int part = table[i][j];

      for (int p = 0; p < piece_height; p++)
        for (int q = 0; q < piece_width; q++)
          newImage.bitmap[pos.X * piece_height + p][pos.Y * piece_width + q] = data->piece[part].bitmap[p][q];
    }

    newImage.exportTo(path.c_str(), 0, 0, newImage.height, newImage.width);
}

void Segment::exportToOrderFile(const std::string& path) const {
  std::ofstream orderFile(path.c_str());

  if (!orderFile.good()) return;

  orderFile << mRowsCount << " " << mColumnsCount << "\n";
  for (int i = mLeftTop.X; i <= mRightBot.X; i++) {
    for (int j = mLeftTop.Y; j <= mRightBot.Y; j++)
      orderFile << table[i][j] << " ";
    orderFile << "\n";
  }

  orderFile.close();
}

void Segment::fix() {
  isFixed = true;
  mLeftTop = { 1, 1 };
  mRightBot = { mRowsCount, mColumnsCount };
}

// Private function
void Segment::updateBoundaries(const Position& newPos) {
  if (isFixed) return;

  mLeftTop.X = std::min(mLeftTop.X, newPos.X);
  mLeftTop.Y = std::min(mLeftTop.Y, newPos.Y);

  mRightBot.X = std::max(mRightBot.X, newPos.X);
  mRightBot.Y = std::max(mRightBot.Y, newPos.Y);
}

bool Segment::isInside(const Position& newPos) const {
  if (isFixed) {
    return
      newPos.X >= mLeftTop.X && newPos.X <= mRightBot.X &&
      newPos.Y >= mLeftTop.Y && newPos.Y <= mRightBot.Y;
  }

  if (mPiecesCount == 0) return true;

  if (std::abs(newPos.X - mLeftTop.X) >= mRowsCount ||
    std::abs(newPos.X - mRightBot.X) >= mRowsCount ||
    std::abs(newPos.Y - mLeftTop.Y) >= mColumnsCount ||
    std::abs(newPos.Y - mRightBot.Y) >= mColumnsCount) return false;

  return true;
}

void Segment::swap(int piece1, int piece2)  {
  std::swap(table[posOfPiece[piece1].X][posOfPiece[piece1].Y], table[posOfPiece[piece2].X][posOfPiece[piece2].Y]);
  std::swap(posOfPiece[piece1], posOfPiece[piece2]);
}
