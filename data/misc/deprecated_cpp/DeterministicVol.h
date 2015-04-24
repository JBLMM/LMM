//#pragma once
//
//#include "InstantaneousVolatility.h"
//#include <vector>
//
//
//typedef std::vector<std::vector<double>> matrix_;
//
//
//**
// * @brief ABCD volatility class
// */
//class DeterministicVol: public InstantaneousVolatility 
//{
//protected: 
//	// vector of abcd volatility parameters
//	std::vector<double> abcdParams_;
//
//	// Adjustement coefficients for each libor
//	std::vector<double> g_; 
//
//public:
//	DeterministicVol();
//
//	DeterministicVol(size_t reducedRank, 
//					 size_t fullRank,		    
//					 const std::vector<Real>& abcd,
//					 const std::vector<double>& g);
//
//	DeterministicVol(const DeterministicVol & myVolMod);
//	
//    ~DeterministicVol();
//
//	// rho_{ij} * int_S_T f(T_i-u)*f(T_j-u) du, where f(tau) = (a+b*tau)*exp(-c*tau) + d
//	Real covIntegral(size_t i, size_t j, Time Ti, Time Tj, Time S, Time T);
//	Real varIntegral(size_t i,        Time Ti,          Time S, Time T);
//
//	//-- For calibration ??
//	Real covIntegral(const Array& x, size_t i, size_t j, Time Ti, Time Tj, Time S, Time T);
//	Real varIntegral(const Array& x, size_t i,        Time Ti,			 Time S, Time T);
//
//	// Compute int_T_{j-1}^Tj { sigma_i dW_t} 
//	//Real computeIntSto(size_t i, size_t j, const std::vector<matrix_> &covTensor, const std::vector<Real> &G);
//
//
//	//-- Getters
//	const std::vector<Real> & getAbcdParams();
//
//	//-- Setters
//	void setParam_a(double a);
//	void setParam_b(double b);
//	void setParam_c(double c);
//	void setParam_d(double d);
//	void setParam_g_i(size_t index, double g_i);
//
//	//-- Test functions
//	void testCovariances();
//	void testStochasticIntegral(std::vector<matrix_>&);
//	double computeAbcd(double t, size_t index_date);
//	void drawAbcdGraph(const std::vector<double>& tenorDates, size_t index_date);
//};
