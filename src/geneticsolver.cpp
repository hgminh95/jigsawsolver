#include "geneticsolver.hpp"

using namespace JigsawSolver;

bool fitness_cmp(const Segment& x1, const Segment& x2){
	return x1.fitness_value <  x2.fitness_value;
}

GeneticSolver::GeneticSolver() {
}

GeneticSolver::~GeneticSolver() {
}

void GeneticSolver::Solve() {
	std::cout << "Initializing From Database... ";
	importFromDatabase();
	std::cout << "Done" << std::endl;

	std::cout << "Loading fixed point...";
	if (USING_FIXED_POINT) {
		loadFixedPoint();
		std::cout << "Done." << std::endl;
	}
	else std::cout << "Skipped." << std::endl;

	std::cout << "Generating Chromosomes... ";
	generateChromosomes();
	std::cout << "Done" << std::endl;

	std::cout << "Computing... " << std::endl;
	for (unsigned int i = 0; i < MAX_GENERATION_NUMBER; i++) {
		std::cout << "---Generation " << i << std::endl;

		// evaluate all chromosomes using fitness function
		std::cout << "------Evaluating all chromosomes' fitness value... ";
		calculateFitnessValue();
		std::cout << "Done." << std::endl;

		std::sort(crrPopulation.begin(), crrPopulation.end(), fitness_cmp);
		//removeSameChromosome();
		std::cout << "------Best chromosome: " << crrPopulation[0].fitness_value << " " << crrPopulation[MAX_POPULATION - 1].fitness_value <<std::endl;

		// copy 4 best chromosomes to new population
		std::cout << "------Creating new generation...";
		newPopulation.clear();

		newPopulation.push_back(crrPopulation[0]);
		newPopulation.push_back(crrPopulation[1]);
		newPopulation.push_back(crrPopulation[2]);
		newPopulation.push_back(crrPopulation[3]);
		std::cout << "...";

		while (newPopulation.size() < MAX_POPULATION) {
			int parent1 = rand() % MAX_POPULATION;
			int parent2 = rand() % MAX_POPULATION;
			if (rand() % 100 < 50) parent1 = rand() % (MAX_POPULATION / 7);
			if (rand() % 100 < 20) parent2 = rand() % (MAX_POPULATION / 5);

			Segment child = crossover(crrPopulation[parent1], crrPopulation[parent2]);
			newPopulation.push_back(child);
		}
		std::cout << "...";

		crrPopulation.clear();
		crrPopulation = newPopulation;
		std::cout << "Done" << std::endl;
	}
	std::cout << "Done" << std::endl;

	calculateFitnessValue();
	std::sort(crrPopulation.begin(), crrPopulation.end(), fitness_cmp);

	crrPopulation[0].exportToOrderFile(ORDER_FILE);
	std::cout << "Exporting to image file...";
	if (EXPORT_TO_OUTPUT_IMAGE) {
		crrPopulation[0].exportToImageFile(data, NEW_FILE.c_str());
		std::cout << "Done." << std::endl;
	}
	else std::cout << "Skipped" << std::endl;
}

void GeneticSolver::generateChromosomes() {
	crrPopulation.clear();
	for (unsigned int i = 0; i < MAX_POPULATION; i++) {
		Segment tmp_chromosome(nRows, nColumns);
		tmp_chromosome.generate();
		crrPopulation.push_back(tmp_chromosome);
	}
}

void GeneticSolver::calculateFitnessValue() {
	for (unsigned int i = 0; i < MAX_POPULATION; i++)
		crrPopulation[i].calculate_fitness_value(data);
}

void GeneticSolver::removeSameChromosome() {
	std::vector<Segment> temp;
	temp.push_back(crrPopulation[0]);

	for (unsigned int i = 1; i < crrPopulation.size(); i++) {
		if (abs(crrPopulation[i].fitness_value - crrPopulation[i - 1].fitness_value) > EPS)
			temp.push_back(crrPopulation[i]);
	}

	while (temp.size() < MAX_POPULATION) {
		Segment seg(nRows, nColumns);
		seg.generate();

		temp.push_back(seg);
	}

	crrPopulation = temp;
}

bool inside(int p, int q, int n, int m) {
	return p >= 0 && q >= 0 && p < n && q < m;
}

void GeneticSolver::addNewPiece(const Segment& father, const Segment& mother, Segment& child, int part, const Position& pos) {
	if (child.containPiece(part)) return;
	if (!child.isEmpty(pos)) return;

	child.add(pos, part);

	for (unsigned int i = 0; i <= notSelectedPiece.size(); i++)
	if (notSelectedPiece[i] == part) {
		notSelectedPiece.erase(notSelectedPiece.begin() + i);
		break;
	}

	for (int i = 0; i < 4; i++) {
		Position newPos;
		newPos.X = pos.X + JigsawSolver::hx[i];
		newPos.Y = pos.Y + JigsawSolver::hy[i];

		if (!child.isEmpty(newPos)) continue;

		if (father.nextTo(part, i) == mother.nextTo(part, i) &&
			father.nextTo(part, i) != -1)
		if (!child.containPiece(father.nextTo(part, i))) 		{
			vector_1.push_back(std::make_pair(pos, i));
		}

		if (data.isBestBuddy(part, father.nextTo(part, i), i) ||
			data.isBestBuddy(part, mother.nextTo(part, i), i))
			vector_2.push_back(std::make_pair(pos, i));

		vector_3.push_back(std::make_pair(pos, i));
	}
}

Segment GeneticSolver::crossover(Segment& father, Segment& mother) {
	vector_1.clear();
	vector_2.clear();
	vector_3.clear();

	Segment child(nRows, nColumns);

	notSelectedPiece.clear();
	for (unsigned int i = 0; i < nRows; i++)
		for (unsigned int j = 0; j < nColumns; j++)
			notSelectedPiece.push_back(i * nColumns + j);

	if (!USING_FIXED_POINT)
		addNewPiece(father, mother, child, rand() % (nRows * nColumns), Position(nRows, nColumns));
	else {
		child.fix();
		for (unsigned int i = 0; i < fixedPoint.size(); i++)
			addNewPiece(father, mother, child, fixedPoint[i].second, fixedPoint[i].first);
	}

	while (child.count() < nRows * nColumns) {
		if (vector_1.size() > 0) {
			if (rand() % 200 > MUTATION_RATE * 2)
				addPieceInFirstList(father, mother, child);
			else
				addPieceInThirdList(father, mother, child);
		}
		else if (vector_2.size() > 0)	{
			if (rand() % 200 > MUTATION_RATE * 2)
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

void GeneticSolver::addPieceInFirstList(const Segment& father, const Segment& mother, Segment& child) {
	if (vector_1.size() < 1) return;

	int index = rand() % vector_1.size();

	Position pos = vector_1[index].first;
	int direction = vector_1[index].second;
	Position newPos;
	newPos.X = pos.X + JigsawSolver::hx[direction];
	newPos.Y = pos.Y + JigsawSolver::hy[direction];
	int part = father.nextTo(child.getPiece(pos), direction);

	addNewPiece(father, mother, child, part, newPos);

	deleteInList(vector_1, index);
}

void GeneticSolver::addPieceInSecondList(const Segment& father, const Segment& mother, Segment& child) {
	if (vector_2.size() < 1) return;

	int index = rand() % vector_2.size();

	Position pos = vector_2[index].first;
	int direction = vector_2[index].second;
	int part = child.getPiece(pos);
	Position newPos;
	newPos.X = pos.X + JigsawSolver::hx[direction];
	newPos.Y = pos.Y + JigsawSolver::hy[direction];

	int newPart = 0;
	if (data.isBestBuddy(part, father.nextTo(part, direction), direction))
		newPart = father.nextTo(part, direction);
	if (data.isBestBuddy(part, mother.nextTo(part, direction), direction))
		newPart = mother.nextTo(part, direction);

	addNewPiece(father, mother, child, newPart, newPos);

	deleteInList(vector_2, index);
}

void GeneticSolver::addPieceInThirdList(const Segment& father, const Segment& mother, Segment& child) {
	if (vector_3.size() < 1) return;

	int index = rand() % vector_3.size();

	Position pos = vector_3[index].first;
	int direction = vector_3[index].second;
	int part = child.getPiece(pos);
	Position newPos;
	newPos.X = pos.X += JigsawSolver::hx[direction];
	newPos.Y = pos.Y += JigsawSolver::hy[direction];

	int newPart[256];
	int newPart_size = 1;
	newPart[0] = notSelectedPiece[0];
	for (unsigned int i = 1; i < notSelectedPiece.size(); i++) {
		if (data.getCompatibility(part, newPart[0], direction) > data.getCompatibility(part, notSelectedPiece[i], direction)) {
			newPart_size = 1;
			newPart[0] = notSelectedPiece[i];
		}
		else
		if (std::abs(data.getCompatibility(part, newPart[0], direction) - data.getCompatibility(part, notSelectedPiece[i], direction)) < EPS) {
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


void GeneticSolver::importFromDatabase() {
	data.selectedMetric = COMP_TYPE;

	if (USING_DATABASE)
		data.importFromDatabaseFile(DATABASE_FILE);
	else {
		data.importFromImageFile(ORIGINAL_FILE);
		data.exportToFile(DATABASE_FILE);
	}

	nRows = data.nRows;
	nColumns = data.nColumns;
}


void GeneticSolver::loadFixedPoint() {
	std::ifstream input(FIXED_POINT_FILE);

	fixedPoint.clear();

	int x, y, z;
	input >> x >> y >> z;
	while (input.good()) {
		fixedPoint.push_back({ Position(x, y), z });
		input >> x >> y >> z;
	}

	input.close();
}


void GeneticSolver::customizeConstant(int max_generation, int max_chromosome, int mutation_rate) {
	MAX_GENERATION_NUMBER = max_generation;
	MAX_POPULATION = max_chromosome;
	MUTATION_RATE = mutation_rate;
}

void GeneticSolver::customizeIO(std::string original_file, std::string new_file, std::string order_file) {
	ORIGINAL_FILE = original_file;
	NEW_FILE = new_file;
	ORDER_FILE = order_file;
}

void GeneticSolver::customizeMetric(int compType, int randType) {
	COMP_TYPE = compType;
	RAND_TYPE = randType;
}

void GeneticSolver::customizeFixedPoint(bool using_fixed_point, std::string fixed_point_file) {
	USING_FIXED_POINT = using_fixed_point;
	FIXED_POINT_FILE = fixed_point_file;
}

void GeneticSolver::customizeDatabase(bool using_database, std::string database_file) {
	USING_DATABASE = using_database;
	DATABASE_FILE = database_file;
}
