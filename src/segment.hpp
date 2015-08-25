#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include <string>
#include "database.hpp"

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
	Position leftTop, rightBot;
	int nRows, nColumns;

	int table[16 * 2][16 * 2];
	Position posOfPiece[256];

	int piecesCount;

	bool isFixed;

	void updateBoundaries(const Position& newPos);
	bool isInside(const Position& newPos) const;
	void swap(int piece1, int piece2);
public:
	double fitness_value;

	Segment();
	Segment(int _nRows, int _nColumns);

	bool containPiece(int pieceIndex) const;
	bool isEmpty(Position pos) const;
	Position getPosOfPiece(int pieceIndex) const;
	void add(Position pos, int pieceIndex);
	void generate();
	int nextTo(const Position& pos, int direction) const;
	int nextTo(int piece, int direction) const;
	int getPiece(Position pos) const;
	unsigned int count() const;
	void calculateFitnessValue(const JigsawSolver::Database& data);

	void exportToImageFile(JigsawSolver::Database& data, const std::string& path) const;
	void exportToOrderFile(const std::string &path) const;

	void fix();
};

#endif
