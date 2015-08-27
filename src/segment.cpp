#include "segment.hpp"

#include "image/image.hpp"
#include "database.hpp"

using namespace JigsawSolver;

Segment::Segment()	{
  isFixed = false;
}

Segment::Segment(int _nRows, int _nColumns) {
  nRows = _nRows;
  nColumns = _nColumns;

  for (int i = 0; i < nRows * 2; i++)
    for (int j = 0; j < nColumns * 2; j++)
      table[i][j] = -1;

  piecesCount = 0;
  isFixed = false;
  leftTop = Position(10000, 1000);
  rightBot = Position(-1, -1);
}

bool Segment::containPiece(int pieceIndex) const	{
  if (pieceIndex < 0 || pieceIndex >= nRows * nColumns) return false;

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

  piecesCount++;

  updateBoundaries(pos);
}

void Segment::generate()	{
  leftTop = Position(nRows, nColumns);
  rightBot = Position(2 * nRows - 1, 2 * nColumns - 1);
  piecesCount = nRows * nColumns;

  for (int i = 0; i < nRows; i++)
    for (int j = 0; j < nColumns; j++) {
      table[nRows + i][nColumns + j] = i * nColumns + j;
      posOfPiece[i * nColumns + j] = Position(nRows + i, nColumns + j);
    }

  for (int i = 0; i < nRows * nColumns; i++) {
    int piece1 = rand() % (nRows * nColumns);
    int piece2 = rand() % (nRows * nColumns);

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
  return piecesCount;
}

void Segment::calculateFitnessValue() {
  fitness_value = 0;

  auto data = Database::getInstance();

  for (int i = leftTop.X; i <= rightBot.X; i++)
    for (int j = leftTop.Y; j <= rightBot.Y; j++) {
      if (j != rightBot.Y)
        fitness_value += data->getCompatibility(table[i][j], table[i][j + 1], 1);
      if (i != rightBot.X)
        fitness_value += data->getCompatibility(table[i][j], table[i + 1][j], 3);
      if (j != leftTop.Y)
        fitness_value += data->getCompatibility(table[i][j], table[i][j - 1], 0);
      if (i != leftTop.X)
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

  for (int i = leftTop.X; i <= rightBot.X; i++)
    for (int j = leftTop.Y; j <= rightBot.Y; j++) {
      Position pos;
      pos.X = i - leftTop.X;
      pos.Y = j - leftTop.Y;

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

  orderFile << nRows << " " << nColumns << "\n";
  for (int i = leftTop.X; i <= rightBot.X; i++) {
    for (int j = leftTop.Y; j <= rightBot.Y; j++)
      orderFile << table[i][j] << " ";
    orderFile << "\n";
  }

  orderFile.close();
}

void Segment::fix() {
  isFixed = true;
  leftTop = { 1, 1 };
  rightBot = { nRows, nColumns };
}

// Private function
void Segment::updateBoundaries(const Position& newPos) {
  if (isFixed) return;

  leftTop.X = std::min(leftTop.X, newPos.X);
  leftTop.Y = std::min(leftTop.Y, newPos.Y);

  rightBot.X = std::max(rightBot.X, newPos.X);
  rightBot.Y = std::max(rightBot.Y, newPos.Y);
}

bool Segment::isInside(const Position& newPos) const {
  if (isFixed) {
    return
      newPos.X >= leftTop.X && newPos.X <= rightBot.X &&
      newPos.Y >= leftTop.Y && newPos.Y <= rightBot.Y;
  }

  if (piecesCount == 0) return true;

  if (std::abs(newPos.X - leftTop.X) >= nRows ||
    std::abs(newPos.X - rightBot.X) >= nRows ||
    std::abs(newPos.Y - leftTop.Y) >= nColumns ||
    std::abs(newPos.Y - rightBot.Y) >= nColumns) return false;

  return true;
}

void Segment::swap(int piece1, int piece2)	{
  std::swap(table[posOfPiece[piece1].X][posOfPiece[piece1].Y], table[posOfPiece[piece2].X][posOfPiece[piece2].Y]);
  std::swap(posOfPiece[piece1], posOfPiece[piece2]);
}
