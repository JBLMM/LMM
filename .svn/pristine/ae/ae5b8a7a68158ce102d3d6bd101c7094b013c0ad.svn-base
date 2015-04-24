//#include "DeterministicVol.h"
//
//#include <cassert>
//#include <cmath>
//#include <time.h>
//#include <iostream>
//
//#include <boost/random/normal_distribution.hpp>
//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/variate_generator.hpp>
//
//#include <ql/termstructures/volatility/abcd.hpp>
//
//using namespace std;
//
//
//
//DeterministicVol::DeterministicVol() 
//{}
//
//DeterministicVol::DeterministicVol(size_t reducedRank,
//	                               size_t fullRank,	                              
//								   const std::vector<Real> &abcd,
//								   const std::vector<double>& g) 
//								   : InstantaneousVolatility(reducedRank,fullRank)
//{
//	abcdParams_ = abcd;
//	g_ = g;
//}
//
//
//DeterministicVol::DeterministicVol(const DeterministicVol & myVolMod) 
//	: InstantaneousVolatility(myVolMod.BM_,myVolMod.B.size())
//{
//	this->abcdParams_ = myVolMod.abcdParams_;
//	g_ = myVolMod.g_;
//}
//
//DeterministicVol::~DeterministicVol() {}
//
//#pragma endregion
//
//#pragma region methods
//
//
//
//Real DeterministicVol::covIntegral(size_t i, size_t j, Time Ti, Time Tj, Time S, Time T) 
//{
//	Real a = abcdParams_[0];
//	Real b = abcdParams_[1];
//	Real c = abcdParams_[2];
//	Real d = abcdParams_[3];
//
//	AbcdFunction func(a,b,c,d);
//	Real res = func.covariance(S,T,Ti,Tj);
//
//	res *= g_[i] * g_[j] * this->corrMatrix[i][j];
//	return res; 
//}
//
//
//Real DeterministicVol::varIntegral(size_t i, Time Ti, Time S, Time T) 
//{	
//	return this->covIntegral(i,i,Ti,Ti,S,T);
//}
//
//Real DeterministicVol::covIntegral(const Array& x, size_t i, size_t j, Time Ti, Time Tj, Time S, Time T)
//{
//	throw("Error not implemented.");
//	//return 0.;
//}
//
//Real DeterministicVol::varIntegral(const Array& x, size_t i, Time Ti, Time S, Time T)
//{
//	throw("Error not implemented.");
//	//return 0.;
//}
//
//
//
//
//const std::vector<Real>& DeterministicVol::getAbcdParams() {return this->abcdParams_;}
//
