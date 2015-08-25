#ifndef __GENETIC_SOLVER_H__
#define __GENETIC_SOLVER_H__

#include "jigsawsolver.hpp"
#include <vector>
#include <algorithm>
#include "segment.hpp"
#include <iostream>

class GeneticSolver : public JigsawSolver::Solver {
private:
	void generateChromosomes();
	void calculateFitnessValue();
	void removeSameChromosome();

	void loadFixedPoint();

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

	void importFromDatabase();
public:
	GeneticSolver();
	~GeneticSolver();

	void Solve();

	void customizeConstant(int max_generation, int max_chromosome, int mutation_rate);
	void customizeMetric(int compType, int randType);
	void customizeIO(std::string original_file, std::string new_file, std::string order_file);
	void customizeFixedPoint(bool using_fixed_point, std::string fixed_point_file);
	void customizeDatabase(bool using_database, std::string database_file);
};

#endif
