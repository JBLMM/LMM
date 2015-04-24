#include <cassert>
#include <vector>

#include <LMM/numeric/NumericalMethods.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Piterbarg.h>
#include <boost/pointer_cast.hpp>

LmmVanillaSwaptionApproxPricer_Piterbarg::LmmVanillaSwaptionApproxPricer_Piterbarg(const Lmm_PTR& lmmModel)
	: LmmVanillaSwapPricer(lmmModel->get_LMMTenorStructure() )
	, lmm_(lmmModel)
	, buffer_liborAtExpensionInit_(lmm_->get_nbLIBOR(), -1e10)
	, buffer_dSdLi_(lmm_->get_nbLIBOR(), -1.0e10) // N+1 
	, constShifts_(lmm_->get_nbLIBOR())

	//  lmm_(lmmModel),
	//YY TODO: create these vectors with max size ... 
	//buffer_floatingLegPaymentIndexSchedule_(?size?),
	//buffer_fixedLegPaymentIndexSchedule_(?size?),
	//buffer_deltaTFloatingLeg_(?size?),
	//buffer_deltaTFixedLeg_(?size?),
	//buffer_swapStartIndex_(999999),
	//buffer_swapEndIndex_(999999)
{
	assert(lmm_->get_LMMTenorStructure()->get_tenorDate()[0] == 0.0);

	//shifted_HGVolatilityFunction_ = boost::dynamic_pointer_cast<Shifted_HGVolatilityFunction_PTR>(lmm_->get_dispersionRef().get_VolatilityFunction_PTR());
	//assert(shifted_HGVolatilityFunction_ != 0); // if ==0, cast is not succesful.
	assert(lmm_->get_dispersionRef().get_VolatilityFunction_PTR()->isConstShift()); // This approximation works only for constant shift	
	void initializeConstShift();
}



void LmmVanillaSwaptionApproxPricer_Piterbarg::initializeConstShift()   // for each libor: shift as a function of time should be const for Rebonato Approxi.
{	
	for(size_t i=0; i<constShifts_.size(); ++i)
	{
		constShifts_[i] = lmm_->get_dispersionRef().get_VolatilityFunction_PTR()->get_shift_timeIndexVersion(i,i);
	}
}


void LmmVanillaSwaptionApproxPricer_Piterbarg::update_VolatilityParam(VolatilityParam_PTR vol_param_ptr)  
{
	lmm_->update_VolatilityParam(vol_param_ptr);
}
 
double LmmVanillaSwaptionApproxPricer_Piterbarg::price(const VanillaSwaption & swaption, const std::vector<double> & liborsInitValue )const 
{
	this->preCalculateALL(liborsInitValue  ,swaption);

	const VanillaSwap & swap = swaption.getUnderlyingSwap();

	double swaprate0  = this->swapRate_Analytical(swap, liborsInitValue) ;
	double annuity0   = this->annuity0(swaption.getUnderlyingSwap(), liborsInitValue); // swapRate at time 0
	double strike     = swap.get_strike() ;
	double vol_approx = this->volBlack(swaption,liborsInitValue) ;
	
	LMM::Index swaptionIndexMaturity = swaption.get_indexMaturity();
	const double & timeToMaturity = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);

	double approxPrice = annuity0*NumericalMethods::Black_Price(swaprate0 ,strike , vol_approx , timeToMaturity) ;
	//double approxPrice = NumericalMethods::Black_Price(swaprate0 ,strike , vol_approx , buffer_anuity)  ; //older code

	return approxPrice;
}

double LmmVanillaSwaptionApproxPricer_Piterbarg::volBlack(const VanillaSwaption& vanillaswaption, const std::vector<double>& liborsInitValue) const
{
	this->preCalculateALL(liborsInitValue, vanillaswaption);

	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();

	assert(lmm_->get_horizon() >= vanillaSwap.get_indexEnd()); //! if not cannot price it.
	assert(lmm_->get_LMMTenorStructure()->get_tenorType() == vanillaSwap.get_lmmTenorStructureTenorType() );

	//! Piterbarg Formula: YY TODO: can be simplified:
	//  1. use the symmetric 
	//  2. precalculate the matrix dSdLi vector
	LMM::Index swaptionIndexMaturity = vanillaswaption.get_indexMaturity();
	double maturityTime  = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);
	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();

	double volSquare = 0.0;
	for(size_t i=0; i<floatingLegPaymentIndexSchedule.size(); ++i)
	{
		size_t floatingLegPaymentIndex_i = floatingLegPaymentIndexSchedule[i]; // = i+1
		size_t indexLibor_i = floatingLegPaymentIndex_i - 1;
		//size_t indexT_i = indexLibor_i;
		for(size_t j=0; j<floatingLegPaymentIndexSchedule.size(); ++j)
		{
			size_t floatingLegPaymentIndex_j = floatingLegPaymentIndexSchedule[j]; // = j+1
			size_t indexLibor_j = floatingLegPaymentIndex_j - 1;

			//size_t indexT_j     = indexLibor_j;
			// tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]

			// cumulated_covariance_tensor = lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j);
			//volSquare += buffer_omega[i]*buffer_omega[j]*liborsInitValue[indexLibor_i]*liborsInitValue[indexLibor_j]*cumulated_covariance_tensor; 

			double dS_dLi = buffer_dSdLi_[indexLibor_i];
			double dS_dLj = buffer_dSdLi_[indexLibor_j];
			double cumulated_covariance  = lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j); //L_i, L_j 's integral of vol in [T_0,T_k], T_k =swaptionIndexMaturity
			double phi_i = buffer_liborAtExpensionInit_[indexLibor_i] + constShifts_[indexLibor_i]; 
			double phi_j = buffer_liborAtExpensionInit_[indexLibor_j] + constShifts_[indexLibor_j];

			volSquare += dS_dLi*dS_dLj*phi_i*phi_j*cumulated_covariance; 
		}
	}

	double swapRate = swapRate_Analytical(vanillaSwap, liborsInitValue); // swapRate at time 0
	double volBlack = std::sqrt(volSquare/maturityTime) / swapRate;

	return volBlack;
}

//
//void LmmApproxVanillaSwaptionPricer_Piterbarg::accumulateShiftedSwapRateAndStrike(
//	double& out_shifted_swapRate_T0 ,
//	double& out_shifted_strike      ,
//	const std::vector<double> & omega0         ,
//	const std::vector<double> & libor_shifts   ,
//	const std::vector<size_t> & floatingIndices)const
//{
//	//-- Compute shifted swap rate and shifted strike 
//	double swap_shift = 0.0;
//
//	for (auto i : floatingIndices)
//		swap_shift +=  omega0[i-1]*libor_shifts[i-1]; //-- Check the index
//
//	out_shifted_swapRate_T0 += swap_shift;
//	out_shifted_strike += swap_shift;	
//}

//! pre-calculation
void LmmVanillaSwaptionApproxPricer_Piterbarg :: preCalculateLiborAtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
{
	assert(buffer_liborAtExpensionInit_.size() == liborsInitValue.size());
	for(size_t i=0; i<liborsInitValue.size(); ++i)
		buffer_liborAtExpensionInit_[i] = liborsInitValue[i];
}

void LmmVanillaSwaptionApproxPricer_Piterbarg :: preCalculate_dSdLi_AtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
{
	//YY TODO: to delete this line latter, this is only for precaution
	for(size_t liborIndex=0; liborIndex < buffer_dSdLi_.size(); ++liborIndex) 
	{
		buffer_dSdLi_[liborIndex] = -1.0e10; 
	}

	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();

	//for(size_t itr=0; itr < floatingLegPaymentIndexSchedule.size(); ++itr)  // other derivative don't calculate ... 
	for(size_t itr=1; itr < buffer_dSdLi_.size(); ++itr)  // calculate more for MP
	{
		LMM::Index paymentIndex = itr;
		LMM::Index liborIndex   = paymentIndex -1;
		buffer_dSdLi_[liborIndex] = swapRate_1stDerivative(liborIndex);
	}
}

void LmmVanillaSwaptionApproxPricer_Piterbarg :: preCalculateALL(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
{
	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();

	assert(lmm_->get_horizon() >= vanillaSwap.get_indexEnd()); //! if not cannot price it.
	assert(lmm_->get_LMMTenorStructure()->get_tenorType() == vanillaSwap.get_lmmTenorStructureTenorType() );

	buffer_floatingLegPaymentIndexSchedule_ = vanillaSwap.get_floatingLegPaymentIndexSchedule();
	buffer_fixedLegPaymentIndexSchedule_    = vanillaSwap.get_fixedLegPaymentIndexSchedule();
	buffer_deltaTFloatingLeg_               = vanillaSwap.get_DeltaTFloatLeg();
	buffer_deltaTFixedLeg_                  = vanillaSwap.get_DeltaTFixedLeg();
	buffer_swapStartIndex_                  = vanillaSwap.get_indexStart();
	buffer_swapEndIndex_                    = vanillaSwap.get_indexEnd();

	preCalculateLiborAtExpensionInit(liborsInitValue, vanillaswaption);
	preCalculate_dSdLi_AtExpensionInit(liborsInitValue, vanillaswaption);

	assert(buffer_swapStartIndex_ < buffer_swapEndIndex_) ;
}


//! ----- ---- ---- ----  ----- ---- ---- ---- 
//
//           FwdZCInverse_Derivable : OK
//
//! ----- ---- ---- ----  ----- ---- ---- ---- 

double LmmVanillaSwaptionApproxPricer_Piterbarg::fwdZCInverse(size_t liborIndex) const //fwdZCInversed: P^{-1}(t,T_k,T_{k+1}) = 1 + delta_k * L_k(t)
{
	double deltaT = lmm_->get_LMMTenorStructure()->get_deltaT(liborIndex);
	return (1+ deltaT*buffer_liborAtExpensionInit_[liborIndex]);
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::fwdZCInverse(size_t liborIndex_begin, size_t liborIndex_end) const
{
	// check latter : 
	assert(liborIndex_begin <= liborIndex_end); 
	double result = 1.0; 
	for(size_t liborIndex = liborIndex_begin; liborIndex<liborIndex_end; ++liborIndex) // if (liborIndex_begin == liborIndex_end) return 1;
	{
		result *= fwdZCInverse(liborIndex);
	}
	return result;
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::fwdZCInverse_1stDerivative(size_t liborIndex_begin, size_t liborIndex_end, size_t liborIndex_derivative) const
{
	assert(liborIndex_begin <= liborIndex_end); 
	if(liborIndex_derivative < liborIndex_begin || liborIndex_derivative > liborIndex_end-1)
		return 0.0;

	return  fwdZCInverse(liborIndex_begin, liborIndex_end) * deltaMultiplyfwdZC(liborIndex_derivative);
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::fwdZCInverse_2ndDerivative(size_t liborIndex_begin, size_t liborIndex_end, size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const
{
	assert(liborIndex_begin <= liborIndex_end); 
	if( liborIndex_derivative_k < liborIndex_begin || liborIndex_derivative_k > liborIndex_end-1
	  ||liborIndex_derivative_l < liborIndex_begin || liborIndex_derivative_l > liborIndex_end-1
	  ||liborIndex_derivative_k == liborIndex_derivative_l )
	  return 0.0;
	return fwdZCInverse(liborIndex_begin, liborIndex_end) * ( deltaMultiplyfwdZC(liborIndex_derivative_k) * deltaMultiplyfwdZC(liborIndex_derivative_l) );
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::deltaMultiplyfwdZC(size_t liborIndex) const 
{
	double  deltaT  = lmm_->get_LMMTenorStructure()->get_deltaT(liborIndex);
	return  deltaT / fwdZCInverse(liborIndex);
}


//! ----- ---- ---- ----  ----- ---- ---- ---- 
//
//           swapRateNumerator_Derivable: OK 
//
//! ----- ---- ---- ----  ----- ---- ---- ---- 
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRateNumerator() const // Numerator = ( P(t,T_p) - P(t,T_q) ) / P(t,T_q) = P(t,T_p,T_q)-1
{
	return fwdZCInverse(buffer_swapStartIndex_, buffer_swapEndIndex_) - 1.0;
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRateNumerator_1stDerivative(size_t liborIndex_derivative) const
{
    return fwdZCInverse_1stDerivative(buffer_swapStartIndex_, buffer_swapEndIndex_, liborIndex_derivative);
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRateNumerator_2ndDerivative(size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const
{
	return fwdZCInverse_2ndDerivative(buffer_swapStartIndex_, buffer_swapEndIndex_, liborIndex_derivative_k, liborIndex_derivative_l);
}

//! ----- ---- ---- ----  ----- ---- ---- ---- 
//
//           swapRateDenominator_Derivable : OK ---- TODO, more optimization can be done latter: for avoiding calling un-necessary derivative !
//
//! ----- ---- ---- ----  ----- ---- ---- ---- 
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRateDenominator() const // Denominator = \sum delta_k P^{-1}(t,.,.)
{
	double result = 0;
	for(size_t i=0; i<buffer_fixedLegPaymentIndexSchedule_.size(); ++i)
	{
		LMM::Index	fixedLegPaymentIndex = buffer_fixedLegPaymentIndexSchedule_[i];
		result += buffer_deltaTFixedLeg_[i]*fwdZCInverse(fixedLegPaymentIndex, buffer_swapEndIndex_);
	}
	return result;
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRateDenominator_1stDerivative(size_t liborIndex_derivative) const
{
	double result = 0;
	for(size_t i=0; i<buffer_fixedLegPaymentIndexSchedule_.size(); ++i)
	{
		LMM::Index	fixedLegPaymentIndex = buffer_fixedLegPaymentIndexSchedule_[i];
		result += buffer_deltaTFixedLeg_[i]*fwdZCInverse_1stDerivative(fixedLegPaymentIndex, buffer_swapEndIndex_, liborIndex_derivative);
	}
	return result;
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRateDenominator_2ndDerivative(size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const
{
	double result = 0;
	for(size_t i=0; i<buffer_fixedLegPaymentIndexSchedule_.size(); ++i)
	{
		LMM::Index	fixedLegPaymentIndex = buffer_fixedLegPaymentIndexSchedule_[i];
		result += buffer_deltaTFixedLeg_[i]*fwdZCInverse_2ndDerivative(fixedLegPaymentIndex, buffer_swapEndIndex_, liborIndex_derivative_k, liborIndex_derivative_l);
	}
	return result;
}

//! ----- ---- ---- ----  ----- ---- ---- ---- 
//
//           swapRate_Derivable: OK
//
//! ----- ---- ---- ----  ----- ---- ---- ---- 
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRate() const
{
	double n      = swapRateNumerator();
	double d      = swapRateDenominator();
	return n/d;
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRate_1stDerivative(size_t liborIndex_derivative) const
{
	double n   = swapRateNumerator();
	double n_1 = swapRateNumerator_1stDerivative(liborIndex_derivative); 

	double d   = swapRateDenominator();
	double d_1 = swapRateDenominator_1stDerivative(liborIndex_derivative);

	return (n_1*d - d_1*n)/(d*d);
}
double LmmVanillaSwaptionApproxPricer_Piterbarg::swapRate_2ndDerivative(size_t liborIndex_derivative_k, size_t liborIndex_derivative_l) const
{
	double n     = swapRateNumerator();
	double n_1_k = swapRateNumerator_1stDerivative(liborIndex_derivative_k); 
	double n_1_l = swapRateNumerator_1stDerivative(liborIndex_derivative_l); 
	double n_2   = swapRateNumerator_2ndDerivative(liborIndex_derivative_k,liborIndex_derivative_l); 

	double d     = swapRateDenominator();
	double d_1_k = swapRateDenominator_1stDerivative(liborIndex_derivative_k);
	double d_1_l = swapRateDenominator_1stDerivative(liborIndex_derivative_l);
	double d_2   = swapRateDenominator_2ndDerivative(liborIndex_derivative_k, liborIndex_derivative_l);

	double result = ( (n_1_k*d_1_l + n_2*d) - (d_1_k*n_1_l + d_2*n) ) *d*d;
	result  -= 2*d*d_1_l*(n_1_k*d-d_1_k*n);
	result /= (d*d*d*d);

	return result;
}

