#ifndef __GENETIC_SOLVER_H__
#define __GENETIC_SOLVER_H__

#include <vector>
#include <algorithm>
#include "segment.hpp"
#include <iostream>

class GeneticSolver {
private:
	void generateChromosomes();
	void calculateFitnessValue();

	std::vector< std::pair<Position, int> > fixedPoint;

	std::vector<Segment> crrPopulation, newPopulation;

	std::vector< std::pair<Position, int> > vector_1, vector_2, vector_3;
	std::vector<int> notSelectedPiece;
	Segment child;

	void addNewPiece(const Segment& father, const Segment& mother, Segment& child, int part, const Position& pos);
	Segment crossover(Segment& father, Segment& mother);
	void addPieceInFirstList(const Segment& father, const Segment& mother, Segment& child);
	void addPieceInSecondList(const Segment& father, const Segment& mother, Segment& child);
	void addPieceInThirdList(const Segment& father, const Segment& mother, Segment& child);
	void deleteInList(std::vector< std::pair<Position, int> >& list, int index);

	unsigned int mMaxGeneration;
	unsigned int mMaxPopulation;
	unsigned int mMutationRate;
	unsigned int mMetric;
	unsigned int nRows;
	unsigned int nColumns;
	std::string mOutputFile;
public:
	GeneticSolver();
	~GeneticSolver();

	void solve();

	void setMaxGeneration(unsigned int maxGeneration);
	void setMaxPopulation(unsigned int maxPopulation);
	void setMutationRate(unsigned int mutationRate);
	void setMetric(unsigned int metric);
	void setOutputFile(std::string outputFile);
};

#endif
