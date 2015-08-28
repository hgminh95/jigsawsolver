#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include <string>
#include "database.hpp"
#include <iostream>

struct Position {
  int X, Y;

  Position() {
    X = Y = 0;
  };

  Position(int _X, int _Y) {
    X = _X;
    Y = _Y;
  }

  bool operator==(const Position& pos) {
    return X == pos.X && Y == pos.Y;
  }
};

class Segment {
protected:
  Position mLeftTop, mRightBot;
  int mRowsCount, mColumnsCount;

  int **table;
  Position *posOfPiece;

  int mPiecesCount;

  bool isFixed;

  void updateBoundaries(const Position& newPos);
  bool isInside(const Position& newPos) const;
  void swap(int piece1, int piece2);

public:
  double fitness_value;

  Segment(int rowsCount, int columnsCount);

  ~Segment();

  bool containPiece(int pieceIndex) const;
  bool isEmpty(Position pos) const;
  Position getPosOfPiece(int pieceIndex) const;
  void add(Position pos, int pieceIndex);
  void generate();
  int nextTo(const Position& pos, int direction) const;
  int nextTo(int piece, int direction) const;
  int getPiece(Position pos) const;
  unsigned int count() const;
  void calculateFitnessValue();

  void exportToImageFile(const std::string& path) const;
  void exportToOrderFile(const std::string &path) const;

  void fix();
};

#endif
