//#pragma once
//
//#include <vector>
//
//#include <boost/shared_ptr.hpp>
//
////#include <LMM/helper/Name.h>
////#include <LMM/helper/Tensor.h>
////#include <LMM/RNGenerator/RNGenerator.h>
////#include <LMM/helper/LMMTenorStructure.h>
////#include <LMM/LmmModel/Dispersion.h>
//#include <LMM/LmmModel/Lmm.h>
//
//
///*  -----------------------------------------------------------
//
//    shifted lmm Model: 
//
//	dL_i(t) = phi_i(t,L_i(t)) dW_t^{T_{i+1}} 
//	phi_i(t,L_i(t))  = varphi(t, L_i(t)) * vol(t) * B_i(t)
//	(i.  shifted lmm: \varphi(t, L_i(t)) = L_i(t) + a_i
//
//   -----------------------------------------------------------*/
//
//class ShiftedLmm : public Lmm
//{
//	std::vector<double> shifts_;  // const for each libor                
//
//public:
//	ShiftedLmm( const Dispersion & dispersion, const std::vector<double> & shiftsLibors);
//
//	//const std::vector<double>& get_shifts() const {return shifts_;}
//	//double get_shift(LMM::Index indexLibor) const {return shifts_[indexLibor];}
//
//	double equivalentShift(LMM::Index liborIndex, LMM::Index timeIndex, const std::vector<double>& liborsInitValue) const {return shifts_[liborIndex];}
//	double nativeShift(LMM::Index liborIndex) const {return shifts_[liborIndex];}
//};
//
//
//typedef boost::shared_ptr<ShiftedLmm> ShiftedLmm_PTR;
