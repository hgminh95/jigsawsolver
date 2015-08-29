#include "geneticsolver.hpp"

#include <iostream>

using namespace JigsawSolver;

bool fitness_cmp(const Segment *x1, const Segment *x2){
  return x1->fitness_value <  x2->fitness_value;
}

GeneticSolver::GeneticSolver() {
  this->mRowsCount = Database::getInstance()->getRowsCount();
  this->mColumnsCount = Database::getInstance()->getColumnsCount();
}

GeneticSolver::~GeneticSolver() {
}

void GeneticSolver::solve() {
  generateChromosomes();

  for (unsigned int i = 0; i < mMaxGeneration; i++) {
    std::cout << "---Generation " << i << std::endl;

    // evaluate all chromosomes using fitness function
    std::cout << "------Evaluating all chromosomes' fitness value... ";
    calculateFitnessValue();
    std::cout << "Done." << std::endl;

    std::sort(crrPopulation.begin(), crrPopulation.end(), fitness_cmp);
    //removeSameChromosome();
    std::cout << "------Best chromosome: " << crrPopulation[0]->fitness_value <<std::endl;

    // copy 4 best chromosomes to new population
    std::cout << "------Creating new generation..." << std::flush;
    newPopulation.clear();

    newPopulation.push_back(crrPopulation[0]);
    newPopulation.push_back(crrPopulation[1]);
    newPopulation.push_back(crrPopulation[2]);
    newPopulation.push_back(crrPopulation[3]);
    std::cout << "...";

    while (newPopulation.size() < mMaxPopulation) {
      int parent1 = rand() % mMaxPopulation;
      int parent2 = rand() % mMaxPopulation;
      if (rand() % 100 < 50) parent1 = rand() % (mMaxPopulation / 7);
      if (rand() % 100 < 30) parent2 = rand() % (mMaxPopulation / 5);

      auto child = crossover(crrPopulation[parent1], crrPopulation[parent2]);
      newPopulation.push_back(child);
    }
    std::cout << "...";

    for (unsigned int i = 4; i < mMaxPopulation; i++)
      delete crrPopulation[i];
    crrPopulation.clear();
    crrPopulation = newPopulation;
    std::cout << "Done" << std::endl;
  }
  std::cout << "Done" << std::endl;

  calculateFitnessValue();
  std::sort(crrPopulation.begin(), crrPopulation.end(), fitness_cmp);

  std::cout << "Exporting to image file...";
  crrPopulation[0]->exportToImageFile(mOutputFile.c_str());
  std::cout << "Done." << std::endl;
}

void GeneticSolver::generateChromosomes() {
  crrPopulation.clear();
  for (unsigned int i = 0; i < mMaxPopulation; i++) {
    auto seg = new Segment(mRowsCount, mColumnsCount);
    seg->generate();
    crrPopulation.push_back(seg);
  }
}

void GeneticSolver::calculateFitnessValue() {
  for (unsigned int i = 0; i < mMaxPopulation; i++)
    crrPopulation[i]->calculateFitnessValue();
}

bool inside(int p, int q, int n, int m) {
  return p >= 0 && q >= 0 && p < n && q < m;
}

void GeneticSolver::addNewPiece(const Segment *father, const Segment *mother, Segment *child, int part, const Position& pos) {
  if (child->containPiece(part)) return;
  if (!child->isEmpty(pos)) return;

  auto data = Database::getInstance();

  child->add(pos, part);

  for (unsigned int i = 0; i <= notSelectedPiece.size(); i++)
  if (notSelectedPiece[i] == part) {
    notSelectedPiece.erase(notSelectedPiece.begin() + i);
    break;
  }

  for (int i = 0; i < 4; i++) {
    Position newPos;
    newPos.X = pos.X + JigsawSolver::hx[i];
    newPos.Y = pos.Y + JigsawSolver::hy[i];

    if (!child->isEmpty(newPos)) continue;

    if (father->nextTo(part, i) == mother->nextTo(part, i) &&
      father->nextTo(part, i) != -1)
    if (!child->containPiece(father->nextTo(part, i)))     {
      vector_1.push_back(std::make_pair(pos, i));
    }

    if (data->isBestBuddy(part, father->nextTo(part, i), i) ||
      data->isBestBuddy(part, mother->nextTo(part, i), i))
      vector_2.push_back(std::make_pair(pos, i));

    vector_3.push_back(std::make_pair(pos, i));
  }
}

Segment *GeneticSolver::crossover(Segment *father, Segment *mother) {
  vector_1.clear();
  vector_2.clear();
  vector_3.clear();

  auto child = new Segment(mRowsCount, mColumnsCount);

  notSelectedPiece.clear();
  for (unsigned int i = 0; i < mRowsCount * mColumnsCount; i++)
    notSelectedPiece.push_back(i);

  addNewPiece(father, mother, child, rand() % (mRowsCount * mColumnsCount), Position(mRowsCount, mColumnsCount));

  while (child->count() < mRowsCount * mColumnsCount) {
    if (vector_1.size() > 0) {
      if ((unsigned int) rand() % 200 > mMutationRate * 2)
        addPieceInFirstList(father, mother, child);
      else
        addPieceInThirdList(father, mother, child);
    }
    else if (vector_2.size() > 0)  {
      if ((unsigned int) rand() % 200 > mMutationRate * 2)
        addPieceInSecondList(father, mother, child);
      else
        addPieceInThirdList(father, mother, child);
    }
    else {
      addPieceInThirdList(father, mother, child);
    }
  }

  return child;
}

void GeneticSolver::addPieceInFirstList(const Segment *father, const Segment *mother, Segment *child) {
  if (vector_1.size() < 1) return;

  int index = rand() % vector_1.size();

  Position pos = vector_1[index].first;
  int direction = vector_1[index].second;
  Position newPos;
  newPos.X = pos.X + JigsawSolver::hx[direction];
  newPos.Y = pos.Y + JigsawSolver::hy[direction];
  int part = father->nextTo(child->getPiece(pos), direction);

  addNewPiece(father, mother, child, part, newPos);

  deleteInList(vector_1, index);
}

void GeneticSolver::addPieceInSecondList(const Segment *father, const Segment *mother, Segment *child) {
  auto data = Database::getInstance();

  if (vector_2.size() < 1) return;

  int index = rand() % vector_2.size();

  Position pos = vector_2[index].first;
  int direction = vector_2[index].second;
  int part = child->getPiece(pos);
  Position newPos;
  newPos.X = pos.X + JigsawSolver::hx[direction];
  newPos.Y = pos.Y + JigsawSolver::hy[direction];

  int newPart = 0;
  if (data->isBestBuddy(part, father->nextTo(part, direction), direction))
    newPart = father->nextTo(part, direction);
  if (data->isBestBuddy(part, mother->nextTo(part, direction), direction))
    newPart = mother->nextTo(part, direction);

  addNewPiece(father, mother, child, newPart, newPos);

  deleteInList(vector_2, index);
}

void GeneticSolver::addPieceInThirdList(const Segment *father, const Segment *mother, Segment *child) {
  auto data = Database::getInstance();

  if (vector_3.size() < 1) return;

  int index = rand() % vector_3.size();

  Position pos = vector_3[index].first;
  int direction = vector_3[index].second;
  int part = child->getPiece(pos);
  Position newPos;
  newPos.X = pos.X += JigsawSolver::hx[direction];
  newPos.Y = pos.Y += JigsawSolver::hy[direction];

  int newPart[256];
  int newPart_size = 1;
  newPart[0] = notSelectedPiece[0];
  for (unsigned int i = 1; i < notSelectedPiece.size(); i++) {
    if (data->getCompatibility(part, newPart[0], direction) > data->getCompatibility(part, notSelectedPiece[i], direction)) {
      newPart_size = 1;
      newPart[0] = notSelectedPiece[i];
    }
    else
    if (std::abs(data->getCompatibility(part, newPart[0], direction) - data->getCompatibility(part, notSelectedPiece[i], direction)) < EPS) {
      newPart[newPart_size] = notSelectedPiece[i];
      newPart_size++;
    }
  }

  addNewPiece(father, mother, child, newPart[rand() % newPart_size], newPos);

  deleteInList(vector_3, index);
}

void GeneticSolver::deleteInList(std::vector< std::pair<Position, int> >& list, int index) {
  std::swap(list[index], list[list.size() - 1]);
  list.erase(list.end() - 1);
}

void GeneticSolver::setMaxGeneration(unsigned int maxGeneration) {
  mMaxGeneration = maxGeneration;
}

void GeneticSolver::setMaxPopulation(unsigned int maxPopulation) {
  mMaxPopulation = maxPopulation;
}

void GeneticSolver::setMutationRate(unsigned int mutationRate) {
  mMutationRate = mutationRate;
}

void GeneticSolver::setMetric(unsigned int metric) {
  mMetric = metric;
}

void GeneticSolver::setOutputFile(std::string outputFile) {
  mOutputFile = outputFile;
}
