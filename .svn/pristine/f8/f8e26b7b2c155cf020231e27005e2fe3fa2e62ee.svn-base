//#pragma once
//
//#include <vector>
//
//#include <boost/shared_ptr.hpp>
//#include <boost/numeric/ublas/triangular.hpp>  // Triangular matrix
//#include <boost/numeric/ublas/io.hpp>
//
//#include <LMM/helper/Name.h>
//#include <LMM/helper/Tensor.h>
//#include <LMM/RNGenerator/RNGenerator.h>
//#include <LMM/helper/LMMTenorStructure.h>
//#include <LMM/LmmModel/Dispersion.h>
//#include <LMM/LmmModel/Lmm.h>
//
//
///*  -----------------------------------------------------------
//
//    lmm Model: 
//
//	dL_i(t) = phi_i(t,L_i(t)) dW_t^{T_{i+1}} 
//	phi_i(t,L_i(t))  = varphi(t, L_i(t)) * vol(t) * B_i(t)
//	(i.  shifted lmm: \varphi(t, L_i(t)) = L_i(t) + a_i
//	(ii. Piterbarg lmm: beta_i(t)*L_i(t) + (1-beta_i(t))*L_i(0)
//
//   -----------------------------------------------------------*/
//
//
//class BetaLmm : public Lmm
//{
//public:
//
//	typedef boost::numeric::ublas::triangular_matrix<double, boost::numeric::ublas::lower> LowerTriangularMatrix;
//
//	//! constructor
//	BetaLmm( const Dispersion & dispersion, const LowerTriangularMatrix & betas);
//
//	//! destructor
//	virtual ~BetaLmm(){}
//
//
//private :
//
//	LowerTriangularMatrix betas_;  
//
//private :
//	double get_beta(LMM::Index liborIndex, LMM::Index timeIndex) const;      
//	double equivalentShift(LMM::Index liborIndex, LMM::Index timeIndex, const std::vector<double>& liborsInitValue) const; // for MC simulation, beta for period [timeIndex-1, timeIndex]
//	  
//
//};
//
//typedef boost::shared_ptr<BetaLmm> BetaLmm_PTR;
////std::ostream& operator<<(std::ostream& os, const Lmm& lmm);
//
