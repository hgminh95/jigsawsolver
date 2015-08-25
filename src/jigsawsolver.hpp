#ifndef JIGSAW_SOLVER_H
#define JIGSAW_SOLVER_H

#include <string>
#include "database.hpp"

namespace JigsawSolver {
	class Solver {
	public:
		unsigned int nRows, nColumns;

		bool USING_FIXED_POINT;
		bool USING_DATABASE;

		bool EXPORT_TO_OUTPUT_IMAGE;

		unsigned int MAX_GENERATION_NUMBER;
		unsigned int MAX_POPULATION;
		unsigned int MUTATION_RATE;

		std::string ORIGINAL_FILE;
		std::string ORDER_FILE;
		std::string NEW_FILE;
		std::string FIXED_POINT_FILE;
		std::string DATABASE_FILE;

		unsigned int RAND_TYPE;
		unsigned int COMP_TYPE;

		Database data;

		Solver();
		~Solver();
	};
}

#endif
