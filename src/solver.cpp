#include "geneticsolver.hpp"
#include "database.hpp"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <iomanip>
#include <chrono>

using namespace JigsawSolver;
using namespace std::chrono;

high_resolution_clock::time_point startTime, endTime;

GeneticSolver geneticSolver;

std::string kOutputFile;
std::string kInputFile;
std::string kMetric;
std::string kMode;

int kMaxGeneration = -1;
int kMaxPopulation = -1;
int kMutationRate  = -1;
int kSizeRowsCount = -1;
int kSizeColumnsCount = -1;

void processCommandlineArguments(int argc, char *argv[]);
void printError(std::string errorMessage);
void printUsage();

int main(int argc, char* argv[]) {
  processCommandlineArguments(argc, argv);
  std::ios_base::sync_with_stdio(false);

  if (kInputFile.length() == 0)
    printError("You must specific input file.");

  if (kSizeRowsCount == -1 || kSizeColumnsCount == -1) {
    std::cout << "You must specific size of image file:" << std::endl;
    std::cout << "---Number of rows: ";
    std::cin >> kSizeRowsCount;
    std::cout << "---Number of columns: ";
    std::cin >> kSizeColumnsCount;
  }

  if (kOutputFile.length() == 0)
    printError("You must specific output file.");

  if (kMetric.length() == 0) {
    std::cout << "Not found metric. Use default value (metric = LPQ)." << std::endl;
    kMetric = "LPQ";
  }

  if (kMode.length() != 0) {
    if (kMode == "fast") {
      std::cout << "Mode: fast (population: 50, generation: 20)." << std::endl;
      kMaxGeneration = 20;
      kMaxPopulation = 50;
    }
    else {
      printError("Unknown mode");
    }
  }

  if (kMaxGeneration == -1) {
    std::cout << "Not found max generation. Use default value (max generation = 20)." << std::endl;
    kMaxGeneration = 20;
  }

  if (kMaxPopulation == -1) {
    std::cout << "Not found max population. Use default value (max population = 50)." << std::endl;
    kMaxPopulation = 50;
  }

  if (kMutationRate == -1) {
    std::cout << "Not found mutation rate. Use default value (mutation rate = 5\%)." << std::endl;
    kMutationRate = 5;
  }

  startTime = high_resolution_clock::now();
  Database::getInstance()->importFromImageFile(kInputFile, kSizeRowsCount, kSizeColumnsCount);
  Database::getInstance()->requestCalculateMetric(kMetric);

  auto geneticSolver = new GeneticSolver();
  geneticSolver->setMaxGeneration(kMaxGeneration);
  geneticSolver->setMaxPopulation(kMaxPopulation);
  geneticSolver->setMutationRate(kMutationRate);
  geneticSolver->setOutputFile(kOutputFile);
  geneticSolver->solve();

  endTime = high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>> (endTime - startTime);
  std::cout << "Execution time: " << time_span.count() << "s." << std::endl;
  return 0;
}

void processCommandlineArguments(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {  /* Process optional arguments */
      if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
        if (i + 1 < argc) {
          i++;
          kOutputFile = argv[i];
        }
        else {
          printError("Expected output file after --output.");
        }
      }

      if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--metric") == 0) {
        if (i + 1 < argc) {
          i++;
          kMetric = argv[i];
        }
        else {
          printError("Expected metric type after --metric.");
        }
      }

      if (strcmp(argv[i], "--mode") == 0) {
        if (i + 1 < argc) {
          i++;
          kMode = argv[i];
        }
        else {
          printError("Expected mode after --mode.");
        }
      }

      if (strcmp(argv[i], "--generation") == 0) {
        if (i + 1 < argc) {
          i++;
          kMaxGeneration = atoi(argv[i]);
        }
        else {
          printError("Expected number generation after --generation.");
        }
      }

      if (strcmp(argv[i], "--population") == 0) {
        if (i + 1 < argc) {
          i++;
          kMaxPopulation = atoi(argv[i]);
        }
        else {
          printError("Expected population after --population.");
        }
      }

      if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--size") == 0) {
        if (i + 2 < argc) {
          kSizeRowsCount = atoi(argv[i + 1]);
          kSizeColumnsCount = atoi(argv[i + 2]);
          i += 2;
        }
        else {
          printError("Expected {rows} {columns} after --size.");
        }
      }
    }
    else {  /* Process non-optional arguments */
      kInputFile = argv[i];
    }
  }
}

void printError(std::string errorMessage) {
  std::cout << "ERROR: " << errorMessage << std::endl;
  printUsage();
  exit(0);
}

void printUsage() {
  std::cout << "Usage:\n";
  std::cout << "  jigsawsolver INPUT_FILE [options]\n";
  std::cout << "\nOptions: \n";
  std::cout.setf(std::cout.left);
  std::cout << " " << std::setw(30) << "-o OUTPUT, --output OUTPUT" << std::setw(0) << "specific output file's path\n";
  std::cout << " " << std::setw(30) << "-m METRIC, --metric METRIC" << std::setw(0) << "specific metric type: lpq, prediction\n";
  std::cout << " " << std::setw(30) << "--mode MODE" << std::setw(0) << "specific mode: fast, slow\n";
  std::cout << " " << std::setw(30) << "--generation N" << std::setw(0) << "specific number of generation (N)\n";
  std::cout << " " << std::setw(30) << "--population S" << std::setw(0) << "specific population's size (S)\n";
  std::cout << " " << std::setw(30) << "-s R C, --size R C" << std::setw(0) << "specific number of rows (R) and columns (C)\n";
}
