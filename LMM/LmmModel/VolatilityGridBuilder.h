#pragma once

#include <vector>
#include <cstring>

typedef std::vector<std::vector<double>> matrix_;


class Pillar
{
private:
	std::pair<size_t,size_t> coordinates_;
	double value_;

public:
	Pillar();
	Pillar(size_t i, size_t j, double value);
	static std::vector<Pillar> findPillarsByRow(size_t row, const std::vector<Pillar>& pillars);
	static std::vector<Pillar> findPillarsByColumn(size_t col, const std::vector<Pillar>& pillars);
	void print();

	//-- getters
	size_t getX();
	size_t getY();
	double getValue();

	//-- TEST
	static void test_pillar();
	static void test_findPillarsWithSameRow();
	static void test_findPillarsWithSameColumn();
};


class VolatilityGridBuilder
{
private:
	size_t dim_;                        //-- grid's dimension
	matrix_ grid_;                      //-- the grid to build
	size_t jumpsize_;                   //-- jumps between each pillar
	std::vector<Pillar> pillars_;

	void createPillars(const std::vector<double>& pillarValues);
	void buildGrid();
	void interpolateAllRows();
	void interpolateAllColumns();
	void bilinearInterpolations(); // Find values between four pillars
	void borderExtrapolations();
	void fillAntidiagonal();

public :
	VolatilityGridBuilder(size_t dim, size_t jumpsize, const std::vector<double>& pillarValues);
	void printGrid(std::string fileName);

	//-- Getters
	matrix_& get_grid();
	std::vector<Pillar>& get_pillars();

	//-- TESTS
	static void test_insertPillarsInGrid();
	static void test_buildFullGrid();
};
