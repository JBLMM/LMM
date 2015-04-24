//#pragma once
//
//#include "VolatilityModel.h"
//
//class StochasticVol : public VolatilityModel {
//private:
//public:
//
//	StochasticVol();
//
//	virtual ~StochasticVol();
//
//	virtual const matrix<Real> &getCorr();
//
//	virtual Real computeIntSto(size_t i, size_t j,
//		const numeric::ublas::vector<matrix<Real>> &covTensor, numeric::ublas::vector<Real> &G);
//
//
//	 // Compute rho_{i,j} * sigma_i * sigma_j, with sigma_i following an ABCD structure
//	virtual Real abcdCov(size_t i, size_t j, Time Ti, Time Tj, Time t, Real a, Real b, Real c, Real d);
//
//	virtual Real covIntegral(size_t i, size_t j, Time Ti, Time Tj, Time S, Time T);
//
//	virtual Real varIntegral(size_t i, Time Ti, Time S, Time T);
//
//	virtual const matrix<Real> &getterB();
//};
//
