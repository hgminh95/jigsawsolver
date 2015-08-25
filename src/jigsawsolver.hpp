#ifndef JIGSAW_SOLVER_H
#define JIGSAW_SOLVER_H

#include <string>
#include "database.hpp"

class JigsawSolver
{
public:
	int nRows, nColumns;

	bool USING_FIXED_POINT;
	bool USING_DATABASE;

	bool EXPORT_TO_OUTPUT_IMAGE;

	int MAX_GENERATION_NUMBER;
	int MAX_POPULATION;
	int MUTATION_RATE;

	std::string ORIGINAL_FILE;
	std::string ORDER_FILE;
	std::string NEW_FILE;
	std::string FIXED_POINT_FILE;
	std::string DATABASE_FILE;

	int RAND_TYPE;
	int COMP_TYPE;

	Database data;

	JigsawSolver();
	~JigsawSolver();
};

#endif