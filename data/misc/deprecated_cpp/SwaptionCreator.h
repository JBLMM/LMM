//#pragma once
//
//#include <cstring>
//#include <fstream>
//#include <vector>
//#include "Swap.h"
//#include "DateCreator.h"
//#include "Approximation.h"
//#include "PiecewiseConstantVolatility.h"
//#include "DeterministicVol.h"
//
//typedef std::vector<std::vector<std::pair<double,double>>> matrix_vol_strike;
//typedef std::vector<std::vector<std::vector<size_t>>> indexMatrix;
//
//class SwaptionCreator
//{
//public:
//	static matrix_ createSwaptionMatrix(const matrix_pairOfIndices&,
//										size_t liborTenor,
//										size_t floatingTenor,
//										size_t fixingTenor,
//										matrix_& out_strikes,
//										matrix_& out_swapRates,
//										matrix_& out_annuities,
//										indexMatrix& floatingIndexMatrix,
//										const std::vector<double>& bonds_0,
//										const std::vector<double>& libors_init,
//										const std::vector<double>& libors_shifts,
//										Approximation& approximation);
//
//	
//	
//	static matrix_vol_strike createSwaptionMatrix_withStrike(const matrix_pairOfIndices&);
//
//	static void print_swaptionMatrix(std::ofstream& outputFile, const matrix_& swaptionMatrix);
//
//	//--------------------------------------------------//
//	//                      TESTS                       //
//	//--------------------------------------------------//
//
//	static void test_swaptionMatrixCreation();
//
//};
