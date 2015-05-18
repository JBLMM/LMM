#include "CheyetteVanillaSwaptionApproxPricer_Piterbarg.h"
#include "LMM/numeric/NumericalMethods.h"

#include <cassert>
#include <vector>
#include <boost/pointer_cast.hpp>


CheyetteVanillaSwaptionApproxPricer_Piterbarg::CheyetteVanillaSwaptionApproxPricer_Piterbarg
						(const Cheyette_PTR& CheyetteModel, 
						const VanillaSwaption_PTR& Swaption, 
						const double t, const double y_bar, const double ExpansionPoint_s,
						const double s_bar, const double Phi_t_s_bar)
{
	// domaine de validité de l'approximation :
	//assert(lmm_->get_dispersionRef().get_VolatilityFunction_PTR()->isConstShift()); 
	Cheyette_			= CheyetteModel ;
	Swaption_			= Swaption ;
	t_					= t ;
	y_bar_				= y_bar ;
	ExpansionPoint_s_	= ExpansionPoint_s ;

	s_bar_				= s_bar ;       
	Phi_t_s_bar_		= Phi_t_s_bar ;

	double buffer_T0_  = Swaption_->getUnderlyingSwap().get_StartDate() ;		//1ere date de fixing
	double buffer_TN_  = Swaption_->getUnderlyingSwap().get_EndDate() ;			//date dernier flux (setté en T_{N-1})

	VanillaSwap buffer_UnderlyingSwap_							= Swaption_->getUnderlyingSwap() ;
	std::vector<LMM::Index> buffer_fixedLegPaymentIndexSchedule_ = buffer_UnderlyingSwap_.get_fixedLegPaymentIndexSchedule() ;
	std::vector<double> buffer_deltaTFixedLeg_					= buffer_UnderlyingSwap_.get_DeltaTFixedLeg() ;
}


double CheyetteVanillaSwaptionApproxPricer_Piterbarg::ZC_1stDerivative
					(double t, double T, 
					double x_t, double y_t) const
{
	assert(t >= 0); assert(0 <= T && T <= buffer_TN_) ;
	double ZC = Cheyette_->P(t, T, x_t, y_t) ;
	return (- Cheyette_->G(t,T) * ZC ) ;
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::ZC_2ndDerivative(double t, double T, double x_t, double y_t) const
{
	assert(t >= 0); assert(0 <= T && T <= buffer_TN_) ;
	double ZC = Cheyette_->P(t, T, x_t, y_t) ;
	double g = Cheyette_->G(t,T) ;
	return (g * g * ZC ) ;
}

// Numerator = P(t, T0) - P(t, TN)
double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateNumerator(double x_t, double y_t) const 
{
	double ZC_T0 = Cheyette_->P(t_, buffer_T0_, x_t, y_t) ;
	double ZC_TN = Cheyette_->P(t_, buffer_TN_, x_t, y_t) ;
	
	return (ZC_T0 - ZC_TN) ; 
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateNumerator_1stDerivative(double x_t, double y_t) const
{
	return ( ZC_1stDerivative(t_, buffer_T0_, x_t, y_t) - ZC_1stDerivative(t_, buffer_TN_, x_t, y_t) ) ;
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateNumerator_2ndDerivative(double x_t, double y_t) const
{
	return ( ZC_2ndDerivative(t_, buffer_T0_, x_t, y_t) - ZC_2ndDerivative(t_, buffer_TN_, x_t, y_t) ) ;
}

// Denominator = \sum delta_k P(t,T_k)
double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateDenominator(double x_t, double y_t) const 
{
	return Cheyette_->annuity(t_, buffer_UnderlyingSwap_, x_t, y_t) ;
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateDenominator_1stDerivative(double x_t, double y_t) const
{
	double result = 0;

	for(size_t i=0; i<buffer_fixedLegPaymentIndexSchedule_.size(); ++i)
	{
		size_t	fixedLegPaymentIndex = buffer_fixedLegPaymentIndexSchedule_[i];
		result += buffer_deltaTFixedLeg_[i] * ZC_1stDerivative(t_, fixedLegPaymentIndex, x_t, y_t) ;	
	}
	return result;
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateDenominator_2ndDerivative(double x_t, double y_t) const
{
	double result = 0;

	for(size_t i=0; i<buffer_fixedLegPaymentIndexSchedule_.size(); ++i)
	{
		size_t	fixedLegPaymentIndex = buffer_fixedLegPaymentIndexSchedule_[i];
		result += buffer_deltaTFixedLeg_[i] * ZC_2ndDerivative(t_, fixedLegPaymentIndex, x_t, y_t) ;		
	}
	return result;
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRate(double x_t, double y_t) const
{
	double n      = swapRateNumerator(x_t, y_t);
	double d      = swapRateDenominator(x_t, y_t);
	return n/d;
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRate_1stDerivative(double x_t, double y_t) const
{
	double n   = swapRateNumerator(x_t, y_t);
	double n_1 = swapRateNumerator_1stDerivative(x_t, y_t); 

	double d   = swapRateDenominator(x_t, y_t);
	double d_1 = swapRateDenominator_1stDerivative(x_t, y_t);

	return (n_1*d - n*d_1)/(d*d);
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRate_2ndDerivative(double x_t, double y_t) const
{
	double n	= swapRateNumerator(x_t, y_t);
	double n_1	= swapRateNumerator_1stDerivative(x_t, y_t); 
	double n_2	= swapRateNumerator_2ndDerivative(x_t, y_t); 

	double d	= swapRateDenominator(x_t, y_t);
	double d_1	= swapRateDenominator_1stDerivative(x_t, y_t);
	double d_2	= swapRateDenominator_2ndDerivative(x_t, y_t);

	double result = (n_2 * d - n * d_2) * d*d - (n_1 * d - n * d_1) * 2 * d * d_1 ;
	result /= (d*d*d*d);

	return result;
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateVolatility_1stDerivative(double s) const
{
	// calcul de l'inverse \Chi_t(S_t)
	std::vector<double> tableauX ;
	for (int i = 0; i < 1000 ; ++i){tableauX.push_back(i/10) ;}
	InverseFunction i = InverseFunction(tableauX) ;
	double inverse = i.fMoinsUn(s) ; 

	CheyetteModel::Cheyette_Parameter parameter = this->Cheyette_->get_Cheyette_Parameter() ;
	piecewiseconst_DD_R2R_Function sigma = parameter.sigma_ ;
	
	return (sigma.evaluate(t_ , inverse) * swapRate_2ndDerivative(inverse, y_bar_) / swapRate_1stDerivative(inverse, y_bar_) 
					+ sigma.d_sigma_r_dx(t_) );
}

double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateVolatility_approx_lineaire(double s) const
{
	return ( Phi_t_s_bar_ + swapRateVolatility_1stDerivative(s) * (s - s_bar_) ) ;
}


/****************  parameter averaging  *********************
*
*	dX(t) = \lambda(t) ( (1-b(t)) X0 + b(t) X(t) ) dW(t)
*	dY(t) = \lambda(t) ( (1- b )  X0  +  b   X(t) ) dW(t)
*
*	dS(t) = (A(t) + B(t) S(t)) dW^Q_A
************************************************************/

//A(t) 
double CheyetteVanillaSwaptionApproxPricer_Piterbarg::A(double t) const
{
	return Phi_t_s_bar_ - swapRateVolatility_1stDerivative(t) * s_bar_  ;
}

//B(t) 
double CheyetteVanillaSwaptionApproxPricer_Piterbarg::B(double t) const
{
	return swapRateVolatility_1stDerivative(t) ;
}

//b(t) 
double CheyetteVanillaSwaptionApproxPricer_Piterbarg::b(double t)
{
	return  A(t) ;
}	

//lambda(t) AU CARRE
double lambda2(double t) 
{
	double lambda_t =  ;
	return  lambda_t * lambda_t ;
}

double v2(double t){return (NumericalMethods::qgaus((*lambda2), 0,t)) ;}

double numerateur(double t){return lambda2(t) * v2(t) * b(t) ;}
double denom(double t){return lambda2(t) * v2(t) ;}

//retourne b_barre
//average over [0, t]
double CheyetteVanillaSwaptionApproxPricer_Piterbarg::timeAverage(double t) const
{
	double integrale_numerateur	= NumericalMethods::qgaus((*numerateur), 0, t) ;
	double integrale_denom		= NumericalMethods::qgaus((*denom), 0, t) ;
	return integrale_numerateur/integrale_denom ;

}





/******************************************
		Dirkmann approximation
		**************************/


double CheyetteVanillaSwaptionApproxPricer_Piterbarg::prixSwaption_Dirkmann(double K, 
																			double t_valo, 
																			double x_t, 
																			double y_t)
{
	double prixSwaption(0) ;

	double m			= Cheyette_->get_Cheyette_Parameter().sigma_.get_m().evaluate(0) ;
	double L			= Cheyette_->get_Cheyette_Parameter().sigma_.getL() ;
	
	VanillaSwap swap	= Swaption_->getUnderlyingSwap() ;
	double annuite		= Cheyette_->annuity(t_valo, swap, x_t, y_t) ;

	double S_tilde_0N_t = m * annuite + (1- m) * L ;
	double K_tilde		= (1-m) * L + m * K ;

	//exercise date for the swaption

	//  !!!!!!!!!!!!!!!!!!   verifier que date exercice swaption = get_Indexstart          !!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	double T0 = Swaption_->getUnderlyingSwap().get_indexStart() ; 
	
	//  !!!!!!!!!!!!!!!!!!   definir fonction d'intégration          !!!!!!!!!!!!!!!!!!!!!!!!!!!

	double vol2 ;
	
	//prixSwaption = annuite/m * NumericalMethods::Black_Price_vol2(S_tilde_0N_t, K_tilde, vol2, T0) ;
	
	return prixSwaption ;
}







//double CheyetteVanillaSwaptionApproxPricer_Piterbarg::price(const VanillaSwaption & swaption, const std::vector<double> & liborsInitValue )const 
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
//double CheyetteVanillaSwaptionApproxPricer_Piterbarg::volBlack(const VanillaSwaption& vanillaswaption, const std::vector<double>& liborsInitValue) const
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
//			double phi_i = buffer_liborAtExpensionInit_[indexLibor_i] + constShifts_[indexLibor_i]; 
//			double phi_j = buffer_liborAtExpensionInit_[indexLibor_j] + constShifts_[indexLibor_j];
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
//
////
////void LmmApproxVanillaSwaptionPricer_Piterbarg::accumulateShiftedSwapRateAndStrike(
////	double& out_shifted_swapRate_T0 ,
////	double& out_shifted_strike      ,
////	const std::vector<double> & omega0         ,
////	const std::vector<double> & libor_shifts   ,
////	const std::vector<size_t> & floatingIndices)const
////{
////	//-- Compute shifted swap rate and shifted strike 
////	double swap_shift = 0.0;
////
////	for (auto i : floatingIndices)
////		swap_shift +=  omega0[i-1]*libor_shifts[i-1]; //-- Check the index
////
////	out_shifted_swapRate_T0 += swap_shift;
////	out_shifted_strike += swap_shift;	
////}
//
////! pre-calculation
//void CheyetteVanillaSwaptionApproxPricer_Piterbarg :: preCalculateLiborAtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
//{
//	assert(buffer_liborAtExpensionInit_.size() == liborsInitValue.size());
//	for(size_t i=0; i<liborsInitValue.size(); ++i)
//		buffer_liborAtExpensionInit_[i] = liborsInitValue[i];
//}
//
//void CheyetteVanillaSwaptionApproxPricer_Piterbarg :: preCalculate_dSdLi_AtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
//{
//	//YY TODO: to delete this line latter, this is only for precaution
//	for(size_t liborIndex=0; liborIndex < buffer_dSdLi_.size(); ++liborIndex) 
//	{
//		buffer_dSdLi_[liborIndex] = -1.0e10; 
//	}
//
//	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
//	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
//
//	//for(size_t itr=0; itr < floatingLegPaymentIndexSchedule.size(); ++itr)  // other derivative don't calculate ... 
//	for(size_t itr=1; itr < buffer_dSdLi_.size(); ++itr)  // calculate more for MP
//	{
//		LMM::Index paymentIndex = itr;
//		LMM::Index liborIndex   = paymentIndex -1;
//		buffer_dSdLi_[liborIndex] = swapRate_1stDerivative(liborIndex);
//	}
//}
//
//void CheyetteVanillaSwaptionApproxPricer_Piterbarg :: preCalculateALL(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
//{
//	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
//
//	assert(lmm_->get_horizon() >= vanillaSwap.get_indexEnd()); //! if not cannot price it.
//	assert(lmm_->get_LMMTenorStructure()->get_tenorType() == vanillaSwap.get_lmmTenorStructureTenorType() );
//
//	buffer_floatingLegPaymentIndexSchedule_ = vanillaSwap.get_floatingLegPaymentIndexSchedule();
//	buffer_fixedLegPaymentIndexSchedule_    = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//	buffer_deltaTFloatingLeg_               = vanillaSwap.get_DeltaTFloatLeg();
//	buffer_deltaTFixedLeg_                  = vanillaSwap.get_DeltaTFixedLeg();
//	buffer_swapStartIndex_                  = vanillaSwap.get_indexStart();
//	buffer_swapEndIndex_                    = vanillaSwap.get_indexEnd();
//
//	preCalculateLiborAtExpensionInit(liborsInitValue, vanillaswaption);
//	preCalculate_dSdLi_AtExpensionInit(liborsInitValue, vanillaswaption);
//
//	assert(buffer_swapStartIndex_ < buffer_swapEndIndex_) ;
//}
//


