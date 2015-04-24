//#pragma once
//#include <ql/types.hpp>
//#include <boost/numeric/ublas/vector.hpp>
//#include <boost/numeric/ublas/matrix.hpp>
//
//#include <time.h>
//#include <boost/random/normal_distribution.hpp>
//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/variate_generator.hpp>
//
//using namespace boost;
//using namespace boost::numeric::ublas;
//using namespace QuantLib;
//
//
//typedef boost::variate_generator<boost::mt19937, boost::normal_distribution<> > RNG_Type;
//
//
///**
// * @brief Abstract class to build volatility models
// */
//class VolatilityModel 
//{
//
//protected:
//	/// (N+1)x3 matrix obtained by PCA 
//	matrix<Real> B;
//	/// Correlation matrix, defined by B * trans(B)
//	matrix<Real> corrMatrix;
//	//! 
//
//public:
//
//	// Static protected getters to access attributes of 'non this' object //
//	static matrix<Real> getB(const VolatilityModel & myVolMod);
//	static matrix<Real> getCorrMatrix(const VolatilityModel & myVolMod);
//
//	
//	virtual ~VolatilityModel() {}
//
//	virtual const matrix<Real> &getCorr()=0;
//
//	virtual Real computeIntSto(size_t i, 
//		                       size_t j,
//		                       const numeric::ublas::vector<matrix<Real>> &covTensor,
//							   numeric::ublas::vector<Real> &G)=0;
//
//	//-- TODO : Remove that method later
//	// Compute rho_{i,j} * sigma_i * sigma_j, with sigma_i following an ABCD structure
//	virtual Real abcdCov(size_t i, size_t j, Time Ti, Time Tj, Time t, Real a, Real b, Real c, Real d)=0;
//
//	// Compute int_S^T {rho_{i,j} * sigma_i * sigma_j}
//	virtual Real covIntegral(size_t i, size_t j, Time Ti, Time Tj, Time S, Time T)=0;
//
//	// Compute int_S^T {sigma_i * sigma_i}
//	virtual Real varIntegral(size_t i, Time Ti, Time S, Time T)=0;
//
//	virtual const matrix<Real> &getterB()=0;
//
//};
