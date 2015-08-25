#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include <map>
#include <algorithm>
#include <cmath>
#include "image/image.hpp"
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

typedef std::map<Position, int> map_pII;

class Segment {
protected:
	Position leftTop, rightBot;
	int nRows, nColumns;

	int table[16 * 2][16 * 2];
	Position posOfPiece[256];

	int piecesCount;

	bool isFixed;

	void updateBoundaries(const Position& newPos) {
		if (isFixed) return;

		leftTop.X = std::min(leftTop.X, newPos.X);
		leftTop.Y = std::min(leftTop.Y, newPos.Y);

		rightBot.X = std::max(rightBot.X, newPos.X);
		rightBot.Y = std::max(rightBot.Y, newPos.Y);
	}

	bool isInside(const Position& newPos) const	{
		if (isFixed) {
			return
				newPos.X >= leftTop.X && newPos.X <= rightBot.X &&
				newPos.Y >= leftTop.Y && newPos.Y <= rightBot.Y;
		}

		if (piecesCount == 0) return true;

		if (abs(newPos.X - leftTop.X) >= nRows ||
			abs(newPos.X - rightBot.X) >= nRows ||
			abs(newPos.Y - leftTop.Y) >= nColumns ||
			abs(newPos.Y - rightBot.Y) >= nColumns) return false;

		return true;
	}

	void swap(int piece1, int piece2)	{
		std::swap(table[posOfPiece[piece1].X][posOfPiece[piece1].Y], table[posOfPiece[piece2].X][posOfPiece[piece2].Y]);
		std::swap(posOfPiece[piece1], posOfPiece[piece2]);
	}

public:
	Segment()	{
		isFixed = false;
	}

	Segment(int _nRows, int _nColumns) {
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

	bool containPiece(int pieceIndex) const	{
		if (pieceIndex < 0 || pieceIndex >= nRows * nColumns) return false;

		if (table[posOfPiece[pieceIndex].X][posOfPiece[pieceIndex].Y] != pieceIndex) return false;
		else return true;
	}

	bool isEmpty(Position pos) const {
		if (!isInside(pos)) return false;

		if (table[pos.X][pos.Y] == -1) return true;
		else return false;
	}

	Position getPosOfPiece(int pieceIndex) const {
		Position result;
		if (!containPiece(pieceIndex)) return result;

		return posOfPiece[pieceIndex];
	}

	void add(Position pos, int pieceIndex) {
		if (containPiece(pieceIndex)) return;
		if (!isInside(pos)) return;
		if (!isEmpty(pos)) return;

		table[pos.X][pos.Y] = pieceIndex;
		posOfPiece[pieceIndex] = pos;

		piecesCount++;

		updateBoundaries(pos);
	}

	void generate()	{
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

	int nextTo(const Position& pos, int direction) const {
		if (!isInside(pos)) return -1;

		Position newPos;
		newPos.X = pos.X + JigsawSolver::hx[direction];
		newPos.Y = pos.Y + JigsawSolver::hy[direction];

		if (!isInside(newPos)) return -1;

		return table[newPos.X][newPos.Y];
	}

	int nextTo(int piece, int direction) const {
		if (!containPiece(piece)) return -1;
		return nextTo(posOfPiece[piece], direction);
	}

	int getPiece(Position pos) {
		if (!isInside(pos)) return -1;
		return table[pos.X][pos.Y];
	}


	unsigned int count() {
		return piecesCount;
	}

	double fitness_value;

	void calculate_fitness_value(const JigsawSolver::Database& data) {
		fitness_value = 0;

		for (int i = leftTop.X; i <= rightBot.X; i++)
		for (int j = leftTop.Y; j <= rightBot.Y; j++)
		{
			if (j != rightBot.Y)
				fitness_value += data.getCompatibility(table[i][j], table[i][j + 1], 1);
			if (i != rightBot.X)
				fitness_value += data.getCompatibility(table[i][j], table[i + 1][j], 3);
			if (j != leftTop.Y)
				fitness_value += data.getCompatibility(table[i][j], table[i][j - 1], 0);
			if (i != leftTop.X)
				fitness_value += data.getCompatibility(table[i][j], table[i - 1][j], 2);
		}
	}

	void exportToImageFile(JigsawSolver::Database& data, const std::string& path) {
		JigsawSolver::PPMImage newImage;
		newImage.resize(data.originalImage.height, data.originalImage.width);

		newImage.maxColor = data.originalImage.maxColor;
		newImage.maxSelections = data.originalImage.maxSelections;
		newImage.nColumns = data.originalImage.nColumns;
		newImage.nRows = data.originalImage.nRows;
		newImage.selectCost = data.originalImage.selectCost;
		newImage.swapCost = data.originalImage.swapCost;

		int piece_height = newImage.height / newImage.nRows;
		int piece_width = newImage.width / newImage.nColumns;

		for (int i = leftTop.X; i <= rightBot.X; i++)
			for (int j = leftTop.Y; j <= rightBot.Y; j++) {
				Position pos;
				pos.X = i - leftTop.X;
				pos.Y = j - leftTop.Y;

				int part = table[i][j];

				for (int p = 0; p < piece_height; p++)
					for (int q = 0; q < piece_width; q++)
						newImage.bitmap[pos.X * piece_height + p][pos.Y * piece_width + q] = data.piece[part].bitmap[p][q];
			}

		newImage.exportTo(path.c_str(), 0, 0, newImage.height, newImage.width);
	}

	void exportToOrderFile(const std::string& path) {
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

	void fix() {
		isFixed = true;
		leftTop = { 1, 1 };
		rightBot = { nRows, nColumns };
	}
};

#endif
