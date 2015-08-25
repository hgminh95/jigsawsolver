#include "geneticsolver.hpp"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>

GeneticSolver geneticSolver;

/* const */
std::string ORIGINAL_FILE;
std::string OUTPUT_FILE;
std::string ORDER_FILE;
std::string FIXED_POINT_FILE;
std::string DATABASE_FILE;

int COMP_METRIC;
int RAND_TYPE;

int MAX_GENERATION;
int MAX_POPULATION;
int MUTATION_RATE;

bool USING_FIXED_POINT;
bool USING_DATABASE;
bool EXPORT_TO_OUTPUT_FILE;

void initDefaultValue()
{
	ORIGINAL_FILE = "D:\\prob01.ppm";
	OUTPUT_FILE = "D:\\prob01.ppm.result";
	ORDER_FILE = "D:\\prob01.ppm.order";
	FIXED_POINT_FILE = "D:\\prob01.ppm.fixed";
	COMP_METRIC = 1;

	MAX_GENERATION = 50;
	MAX_POPULATION = 400;
	MUTATION_RATE = 5;

	USING_FIXED_POINT = false;
	USING_DATABASE = false;

	EXPORT_TO_OUTPUT_FILE = true;
}


int main(int argc, char* arg[])
{
	initDefaultValue();
	std::ios_base::sync_with_stdio(false);

	for (int i = 1; i < argc; i++)
	{
		char* identity;
		char* value;

		identity = strtok(arg[i], "=");
		value = strtok(NULL, "=");

		if (strcmp(identity, "original") == 0)
			ORIGINAL_FILE = value;

		if (strcmp(identity, "output") == 0)
			OUTPUT_FILE = value;

		if (strcmp(identity, "order") == 0)
			ORDER_FILE = value;

		if (strcmp(identity, "max_generation") == 0)
			MAX_GENERATION = atoi(value);

		if (strcmp(identity, "max_population") == 0)
			MAX_POPULATION = atoi(value);

		if (strcmp(identity, "mutation_rate") == 0)
			MUTATION_RATE = atoi(value);

		if (strcmp(identity, "using_database") == 0)
		if (strcmp(value, "true") == 0)
		{
			USING_DATABASE = true;
			EXPORT_TO_OUTPUT_FILE = false;
		}
		else USING_DATABASE = false;

		if (strcmp(identity, "database") == 0)
			DATABASE_FILE = value;

		if (strcmp(identity, "rand_type") == 0)
			RAND_TYPE = atoi(value);

		if (strcmp(identity, "comp_metric") == 0)
			COMP_METRIC = atoi(value);

		if (strcmp(identity, "using_fixedpoint") == 0)
			if (strcmp(value, "true") == 0) USING_FIXED_POINT = true;
			else USING_FIXED_POINT = false;

		if (strcmp(identity, "fixedpoint") == 0)
			FIXED_POINT_FILE = value;
	}

	geneticSolver.customizeConstant(MAX_GENERATION, MAX_POPULATION, MUTATION_RATE);
	geneticSolver.customizeIO(ORIGINAL_FILE, OUTPUT_FILE, ORDER_FILE);
	geneticSolver.customizeMetric(COMP_METRIC, RAND_TYPE);
	geneticSolver.customizeDatabase(USING_DATABASE, DATABASE_FILE);
	geneticSolver.customizeFixedPoint(USING_FIXED_POINT, FIXED_POINT_FILE);
	geneticSolver.EXPORT_TO_OUTPUT_IMAGE = EXPORT_TO_OUTPUT_FILE;

	geneticSolver.Solve();
	return 0;
}
