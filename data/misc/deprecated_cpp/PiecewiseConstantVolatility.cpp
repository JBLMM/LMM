//#include "PiecewiseConstantVolatility.h"
//#include <iostream>
//#include <cassert>
//
//
//PiecewiseConstantVolatility::PiecewiseConstantVolatility()
//{}
//
//PiecewiseConstantVolatility::PiecewiseConstantVolatility(const matrix_& h,
//														 const std::vector<double>& tenorDates,
//		                                                 size_t reducedRank, 
//					                                     size_t fullRank)
//{
//	//-- Check if the number of dates in the vol matrix is same as the given vector of dates
//	assert(h[0].size() == tenorDates.size()-1);
//	
//	bool isWellInitialized = true;
//	for each (double h_i in h[0]) //-- Check if the first row has only zeros
//	{
//		if (h_i != 0.)
//			isWellInitialized = false;
//	}
//	for (size_t i = 0; i < h.size();++i) //-- Check if the first clomn has only zeros
//	{
//		if (h[i][0] != 0.)
//			isWellInitialized = false;
//	}
//
//	if (!isWellInitialized)
//		throw("Vol matrix must have its first column and row set to zero");
//	
//
//	BM_ = reducedRank;
//
//	//-- TODO : Allow other correlation methods to be used
//	double beta  = 0.1;
//	double rhoInfty = 0.0158;
//	CorrelationReductionTyype reductionType =  CorrelationReductionTyype::RA; // PCA, RA;
//
//	correlation_ = new Robonato2_Correlation(fullRank, BM_, reductionType, beta, rhoInfty);
//	correlation_->calculate();
//
//	Matrix B_ = correlation_->get_reducedCorrelMatrixB();
//	for (size_t i = 0; i < B_.rows(); ++i)
//	{
//		std::vector<double> B_row;
//		
//		for (size_t j = 0; j < B_.columns(); ++j)
//			B_row.push_back(B_[i][j]);
//
//		B.push_back(B_row);
//	}
//
//	Matrix corrMatrix_ = correlation_->get_reducedCorrelMatrixApprox();
//	for (size_t i = 0; i < corrMatrix_.rows(); ++i)
//	{
//		std::vector<double> corr_row;
//		for (size_t j = 0; j < corrMatrix_.columns(); ++j)
//			corr_row.push_back(corrMatrix_[i][j]);
//
//		corrMatrix.push_back(corr_row);
//	}
//
//	h_ = h;
//	tenorDates_ = tenorDates;
//}
//
//
//PiecewiseConstantVolatility::PiecewiseConstantVolatility(const PiecewiseConstantVolatility& myVolMod)
//{
//	h_ = myVolMod.h_;
//	tenorDates_ = myVolMod.tenorDates_;
//	BM_ = myVolMod.BM_;
//	this->B = myVolMod.B;
//	this->corrMatrix = myVolMod.corrMatrix;
//}
//
//
////-- We assume that S <= T
//double PiecewiseConstantVolatility::covIntegral(size_t i, size_t j, Time Ti, Time Tj, Time S, Time T)
//{
//	if (S > T)
//		throw("Case S > T not handled yet");
//
//	if (Ti < S || Tj < S || S == T)
//	{
//		return 0.;
//	}
//	else
//	{
//		//-- Find the index of the date immediately greater than S
//		size_t index_after_S = findIndexAfterGivenDate(S);
//
//		//-- find the index of the date immediately smaller than T
//		size_t index_before_T = findIndexBeforeGivenDate(T);
//		
//		//-- In case T is between S and tenorDates[index_after_S]
//		double min_val = (tenorDates_[index_after_S] <= T)?tenorDates_[index_after_S]:T;
//
//		double covariance = (min_val-S)*(h_[i][index_after_S] * h_[j][index_after_S]);
//
//		if (index_before_T >= index_after_S)
//			covariance += (T-tenorDates_[index_before_T])*(h_[i][index_before_T+1] * h_[j][index_before_T+1]);
//
//		for (size_t k = index_after_S+1; k <= index_before_T; ++k)
//			covariance += (tenorDates_[k]-tenorDates_[k-1]) * h_[i][k] * h_[j][k]; 
//
//		covariance *= this->corrMatrix[i][j];
//		return covariance;
//	}
//}
//
//double PiecewiseConstantVolatility::varIntegral(size_t i, Time Ti, Time S, Time T)
//{
//	return covIntegral(i,i,Ti,Ti,S,T);
//}
//
////---- For Calibration -----//
//
//
////-- Array x contains volatility matrix components
//double PiecewiseConstantVolatility::covIntegral(const Array& x, size_t i, size_t j, Time Ti, Time Tj, Time S, Time T)
//{
//	if (S > T)
//		throw("Case S > T not handled yet");
//
//	if (Ti < S || Tj < S || S == T)
//	{
//		return 0.;
//	}
//	else
//	{
//		//-- Map array to volatility matrix 
//		matrix_ H = map_ArrayToMatrix(x); // Precalculer et veriier si c'est la meme matrice ou pas (pas besoin de recalculer)
//
//		//-- Find the index of the date immediately greater than S
//		size_t index_after_S = findIndexAfterGivenDate(S);
//
//		//-- find the index of the date immediately smaller than T
//		size_t index_before_T = findIndexBeforeGivenDate(T);
//		
//		//-- In case T is between S and tenorDates[index_after_S]
//		double min_val = (tenorDates_[index_after_S] <= T)?tenorDates_[index_after_S]:T;
//
//		double covariance = (min_val-S)*(H[i][index_after_S] * H[j][index_after_S]);
//
//		if (index_before_T >= index_after_S)
//			covariance += (T-tenorDates_[index_before_T])*(H[i][index_before_T+1] * H[j][index_before_T+1]);
//
//		for (size_t k = index_after_S+1; k <= index_before_T; ++k)
//			covariance += (tenorDates_[k]-tenorDates_[k-1]) * H[i][k] * H[j][k]; 
//
//		covariance *= this->corrMatrix[i][j];
//		return covariance;
//	}
//}
//
//double PiecewiseConstantVolatility::varIntegral(const Array& x, size_t i, Time Ti, Time S, Time T)
//{
//	return covIntegral(x,i,i,Ti,Ti,S,T);
//}
//
////--------------------------//
//
//size_t PiecewiseConstantVolatility::findIndexAfterGivenDate(double date)
//{
//	size_t index_after_date = 0;
//		while (date >= tenorDates_[index_after_date])
//			index_after_date++;
//
//	return index_after_date;
//}
//
//size_t PiecewiseConstantVolatility::findIndexBeforeGivenDate(double date)
//{
//	size_t index_before_T = tenorDates_.size() - 1;
//	while (date <= tenorDates_[index_before_T])
//		index_before_T--;
//
//	return index_before_T;
//}
//
//void PiecewiseConstantVolatility::printVolMatrix()
//{
//	for (size_t i = 0; i < h_.size(); ++i)
//	{
//		for (size_t j = 0; j < h_[0].size(); ++j)
//			std::cout << h_[i][j] << " ";
//
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//}
//
//void PiecewiseConstantVolatility::printTenorDates()
//{
//	for (size_t i = 0; i < tenorDates_.size(); ++i)
//		std::cout << tenorDates_[i] << " ";
//
//	std::cout << std::endl << std::endl;
//}
//
//
//Array PiecewiseConstantVolatility::map_MatrixToArray(const matrix_& h)
//{
//	std::vector<double> tmpRes;
//	size_t nbRows = h.size();
//	for (size_t i = 1; i < nbRows; ++i)
//	{
//		for (size_t j = 1; j <= i; ++j)
//		{	
//			tmpRes.push_back(h[i][j]);
//		}
//	}
//
//	size_t nbVol = tmpRes.size();
//	Array res(nbVol);
//	for (size_t i = 0; i < nbVol; ++i)
//		res[i] = tmpRes[i];
//
//	return res;
//}
//
//matrix_ PiecewiseConstantVolatility::map_ArrayToMatrix(const Array& x)
//{
//	size_t h_size = h_.size();
//	matrix_ h(h_size,std::vector<double>(h_size));
//	size_t array_index = 0;
//	for (size_t i = 1; i < h_size; ++i)
//	{
//		for (size_t j = 1; j <= i; ++j)
//		{
//			h[i][j] = x[array_index];
//			array_index++;
//		}
//	}
//	return h;
//}
//
//void PiecewiseConstantVolatility::saveMatrix(ofstream& outputFile) 
//{
//	//outputFile << "Input volatility matrix" << std::endl << std::endl;
//	
//	for (size_t i = 0; i < h_.size(); ++i)
//	{
//		outputFile << i << "\t";
//		for (size_t j = 0; j < h_[i].size(); ++j)
//			outputFile << h_[i][j] << "\t";
//		
//		outputFile<< "\n";
//	}
//	outputFile << std::endl << std::endl;
//}
//
//
////-- Getters/Setters
//
//const std::vector<double>& PiecewiseConstantVolatility::get_tenorDates()
//{return tenorDates_;}
//
//void PiecewiseConstantVolatility::set_tenorDates(const std::vector<double>& tenorDates)
//{tenorDates_ = tenorDates;}
//
//
////-----------------------------------------------------------------------//
////                               TESTS                                   //
////-----------------------------------------------------------------------//
//
//void PiecewiseConstantVolatility::test_findDate()
//{
//	std::cout << "//-- Tenor Dates : " << std::endl << std::endl;
//	printTenorDates();
//
//	double date = 1.5; // Date is on among tenor dates
//	size_t afterDate = findIndexAfterGivenDate(date);
//	size_t beforeDate = findIndexBeforeGivenDate(date);
//
//	std::cout << "date is: " << date << " (which is on the grid)" << std::endl;
//	std::cout << "previous date index: " << beforeDate << " -- previous date: " << tenorDates_[beforeDate] << std::endl;
//	std::cout << "following date index: " << afterDate << " -- following date: " << tenorDates_[afterDate] << std::endl;
//	std::cout << "Same dates, as expected" << std::endl << std::endl;
//
//	date = 0.3; // Date is between T0 = 0. and T1 = 0.5
//	afterDate = findIndexAfterGivenDate(date);
//	beforeDate = findIndexBeforeGivenDate(date);
//
//	std::cout << "date is: " << date << std::endl;
//	std::cout << "previous date index: " << beforeDate << " -- previous date: " << tenorDates_[beforeDate] << std::endl;
//	std::cout << "following date index: " << afterDate << " -- following date: " << tenorDates_[afterDate] << std::endl << std::endl;
//
//	date = 7.8; // Date is between T15 = 7.5 and T16 = 8.0
//	afterDate = findIndexAfterGivenDate(date);
//	beforeDate = findIndexBeforeGivenDate(date);
//
//	std::cout << "date is: " << date << std::endl;
//	std::cout << "previous date index: " << beforeDate << " -- previous date: " << tenorDates_[beforeDate] << std::endl;
//	std::cout << "following date index: " << afterDate << " -- following date: " << tenorDates_[afterDate] << std::endl;
//}
//
//
////-- Pre-conditions: 
////-- 1) Make sure the h matrix has only zeros on the first line and column
////-- 2) Make sure h is a low triangular matrix
//void PiecewiseConstantVolatility::test_computeCovariance()
//{
//	size_t index_i = 1;
//	size_t index_j = 2;
//	double Ti = 0.5; // Ti = T1
//	double Tj = 1.; // Tj = T2
//	double S = 0.;  // S = T0
//	double T = 0.5; // T = T1
//
//	// Compute int_T0^T1 {h_1,1 * h_2,1 * rho_1,2}
//	double covariance = covIntegral(index_i,index_j,Ti,Tj,S,T);
//
//	std::cout << "//-- 1st test: integral bounds are on the date grid" << std::endl << std::endl;
//	std::cout << "Compute int_T0^T1 {h_1,1 * h_2,1 * rho_1,2}: " << std::endl << std::endl; 
//	std::cout << "h_1,1: " << h_[1][1] << " -- h_2,1: " << h_[2][1] << " -- rho_1,2: " << corrMatrix[1][2] << std::endl << std::endl;
//	std::cout << "//-- Result: " << covariance << std::endl;
//
//	double expected_cov = (T-S) * h_[1][1] * h_[2][1] * corrMatrix[1][2];
//	std::cout << "//-- Expected result: " << expected_cov << std::endl << std::endl;
//
//	
//	S = 0.16;  // S > T0
//	covariance = covIntegral(index_i,index_j,Ti,Tj,S,T);
//
//	std::cout << "//-- 2nd test: lower bound is a bit higher than first grid date" << std::endl << std::endl;
//	std::cout << "Compute int_S^T {h_1,1 * h_2,1 * rho_1,2}: " << std::endl << std::endl; 
//	std::cout << "h_1,1: " << h_[1][1] << " -- h_2,1: " << h_[2][1] << " -- rho_1,2: " << corrMatrix[1][2] << std::endl << std::endl;
//	std::cout << "//-- Result: " << covariance << std::endl;
//
//	expected_cov = (T-S) * h_[1][1] * h_[2][1] * corrMatrix[1][2];
//	std::cout << "//-- Expected result: " << expected_cov << std::endl << std::endl;
//
//	S = 0.;  
//	T = 0.46; // T < T1
//	covariance = covIntegral(index_i,index_j,Ti,Tj,S,T);
//
//	std::cout << "//-- 3rd test: upper bound is a bit lower than second grid date" << std::endl << std::endl;
//	std::cout << "Compute int_S^T {h_1,1 * h_2,1 * rho_1,2}: " << std::endl << std::endl; 
//	std::cout << "h_1,1: " << h_[1][1] << " -- h_2,1: " << h_[2][1] << " -- rho_1,2: " << corrMatrix[1][2] << std::endl << std::endl;
//	std::cout << "//-- Result: " << covariance << std::endl;
//
//	expected_cov = (T-S) * h_[1][1] * h_[2][1] * corrMatrix[1][2];
//	std::cout << "//-- Expected result: " << expected_cov << std::endl << std::endl;
//
//	S = 0.;  
//	T = 1.; // T = T2
//	covariance = covIntegral(index_i,index_j,Ti,Tj,S,T);
//
//	std::cout << "//-- 4th test: Computations on larger grid (3 dates)" << std::endl << std::endl;
//	std::cout << "h_1,1: " << h_[1][1] << " -- h_2,1: " << h_[2][1] << " -- rho_1,2: " << corrMatrix[1][2] << std::endl;
//	std::cout << "h_2,2: " << h_[2][2] << std::endl << std::endl;
//	std::cout << "//-- Result: " << covariance << std::endl;
//
//	expected_cov = (0.5-S) * h_[1][1] * h_[2][1] * corrMatrix[1][2];
//	std::cout << "//-- Expected result: " << expected_cov << std::endl << std::endl;
//
//	index_i = 2; 
//	index_j = 3;
//	S = 0.; // S = T0
//	T = 1.; // T = T2
//
//	covariance = covIntegral(index_i,index_j,Ti,Tj,S,T);
//
//	std::cout << "//-- 5th test: We compute covariance between 2nd and 3rd libor" << std::endl << std::endl;
//	std::cout << "h_2,1: " << h_[2][1] << "h_2,2: " << h_[2][2] << " -- h_3,1: " << h_[3][1] << " -- h_3,2: " << h_[3][2] << " -- rho_2,3: " << corrMatrix[2][3] << std::endl << std::endl;
//	std::cout << "//-- Result: " << covariance << std::endl;
//
//	expected_cov = (0.5-S) * h_[2][1] * h_[3][1] * corrMatrix[2][3] + (T-0.5) * h_[2][2] * h_[3][2] * corrMatrix[2][3];
//	std::cout << "//-- Expected result: " << expected_cov << std::endl << std::endl;
//
//	S = 0.2; // S = T0
//	T = 0.9; // T = T2
//	covariance = covIntegral(index_i,index_j,Ti,Tj,S,T);
//
//	std::cout << "//-- 6th test: covariance between 2nd and 3rd libor, with intergal bounds not on the grid" << std::endl << std::endl;
//	std::cout << "h_2,1: " << h_[2][1] << "h_2,2: " << h_[2][2] << " -- h_3,1: " << h_[3][1] << " -- h_3,2: " << h_[3][2] << " -- rho_2,3: " << corrMatrix[2][3] << std::endl << std::endl;
//	std::cout << "//-- Result: " << covariance << std::endl;
//
//	expected_cov = (0.5-S) * h_[2][1] * h_[3][1] * corrMatrix[2][3] + (T-0.5) * h_[2][2] * h_[3][2] * corrMatrix[2][3];
//	std::cout << "//-- Expected result: " << expected_cov << std::endl << std::endl;
//}
//
//void PiecewiseConstantVolatility::test_computeVariance()
//{
//	size_t index_i = 1;
//	double Ti = 0.5; // Ti = T1
//	double S = 0.;  // S = T0
//	double T = 0.5; // T = T1
//
//	double variance = varIntegral(index_i,Ti,S,T);
//
//	std::cout << "//-- 1st test: variance of libor_1 between T0=0 and T1=0.5" << std::endl << std::endl;
//	std::cout << "//-- Result: " << variance << std::endl;
//	double expected_var = (T-S)*h_[index_i][1]*h_[index_i][1];
//	std::cout << "//-- Expected result: " << expected_var << std::endl << std::endl;
//
//	index_i = 3;
//	T = 1.5;
//	variance = varIntegral(index_i,Ti,S,T);
//
//	std::cout << "//-- 2nd test: variance of libor_3 between T0=0 and T3=1.5" << std::endl << std::endl;
//	std::cout << "//-- Result: " << variance << std::endl;
//    expected_var = (0.5-S)*h_[index_i][1]*h_[index_i][1] + (1.-0.5)*h_[index_i][2]*h_[index_i][2] + (T-1.)*h_[index_i][3]*h_[index_i][3];
//	std::cout << "//-- Expected result: " << expected_var << std::endl << std::endl;
//
//	index_i = 3;
//	S = 0.1;
//	T = 1.3;
//	variance = varIntegral(index_i,Ti,S,T);
//
//	std::cout << "//-- 3rd test: variance of libor_3 between dates out of the grid" << std::endl << std::endl;
//	std::cout << "//-- Result: " << variance << std::endl;
//    expected_var = (0.5-S)*h_[index_i][1]*h_[index_i][1] + (1.-0.5)*h_[index_i][2]*h_[index_i][2] + (T-1.)*h_[index_i][3]*h_[index_i][3];
//	std::cout << "//-- Expected result: " << expected_var << std::endl << std::endl;
//}
//
//void PiecewiseConstantVolatility::test_computeAllCovariances(size_t N_libor)
//{
//	std::vector<matrix_> covTensor = computeAllCovariances(N_libor,tenorDates_);
//
//	for each (matrix_ covMat in covTensor)
//	{
//		for each (std::vector<double> covVect in covMat)
//		{
//			for each (double cov in covVect)
//				std::cout << cov << " ";
//
//			std::cout << std::endl;
//		}
//		std::cout << std::endl;
//	}
//}
//
