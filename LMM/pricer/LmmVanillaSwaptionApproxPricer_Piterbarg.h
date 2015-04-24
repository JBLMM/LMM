#pragma once

#include <cassert>

#include <boost/shared_ptr.hpp>

#include <LMM/numeric/NumericalMethods.h>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/ShiftedLmm.h>
#include <LMM/LmmModel/Dispersion.h>
#include <LMM/pricer/LmmVanillaSwapPricer.h>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>


class LmmVanillaSwaptionApproxPricer_Piterbarg : public LmmVanillaSwapPricer
{	
protected:
	Lmm_PTR lmm_;
	//Shifted_HGVolatilityFunction_PTR shifted_HGVolatilityFunction_; // YY: sorry cannot do the casting

	//! preCalculation: for dSdL_i
	mutable std::vector<double>		buffer_liborAtExpensionInit_;
	mutable std::vector<double>     buffer_dSdLi_; 

	mutable std::vector<LMM::Index> buffer_floatingLegPaymentIndexSchedule_;
	mutable std::vector<LMM::Index> buffer_fixedLegPaymentIndexSchedule_; 
	mutable std::vector<double>     buffer_deltaTFloatingLeg_; 
	mutable std::vector<double>     buffer_deltaTFixedLeg_; 
	mutable LMM::Index				buffer_swapStartIndex_;
	mutable LMM::Index				buffer_swapEndIndex_;

	void initializeConstShift();
private:
	std::vector<double>         constShifts_;

protected:
	////！calculate the Piterbarg implied vol
	//class FwdZCI_Derivable 
	                         //   fwdZCInversed: P^{-1}(t,T_i,T_j) 
	//	                     // = \prod_{k=i}^{j-1} P^{-1}(t,T_k,T_{k+1})
	//					     // = \prod_{k=i}^{j-1} ( 1 + delta_k * L_k(t) )
	//{
	//public:
		double fwdZCInverse(size_t liborIndex) const; //fwdZCInversed: P^{-1}(t,T_k,T_{k+1}) = 1 + delta_k * L_k(t)
		double fwdZCInverse(size_t liborIndex_begin, size_t liborIndex_end) const;  //fwdZCInversed: P^{-1}(t,T_i,T_j) =  = \prod_{k=i}^{j-1} P^{-1}(t,T_k,T_{k+1})
		double fwdZCInverse_1stDerivative(size_t liborIndex_begin, size_t liborIndex_end, size_t liborIndex_derivative) const;
		double fwdZCInverse_2ndDerivative(size_t liborIndex_begin, size_t liborIndex_end, size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const;

		double deltaMultiplyfwdZC(size_t liborIndex) const; // delta_k / ( 1 + delta_k * L_k(t) );

	//};


	//class SwapRateNumerator_Derivable 
	//{
	//public:
		double swapRateNumerator() const; // Numerator = ( P(t,T_p) - P(t,T_q) ) / P(t,T_q) = P(t,T_p,T_q)-1
		double swapRateNumerator_1stDerivative(size_t liborIndex_derivative) const;
		double swapRateNumerator_2ndDerivative(size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const;
	//};


	//class SwapRateDenominator_Derivable
	//{
	//public:
		double swapRateDenominator() const;
		double swapRateDenominator_1stDerivative(size_t liborIndex_derivative) const;
		double swapRateDenominator_2ndDerivative(size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const;
	//};


	//class SwapRate_Derivable  // swapRate = swapNumerator / swapDenominator
	//{
	//public:
		double swapRate() const;
		double swapRate_1stDerivative(size_t liborIndex_derivative) const;
		double swapRate_2ndDerivative(size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const;
	//};


	//class SwapRateVolatility_Derivable  // swapRate = swapNumerator / swapDenominator
	//{
	//public:
		double swapRateVolatility() const;
		double swapRateVolatility_1stDerivative(size_t liborIndex_derivative) const;
	//};



public:
	//! constructor  
	LmmVanillaSwaptionApproxPricer_Piterbarg(const Lmm_PTR& lmmModel);
	//! destructor
	virtual ~LmmVanillaSwaptionApproxPricer_Piterbarg(){};
	//!  reset the Volatility Parameters, recompute all covariance, cumulated covariance tensors
	void update_VolatilityParam(VolatilityParam_PTR vol_param_ptr) ; // why ?


protected:
	//! precalculation: YY TODO: can be further optimized for calibration problem. 
	void preCalculateLiborAtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const;
	void preCalculate_dSdLi_AtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const;
	virtual void preCalculateALL(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const; 


	//! method using for swaption approx pricer
	// little different than Robonato
public:
	double volBlack(const VanillaSwaption& vanillaSwaption, const std::vector<double>& liborsInitValue) const;
	// exactly the same as Robonato.
	double price(const VanillaSwaption & swaption, const std::vector<double> & libor_init )const;

private:
	//! YY : why does this fucntion exist ?  
	double fwdZC(int liborIndex_i, int liborIndex_j);
};

typedef boost::shared_ptr<LmmVanillaSwaptionApproxPricer_Piterbarg> LmmVanillaSwaptionApproxPricer_Piterbarg_PTR;
typedef boost::shared_ptr<const LmmVanillaSwaptionApproxPricer_Piterbarg> LmmVanillaSwaptionApproxPricer_Piterbarg_CONSTPTR;

