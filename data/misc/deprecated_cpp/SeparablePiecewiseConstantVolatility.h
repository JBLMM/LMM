//#pragma once
//
//#include "DeterministicVol.h"
//
//typedef std::vector<std::vector<double>> matrix_;
//
//class SeparablePiecewiseConstantVolatility : public DeterministicVol
//{
//private:
//	matrix_ abcd_h_; // matrix of (abcd)*h_ij
//
//public:
//	SeparablePiecewiseConstantVolatility(size_t reducedRank, 
//										 size_t fullRank,		    
//										 const std::vector<Real>& abcd,
//										 const matrix_& h);
//
//	~SeparablePiecewiseConstantVolatility();
//
//	//-- Redefinition 
//	Real covIntegral(size_t i, size_t j, Time Ti, Time Tj, Time S, Time T);
//	Real varIntegral(size_t i, Time Ti, Time S, Time T);
//};
