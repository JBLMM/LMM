//#pragma once
//
//#include <cassert>
//
//#include <boost/shared_ptr.hpp>
//
//#include <LMM/numeric/NumericalMethods.h>
//#include <LMM/instrument/VanillaSwaption.h>
//#include <LMM/LmmModel/Lmm.h>
//#include <LMM/LmmModel/Dispersion.h>
//#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Piterbarg.h>
//
//
////! not for the moment, we don't use the real Piterbarg model, but only the shifted LMM. 
//
//class LmmVanillaSwaptionApproxPricer_MP : public LmmVanillaSwaptionApproxPricer_Piterbarg
//{	
//private:
//
//	//! some information contained in LMM, put it here for simplicity:
//	Matrix B_; 
//	size_t nbFactor_; 
//	
//
//	//! already exist in LmmVanillaSwaptionApproxPricer_Piterbarg
//
//	//Lmm_PTR lmm_;
//	//mutable std::vector<double>		buffer_liborAtExpensionInit_;
//	//mutable std::vector<double>     buffer_dSdLi_; 
//	//mutable std::vector<LMM::Index> buffer_floatingLegPaymentIndexSchedule_;
//	//mutable std::vector<LMM::Index> buffer_fixedLegPaymentIndexSchedule_; 
//	//mutable std::vector<double>     buffer_deltaTFloatingLeg_; 
//	//mutable std::vector<double>     buffer_deltaTFixedLeg_; 
//	//mutable LMM::Index				buffer_swapStartIndex_;
//	//mutable LMM::Index				buffer_swapEndIndex_;
//
//	//! dL_i(t) = phi(t, L_i(t)) dW_t^{Q^{T+i+1}}: to make the formula more general...
//	//! in real Piterbarg Model : phi(t,x) = beta_i(t)*L_i(t) + (1-beta_i(t))*L_i(0)
//	//! in shifted LMM Model:     phi(t,x) = L_i(t) + a_i
//	//std::vector<double> phi(double t, size_t liborIndex) const;  // YY: TODO: not efficient return a vector copy, to improve it latter.
//	//std::vector<double> phi_1stDerivative(double t, size_t liborIndex, size_t liborIndex_derivative) const;
//
//	//std::vector<double> psi(double t, size_t liborIndex, const VanillaSwaption& vanillaswaption) const;  // YY: TODO: not efficient return a vector copy, to improve it latter.
//	//double psi_1stDerivative(double t, size_t liborIndex, size_t liborIndex_derivative) const;
//	
//
//	//! sigma_A
//	double swapVol(double t, const VanillaSwaption& vanillaswaption) const;
//	double swapVol_1stDerivative(double t, const VanillaSwaption& vanillaswaption, size_t liborIndex_derivative) const;
//	//double swapVol_2ndDerivative(size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const;
//
//public:
//	//! constructor  
//	LmmVanillaSwaptionApproxPricer_MP(const Lmm_PTR& lmmModel);
//	//! destructor
//	virtual ~LmmVanillaSwaptionApproxPricer_MP(){};
//	//!  reset the Volatility Parameters, recompute all covariance, cumulated covariance tensors
//	void update_VolatilityParam(VolatilityParam_PTR vol_param_ptr);
//
//
//	//! precalculation: YY TODO: can be further optimized for calibration problem. 
//	//void preCalculateLiborAtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const;
//	//void preCalculate_dSdLi_AtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const;
//	void preCalculateALL(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const; 
//
//
//	//! method using for swaption approx pricer
//	double volBlack(const VanillaSwaption& vanillaSwaption, const std::vector<double>& liborsInitValue) const;
//	double price(const VanillaSwaption & swaption, const std::vector<double> & libor_init )const;
//};
//
//typedef boost::shared_ptr<LmmVanillaSwaptionApproxPricer_MP> LmmVanillaSwaptionApproxPricer_MP_PTR;
//typedef boost::shared_ptr<const LmmVanillaSwaptionApproxPricer_MP> LmmVanillaSwaptionApproxPricer_MP_CONSTPTR;
//
