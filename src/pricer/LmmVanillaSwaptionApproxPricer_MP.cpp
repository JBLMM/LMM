//#include <cassert>
//#include <vector>
//
//#include <LMM/numeric/NumericalMethods.h>
//#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_MP.h>
//
//LmmVanillaSwaptionApproxPricer_MP::LmmVanillaSwaptionApproxPricer_MP(const Lmm_PTR& lmmModel)
//	: LmmVanillaSwaptionApproxPricer_Piterbarg(lmmModel)
//	, B_(lmm_->get_dispersionRef().get_Correlation_PTR()->get_reducedCorrelMatrixB())
//	, nbFactor_(lmm_->get_dispersionRef().getNbFactors())
//{}
// 
//////! not real Piterbarg model, only shifted LMM
//std::vector<double> LmmVanillaSwaptionApproxPricer_MP :: phi(size_t liborIndex, double t) const // expansion at: L_i(t)^*
//{
//	double liborValue = buffer_liborAtExpensionInit_[liborIndex]; // x
//	double shift      = lmm_->get_dispersionRef().??(liborIndex, t);
//	double vol        = lmm_->get_dispersionRef().get_VolatilityFunction_PTR.get_varSigma_timeContinuousVersion(liborIndex,t);
//	double tempValue  = vol * (liborValue + shift);
//
//	std::vector<double> result(nbFactor_);        // YY TODO: ugly code, because of the limited capacity of QL matrix.
//	for(size_t k=0; k<result.size(); ++k)
//	{
//		result[k] = tempValue*B_[liborIndex][k];  // YY TODO: in the future, B_ will be a function of time, to change it latter.
//	}
//
//	return result;
//}
//
////std::vector<double> LmmVanillaSwaptionApproxPricer_MP :: phi_1stDerivative(size_t liborIndex, size_t liborIndex_derivative, double t) const
////{
////	double vol    = lmm_->get_dispersionRef().get_vol(t, liborIndex);
////
////	std::vector<double> result(nbFactor_); // YY TODO: ugly code, because of the limited capacity of QL matrix.
////	for(size_t k=0; k<result.size(); ++k)
////	{
////		result[k] = vol*B_[liborIndex][k];  // YY TODO: in the future, B_ will be a function of time, to change it latter.
////	}
////
////	return result;
////}
////
//////! YY:: TODO: not efficient at all, need to use cache and precalcualtion latter.
//////!     return a copy of vector<double>, not efficient
////std::vector<double> LmmVanillaSwaptionApproxPricer_MP :: psi(size_t liborIndex, const VanillaSwaption& vanillaswaption, double t) const // expansion at: L_i(t)^*
////{
////	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
////
////	LMM::Index swaptionIndexMaturity = vanillaswaption.get_indexMaturity();
////	double maturityTime  = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);
////	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
////
////	std::vector<double> result(nbFactor_, 0.0);
////	for(size_t i=0; i<floatingLegPaymentIndexSchedule.size(); ++i)
////	{
////		size_t floatingLegPaymentIndex_i = floatingLegPaymentIndexSchedule[i]; // = i+1
////		size_t indexLibor_i = floatingLegPaymentIndex_i - 1;
////
////		double dS_dLi = buffer_dSdLi_[indexLibor_i];        // independent of t !
////		std::vector<double> phi_i  = phi(t, indexLibor_i);  // depend on t
////		
////		for(size_t k=0; k<result.size(); ++k)
////		{
////			result[k] = dS_dLi*phi_i[k];
////		}
////	}
////	return result;
////}
//
////! not efficient at all, need to do precalculation, esp for <vol_i, vol_j>
//double LmmVanillaSwaptionApproxPricer_MP :: swapVol(const VanillaSwaption& vanillaswaption, double t) const
//{
//	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
//
//	LMM::Index swaptionIndexMaturity = vanillaswaption.get_indexMaturity();
//	double maturityTime  = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);
//	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
//
//	double volSquare = 0.0;
//	std::vector<double> phi_i(nbFactor_);
//	std::vector<double> phi_j(nbFactor_);
//
//	for(size_t i=0; i<floatingLegPaymentIndexSchedule.size(); ++i)
//	{
//		size_t floatingLegPaymentIndex_i = floatingLegPaymentIndexSchedule[i]; // = i+1
//		size_t indexLibor_i = floatingLegPaymentIndex_i - 1;
//		//size_t indexT_i = indexLibor_i;
//		for(size_t j=0; j<floatingLegPaymentIndexSchedule.size(); ++j)
//		{
//			size_t floatingLegPaymentIndex_j = floatingLegPaymentIndexSchedule[j]; // = j+1
//			size_t indexLibor_j = floatingLegPaymentIndex_j - 1;
//			
//			double dS_dLi = buffer_dSdLi_[indexLibor_i];  // independent of t
//			double dS_dLj = buffer_dSdLi_[indexLibor_j];  // independent of t
//
//			phi_i  = phi(t, indexLibor_i);  // depend on t
//			phi_j  = phi(t, indexLibor_j);  // depend on t
//
//			volSquare += dS_dLi*dS_dLj*NumericalMethods::vectorProduct(phi_i,phi_j);
//		}
//	}
//    return std::sqrt(volSquare);
//}
//double LmmVanillaSwaptionApproxPricer_MP :: swapVol_1stDerivative(const VanillaSwaption& vanillaswaption, size_t liborIndex_derivative, t) const
//{
//	double swapVolValue = swapVol(t, vanillaswaption);
//	Need to calculat the Hessin matrix of dS/dLidLj in Piterbarg.cpp. ---- time to watch match :) ---- 
//}
//
//double LmmVanillaSwaptionApproxPricer_MP::price(const VanillaSwaption & swaption, const std::vector<double> & liborsInitValue )const 
//{
//	this->preCalculateALL(liborsInitValue  ,swaption);
//
//	const VanillaSwap & swap = swaption.getUnderlyingSwap();
//
//	double swaprate0  = this->swapRate_Analytical(swap, liborsInitValue) ;
//	double annuity0   = this->annuity0(swaption.getUnderlyingSwap(), liborsInitValue); // swapRate at time 0
//	double strike     = swap.get_strike() ;
//	double vol_approx = this->volBlack(swaption,liborsInitValue) ;
//	
//	LMM::Index swaptionIndexMaturity = swaption.get_indexMaturity();
//	const double & timeToMaturity = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);
//
//	double approxPrice = annuity0*NumericalMethods::Black_Price(swaprate0 ,strike , vol_approx , timeToMaturity) ;
//	//double approxPrice = NumericalMethods::Black_Price(swaprate0 ,strike , vol_approx , buffer_anuity)  ; //older code
//
//	return approxPrice;
//}
//
//double LmmVanillaSwaptionApproxPricer_MP::volBlack(const VanillaSwaption& vanillaswaption, const std::vector<double>& liborsInitValue) const
//{
//	this->preCalculateALL(liborsInitValue, vanillaswaption);
//
//	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
//
//	assert(lmm_->get_horizon() >= vanillaSwap.get_indexEnd()); //! if not cannot price it.
//	assert(lmm_->get_LMMTenorStructure()->get_tenorType() == vanillaSwap.get_lmmTenorStructureTenorType() );
//
//	//! Piterbarg Formula: YY TODO: can be simplified:
//	//  1. use the symmetric 
//	//  2. precalculate the matrix dSdLi vector
//	LMM::Index swaptionIndexMaturity = vanillaswaption.get_indexMaturity();
//	double maturityTime  = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);
//	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
//
//	double volSquare = 0.0;
//	for(size_t i=0; i<floatingLegPaymentIndexSchedule.size(); ++i)
//	{
//		size_t floatingLegPaymentIndex_i = floatingLegPaymentIndexSchedule[i]; // = i+1
//		size_t indexLibor_i = floatingLegPaymentIndex_i - 1;
//		//size_t indexT_i = indexLibor_i;
//		for(size_t j=0; j<floatingLegPaymentIndexSchedule.size(); ++j)
//		{
//			size_t floatingLegPaymentIndex_j = floatingLegPaymentIndexSchedule[j]; // = j+1
//			size_t indexLibor_j = floatingLegPaymentIndex_j - 1;
//
//			//size_t indexT_j     = indexLibor_j;
//			// tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]
//
//			// cumulated_covariance_tensor = lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j);
//			//volSquare += buffer_omega[i]*buffer_omega[j]*liborsInitValue[indexLibor_i]*liborsInitValue[indexLibor_j]*cumulated_covariance_tensor; 
//
//			double dS_dLi = buffer_dSdLi_[indexLibor_i];
//			double dS_dLj = buffer_dSdLi_[indexLibor_j];
//			double cumulated_covariance  = lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j); //L_i, L_j 's integral of vol in [T_0,T_k], T_k =swaptionIndexMaturity
//			double phi_i = buffer_liborAtExpensionInit_[indexLibor_i] + lmm_->get_shift(indexLibor_i); 
//			double phi_j = buffer_liborAtExpensionInit_[indexLibor_j] + lmm_->get_shift(indexLibor_j);
//
//			volSquare += dS_dLi*dS_dLj*phi_i*phi_j*cumulated_covariance; 
//		}
//	}
//
//	double swapRate = swapRate_Analytical(vanillaSwap, liborsInitValue); // swapRate at time 0
//	double volBlack = std::sqrt(volSquare/maturityTime) / swapRate;
//
//	return volBlack;
//}
