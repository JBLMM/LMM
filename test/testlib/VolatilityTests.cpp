//#include <iostream>
//#include "VolatilityTests.h"
//
//
//// compute (a+b*u)*exp(-c*u)+d, where u = T_index - t
//void computeAbcdVol(double t, size_t index_maturity)
//{
//	Real a = -0.06; 
//	Real b = 0.17; 
//	Real c = 0.54; 
//	Real d = 0.17;
//
//	std::vector<Real> abcdParams(4);
//	abcdParams[0] = a;
//	abcdParams[1] = b;
//	abcdParams[2] = c;
//	abcdParams[3] = d;
//
//	size_t N = 19;
//	size_t nbFactors = 3;
//	double tenorLibor = 0.5; // 6M
//
//	//-- Set tenor periods (in years)
//	Time firstTenor = 1;
//	std::vector<Time> tenorPeriods(N+1);
//	for (size_t i = 0; i < N+1; ++i) 
//		tenorPeriods[i] = tenorLibor;
//
//	//-- Set tenor dates (in years)
//	std::vector<Time> tenorDates(N+2);
//	tenorDates[0] = 0.;
//	for (size_t i = 1; i < N+2; ++i)
//		tenorDates[i] = tenorDates[i-1] + tenorPeriods[i-1];
//
//	//-- Constant parameters for abcd vol structure
//	std::vector<double> g(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		g[i] = 1.;
//
//	DeterministicVol vol(nbFactors,N+1,abcdParams,g);
//
//	double Ti = tenorDates[index_maturity];
//	double abcdVol = (a+b*(Ti-t))*exp(-c*(Ti-t))+d;
//	
//	std::cout << "abcd volatility for the " << index_maturity << "-th Libor : " << abcdVol << std::endl;
//}
//
//
//// compute int_Tk^Tk+1 { sigma_i * sigma_j * rho_i,j}, for all i,j (k = index_time)
//void computeCovariancesTest(size_t index_time)
//{
//	Real a = -0.06; 
//	Real b = 0.17; 
//	Real c = 0.54; 
//	Real d = 0.17;
//
//	std::vector<Real> abcdParams(4);
//	abcdParams[0] = a;
//	abcdParams[1] = b;
//	abcdParams[2] = c;
//	abcdParams[3] = d;
//
//	size_t N = 19;
//	size_t nbFactors = 3;
//	double tenorLibor = 0.5; // 6M
//
//
//	//-- Constant parameters for abcd vol structure
//	std::vector<double> g(N+1);
//	for (size_t i = 0; i < N+1; ++i)
//		g[i] = 1.;
//
//	//-- Set tenor periods (in years)
//	Time firstTenor = 1;
//	std::vector<Time> tenorPeriods(N+1);
//	for (size_t i = 0; i < N+1; ++i) 
//		tenorPeriods[i] = tenorLibor;
//
//	//-- Set tenor dates (in years)
//	std::vector<Time> tenorDates(N+2,0.);
//	for (size_t i = 1; i < N+2; ++i)
//		tenorDates[i] = tenorDates[i-1] + tenorPeriods[i-1];
//
//	DeterministicVol vol(nbFactors,N+1,abcdParams,g);
//
//	std::vector<std::vector<std::vector<double>>> covTensor(N+1);
//	covTensor = vol.computeAllCovariances(N,tenorDates);
//
//	//-- Print the covariance matrix
//	for (size_t i = 0; i < covTensor[index_time].size(); ++i)
//	{
//		for (size_t j = 0; j < covTensor[index_time][0].size(); ++j)
//			std::cout << covTensor[index_time][i][j] << " ";
//
//		std::cout << std::endl;
//	}
//}