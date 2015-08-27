#include "geneticsolver.hpp"
#include "database.hpp"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>

using namespace JigsawSolver;

GeneticSolver geneticSolver;

std::string kOutputFile;
std::string kInputFile;
std::string kMetric;
std::string kMode;

int kMaxGeneration = -1;
int kMaxPopulation = -1;
int kMutationRate  = -1;

void processCommandlineArguments(int argc, char *argv[]);
void printError(std::string errorMessage);

int main(int argc, char* argv[]) {
	processCommandlineArguments(argc, argv);
	std::ios_base::sync_with_stdio(false);

	if (kInputFile.length() == 0)
		printError("You must specific input file");
	std::cout << "Loading image from file " << kInputFile << std::endl;
	Database::getInstance()->importFromImageFile(kInputFile);

	auto geneticSolver = new GeneticSolver();

	if (kOutputFile.length() == 0)
		printError("You must specific output file");
	geneticSolver->setOutputFile(kOutputFile);

	if (kMode.length() != 0) {
		if (kMode == "fast") {
			std::cout << "Mode: fast (population: 50, generation: 20)" << std::endl;
			kMaxGeneration = 20;
			kMaxPopulation = 50;
		}
		else {
			printError("Unknown mode");
		}
	}

	if (kMaxGeneration == -1) {
		std::cout << "Not found max generation. Use default value..." << std::endl;
		kMaxGeneration = 20;
	}
	geneticSolver->setMaxGeneration(kMaxGeneration);

	if (kMaxPopulation == -1) {
		std::cout << "Not found max population. Use default value..." << std::endl;
		kMaxPopulation = 50;
	}
	geneticSolver->setMaxPopulation(kMaxPopulation);

	if (kMutationRate == -1) {
		std::cout << "Not found mutation rate. Use default value..." << std::endl;
		kMutationRate = 5;
	}
	geneticSolver->setMutationRate(kMutationRate);

	geneticSolver->solve();
	return 0;
}

void processCommandlineArguments(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {	/* Process optional arguments */
			if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
				if (i + 1 < argc) {
					i++;
					kOutputFile = argv[i];
				}
				else {
					printError("Expected output file after -o");
				}
			}

			if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--metric") == 0) {
				if (i + 1 < argc) {
					i++;
					kMetric = argv[i];
				}
				else {
					printError("Expected metric type after -o");
				}
			}

			if (strcmp(argv[i], "--mode") == 0) {
				if (i + 1 < argc) {
					i++;
					kMode = argv[i];
				}
				else {
					printError("Expected mode after --mode");
				}
			}

			if (strcmp(argv[i], "--generation") == 0) {
				if (i + 1 < argc) {
					i++;
					kMaxGeneration = atoi(argv[i]);
				}
				else {
					printError("Expected number generation after --generation");
				}
			}

			if (strcmp(argv[i], "--population") == 0) {
				if (i + 1 < argc) {
					i++;
					kMaxPopulation = atoi(argv[i]);
				}
				else {
					printError("Expected population after --population");
				}
			}
		}
		else {	/* Process non-optional arguments */
			kInputFile = argv[i];
		}
	}
}

void printError(std::string errorMessage) {
	std::cout << "ERROR: " << errorMessage << std::endl;
	exit(0);
}
