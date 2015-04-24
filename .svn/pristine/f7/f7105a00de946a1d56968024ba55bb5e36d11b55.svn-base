//#include <iostream>
//#include "SwaptionCreator.h"
//
//
////-- Mettre years en input et convertir dans methode
//matrix_ SwaptionCreator::createSwaptionMatrix(const matrix_pairOfIndices& matrixIndices,
//	                                          size_t liborTenor,
//											  size_t floatingTenor,
//										      size_t fixingTenor,
//											  matrix_& out_strikes,
//										      matrix_& out_swapRates,
//											  matrix_& out_annuities,
//											  indexMatrix& floatingIndexMatrix,
//										      const std::vector<double>& bonds_0,
//										      const std::vector<double>& libors_init,
//											  const std::vector<double>& libors_shifts,
//											  Approximation& approximation)
//{
//	size_t size = matrixIndices.size();
//	size_t fix_float_ratio = fixingTenor/floatingTenor;
//	matrix_ swaptionMatrix(size,std::vector<double>(size));
//	out_strikes = matrix_(size,std::vector<double>(size));
//	out_swapRates = matrix_(size,std::vector<double>(size));
//	out_annuities = matrix_(size,std::vector<double>(size));
//	floatingIndexMatrix = indexMatrix(size,std::vector<std::vector<size_t>>(size));
//
//	size_t i = 0;
//	
//	for each (std::vector<std::pair<size_t,size_t>> vect_index in matrixIndices)
//	{
//		size_t j = 0;
//		//std::vector<std::vector<size_t>> current_floatingIndex(size);
//
//		for each (std::pair<size_t,size_t> pair_indices in vect_index)
//		{
//			size_t indexStart = pair_indices.first;
//			size_t indexEnd = pair_indices.second;
//			
//			if (indexStart == 0 || j == swaptionMatrix[i].size() || indexEnd <= indexStart)
//			{
//				j++;
//				continue;
//			}
//			
//
//			//-- Create the swap corresponding to indexStart & indexEnd
//			SwapInstruments::VanillaSwap swap(indexStart, 
//											  indexEnd,
//											  fixingTenor,
//											  floatingTenor,
//											  liborTenor);
//
//			std::vector<size_t> floatingLegIndices = swap.getFloatingPaymentIndices();
//			std::vector<size_t> fixingLegIndices = swap.getFixingPaymentIndices();
//			//current_floatingIndex.push_back(floatingLegIndices);
//			floatingIndexMatrix[i][j] = floatingLegIndices;
//
//			//-- Annuity
//			double annuity0 = 0.;
//			for each (size_t i in fixingLegIndices)
//				annuity0 += (fixingTenor/12.)*bonds_0[i];
//
//			out_annuities[i][j] = annuity0;
//
//			//-- Swap rate
//			double swapRate0 = (bonds_0[indexStart]-bonds_0[indexEnd])/annuity0;
//			out_swapRates[i][j] = swapRate0;
//			
//			//-- Strike
//			double K = swapRate0;
//			out_strikes[i][j] = K;
//
//			//-- Shift swap rate and strike
//			double shifted_swapRate0 = swapRate0;
//			double shifted_K = K; // ??? Not necessary?
//			approximation.accumulateShiftedSwapRateAndStrike(shifted_swapRate0,shifted_K,bonds_0,libors_shifts,floatingLegIndices,annuity0);
//
//			swaptionMatrix[i][j] = approximation.computeRebonatoVolatility(indexStart,shifted_swapRate0,annuity0,floatingLegIndices,bonds_0,libors_init,libors_shifts);
//			swaptionMatrix[i][j] = sqrt(swaptionMatrix[i][j]);
//			j++;
//		}
//		//floatingIndexMatrix[i] = current_floatingIndex;
//		i++;
//	}
//
//	return swaptionMatrix;
//}
//
//
//
//matrix_vol_strike SwaptionCreator::createSwaptionMatrix_withStrike(const matrix_pairOfIndices& matrixIndices)
//{
//	matrix_vol_strike mat;
//	return mat;
//}
//
//
//void SwaptionCreator::print_swaptionMatrix(std::ofstream& outputFile, const matrix_& swaptionMatrix)
//{
//	for each (std::vector<double> matrix_row in swaptionMatrix)
//	{
//		for each (double vol in matrix_row)
//			outputFile << vol << "\t";
//
//		outputFile << std::endl;
//	}
//	outputFile << std::endl;
//}
//
////--------------------------------------------------//
////                      TESTS                       //
////--------------------------------------------------//
//
//void SwaptionCreator::test_swaptionMatrixCreation()
//{
//	
//	//-- Create a matrix of pairs (indexStart;indexEnd) for swaptions
//	double lmmTenor = 1.0;
//	double lastYear = 3; // T_{N+1} = 10 => N+1 = 20
//	Matrix_PairOfYears matrix_of_years     = DateCreator::createSwaptionMatrixDates_Years(lastYear,lmmTenor);
//	matrix_pairOfMonths matrix_of_months   = DateCreator::convertSwaptionDates_YearsToMonths(matrix_of_years);
//	matrix_pairOfIndices matrix_of_indices = DateCreator::convertSwaptionDates_MonthsToIndices(lmmTenor,matrix_of_months);
//
//	size_t liborTenor = 12;
//	size_t floatingTenor = 12;
//	size_t fixingTenor = 12;
//	size_t fixfloatratio = fixingTenor/floatingTenor;
//
//	size_t size = matrix_of_indices.size();
//	size_t horizon = (matrix_of_indices[size-1][size-1]).second;
//
//	std::vector<double> libors_init(horizon);
//	for (size_t i = 0; i < horizon; ++i)
//		libors_init[i] = 0.0201003 + i*0.001;
//	
//	std::vector<double> tenorPeriods(horizon);
//	for (size_t i = 0; i < horizon; ++i)
//		tenorPeriods[i] = lmmTenor;
//
//	std::vector<double> tenorDates = DateCreator::createLmmTenorDates(tenorPeriods);
//
//	std::vector<double> bonds_0(horizon+1);
//	bonds_0[0] = 1.;
//		for (size_t i = 1; i <= horizon; ++i)
//			bonds_0[i] = bonds_0[i-1]/(1+tenorPeriods[i-1]*libors_init[i-1]);
//
//	std::vector<double> libors_shifts(horizon);
//	for (size_t i = 0; i < horizon; ++i)
//			libors_shifts[i] = 0.0;
//
//	Real a = -0.06; 
//	Real b = 0.40; 
//	Real c = 0.54; 
//	Real d = 0.10;
//
//	std::vector<double> g;
//	double g_interval = 0.1;
//	double g_pace  = g_interval/(horizon-1);
//	double g_start = 1-g_interval/2;
//	for (size_t i = 0; i < horizon; ++i)
//		g.push_back(g_start + i*g_pace);
//
//	matrix_ h(horizon,std::vector<double>(horizon,0.));
//		for (size_t i = 1; i < horizon; ++i)
//		{
//			for (size_t j = 1; j <= i; ++j)
//			{
//				double Ti = tenorDates[i];
//				double Tj = tenorDates[j];
//				double tau = Ti -Tj;
//				h[i][j] = g[i] * ((a+b*tau)*exp(-c*tau)+d);
//			}
//		}
//
//		
//	PiecewiseConstantVolatility volModel(h,tenorDates,3,horizon);
//
//	//-- TODO: write a function which initialize a vol class
//	Approximation approx(volModel,tenorDates);
//
//	matrix_ out_strikes;
//	matrix_ out_swapRates;
//	matrix_ out_annuities;
//	indexMatrix floatingIndexMatrix;
//
//	matrix_ swaptionMatrix = createSwaptionMatrix(matrix_of_indices,
//												  liborTenor,
//											      floatingTenor,
//										          fixingTenor,
//												  out_strikes,
//												  out_swapRates,
//												  out_annuities,
//												  floatingIndexMatrix,
//										          bonds_0,
//										          libors_init,
//											      libors_shifts,
//											      approx);
//
//	std::ofstream outputFile;
//	outputFile.open("E:\\SwaptionVolMatrix.xls");
//	print_swaptionMatrix(outputFile,swaptionMatrix);
//	print_swaptionMatrix(outputFile,out_swapRates);
//	print_swaptionMatrix(outputFile,out_strikes);
//	print_swaptionMatrix(outputFile,out_annuities);
//	outputFile.close();
//}