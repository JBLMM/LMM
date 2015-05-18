#include "CheyetteDD_VanillaSwaptionApproxPricer.h"
#include "LMM/numeric/NumericalMethods.h"

#include <cassert>
#include <vector>
#include <iostream> 
#include <boost/pointer_cast.hpp>



CheyetteDD_VanillaSwaptionApproxPricer::CheyetteDD_VanillaSwaptionApproxPricer
						(const CheyetteDD_Model_PTR&	cheyetteModel, 
						const VanillaSwaption_PTR&		swaption)
{
	// domaine de validité de l'approximation :
	//assert(lmm_->get_dispersionRef().get_VolatilityFunction_PTR()->isConstShift()); 
	cheyetteDD_Model_	= cheyetteModel ;
	swaption_			= swaption ;

	//pour S0 : 
	//remplir en faisant appel au fichier input de calibration avec données de marché

	double buffer_T0_  = swaption_->getUnderlyingSwap().get_StartDate() ;		//1ere date de fixing
	double buffer_TN_  = swaption_->getUnderlyingSwap().get_EndDate() ;			//date dernier flux (setté en T_{N-1})

	VanillaSwap buffer_UnderlyingSwap_							= swaption_->getUnderlyingSwap() ;
	std::vector<LMM::Index> buffer_fixedLegPaymentIndexSchedule_ = buffer_UnderlyingSwap_.get_fixedLegPaymentIndexSchedule() ;
	std::vector<double> buffer_deltaTFixedLeg_					= buffer_UnderlyingSwap_.get_DeltaTFixedLeg() ;

	courbeInput_PTR buffer_courbeInput_ = cheyetteDD_Model_->get_courbeInput_PTR() ;
}


double CheyetteDD_VanillaSwaptionApproxPricer::calculate_y_bar(double t) const
{
	//coder int_0^t sigma_r^0(s)^2 exp(- 2k (t-s) ) ds
	double k = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().k_  ;
	//les abscisses et ordonnees de la fonction sigma
	std::vector<double> x = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().sigma_.getx_() ;
	std::vector<double> y = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().sigma_.gety_() ;
	double integrale(0) ;
	for (int i = 0 ; i < x.size() - 2 ; ++i)
	{
		integrale += y[i] * y[i] * (exp(2 * k * x[i+1]) - exp(2 * k * x[i])) / (2 * k);
	}
	return exp(- 2 * k * t) * integrale ; 
}

////evaluee en t=0 et pour un taux de swap s_bar = s0_
////double CheyetteDD_VanillaSwaptionApproxPricer::calculate_phi_0_s_bar() const
////{
////	TODO
////	std::cout << "fonction Phi_t_s_barre(t) pas encore codee" << std::endl ;
////	coder dS(t)/dx(t) sigma_r(t) (t, inverse, y_bar)
////	
////	return 0 ;
////}


//double CheyetteDD_VanillaSwaptionApproxPricer::ZC_1stDerivative_on_xt(double T) const
//{
//	assert(0 <= T && T <= buffer_TN_) ;
//	//double ZC = cheyetteDD_Model_->P(t, T, x_t, y_t) ;
//	double P_0_T = buffer_courbeInput_->get_ZC0(T) ;
//	return (- cheyetteDD_Model_->G(0,T) * P_0_T ) ;
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::ZC_2ndDerivative_on_xt(double T) const
//{
//	assert(0 <= T && T <= buffer_TN_) ;
//	//double ZC = Cheyette_->P(t, T, x_t, y_t) ;
//	double P_0_T = buffer_courbeInput_->get_ZC0(T) ;
//	double g = cheyetteDD_Model_->G(0,T) ;
//	return (g * g * P_0_T ) ;
//}
//
//// Numerator = P(t, T0) - P(t, TN)
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRateNumerator(double t) const 
//{
//	//double ZC_T0 = cheyetteDD_Model_->P(t_, buffer_T0_, x_t, y_t) ;
//	//double ZC_TN = cheyetteDD_Model_->P(t_, buffer_TN_, x_t, y_t) ;
//	double ZC_T0 = buffer_courbeInput_->get_ZC0(buffer_T0_) ;
//	double ZC_TN = buffer_courbeInput_->get_ZC0(buffer_TN_) ;
//	return (ZC_T0 - ZC_TN) ; 
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRateNumerator_1stDerivative(double t) const
//{
//	return ( ZC_1stDerivative_on_xt(buffer_T0_) - ZC_1stDerivative_on_xt(buffer_TN_) ) ;
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRateNumerator_2ndDerivative(double t) const
//{
//	return ( ZC_2ndDerivative_on_xt(buffer_T0_) - ZC_2ndDerivative_on_xt(buffer_TN_) ) ;
//}
//
//// Denominator = \sum delta_k P(t,T_k)
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRateDenominator(double t) const 
//{
//	return cheyetteDD_Model_->annuity(buffer_UnderlyingSwap_) ;
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRateDenominator_1stDerivative(double t) const
//{
//	double result = 0;
//
//	for(size_t i=0; i<buffer_fixedLegPaymentIndexSchedule_.size(); ++i)
//	{
//		size_t	fixedLegPaymentIndex = buffer_fixedLegPaymentIndexSchedule_[i];
//		result += buffer_deltaTFixedLeg_[i] * ZC_1stDerivative_on_xt(fixedLegPaymentIndex) ;	
//	}
//	return result;
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRateDenominator_2ndDerivative(double t) const
//{
//	double result = 0;
//
//	for(size_t i=0; i<buffer_fixedLegPaymentIndexSchedule_.size(); ++i)
//	{
//		size_t	fixedLegPaymentIndex = buffer_fixedLegPaymentIndexSchedule_[i];
//		result += buffer_deltaTFixedLeg_[i] * ZC_2ndDerivative_on_xt(fixedLegPaymentIndex) ;		
//	}
//	return result;
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRate(double t) const
//{
//	double n      = swapRateNumerator(x_t, y_t);
//	double d      = swapRateDenominator(x_t, y_t);
//	return n/d;
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRate_1stDerivative(double t) const
//{
//	double n   = swapRateNumerator(x_t, y_t);
//	double n_1 = swapRateNumerator_1stDerivative(x_t, y_t); 
//
//	double d   = swapRateDenominator(x_t, y_t);
//	double d_1 = swapRateDenominator_1stDerivative(x_t, y_t);
//
//	return (n_1*d - n*d_1)/(d*d);
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRate_2ndDerivative(double t) const
//{
//	double n	= swapRateNumerator(x_t, y_t);
//	double n_1	= swapRateNumerator_1stDerivative(x_t, y_t); 
//	double n_2	= swapRateNumerator_2ndDerivative(x_t, y_t); 
//
//	double d	= swapRateDenominator(x_t, y_t);
//	double d_1	= swapRateDenominator_1stDerivative(x_t, y_t);
//	double d_2	= swapRateDenominator_2ndDerivative(x_t, y_t);
//
//	double result = (n_2 * d - n * d_2) * d*d - (n_1 * d - n * d_1) * 2 * d * d_1 ;
//	result /= (d*d*d*d);
//
//	return result;
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::swapRateVolatility_1stDerivative(double t) const
//{
//	// calcul de l'inverse \Chi_t(S_t)
//	/*
//	std::vector<double> tableauX ;
//	for (int i = 0; i < 1000 ; ++i)
//	{
//		tableauX.push_back(i/10) ;
//	}
//	*/
//	//InverseFunction i = InverseFunction(tableauX) ;
//
//// A FINIR  et à decommenter 
//	//Newton-Raphson
//	//double xt_inverse_from_sBar = i.fMoinsUn(s) ;  // xt = inverse(\bar{s})
//
//	//CheyetteDD_Model::CheyetteDD_Parameter parameter = cheyetteDD_Model_->get_CheyetteDD_Parameter() ;
//	//piecewiseconst_DD_R2R_Function sigma = parameter.sigma_ ;
//	//
//	//return (sigma.evaluate(t_ , xt_inverse_from_sBar) * swapRate_2ndDerivative(xt_inverse_from_sBar, y_bar_) / swapRate_1stDerivative(xt_inverse_from_sBar, y_bar_) 
//	//				+ sigma.d_sigma_r_dx(t_) );
//
//	return 0 ;
//}
//
///*
//double CheyetteVanillaSwaptionApproxPricer_Piterbarg::swapRateVolatility_approx_lineaire(double t) const
//{
//	return ( Phi_t_s_bar_ + swapRateVolatility_1stDerivative(s) * (s - s_bar_) ) ;
//}
//*/
//
///****************  parameter averaging  *********************
//*
//*	dX(t) = \lambda(t) ( (1-b(t)) X0 + b(t) X(t) ) dW(t)
//*	dY(t) = \lambda(t) ( (1- b )  X0  +  b   X(t) ) dW(t)
//*
//*	dS(t) = (A(t) + B(t) S(t)) dW^Q_A
//************************************************************/
//
////avec fonctions friend
//////A(t) 
////double CheyetteVanillaSwaptionApproxPricer_Piterbarg::A(double t) 
////{
////	return Phi_t_s_bar_ - swapRateVolatility_1stDerivative(t) * s_bar_  ;
////}
////
//////B(t) 
////double CheyetteVanillaSwaptionApproxPricer_Piterbarg::B(double t) 
////{
////	return swapRateVolatility_1stDerivative(t) ;
////}
////
//////lambda(t) 
////double lambda(double t, CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR ch_PTR)
////{
////	double S0 = ch_PTR->get_Cheyette()->get_S0() ;
////	return  ch_PTR->B(t)/S0 + ch_PTR->A(t) ;
////}	
////
//////b(t) 
////double b(double t, CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR ch_PTR)
////{
////	//pas efficace, on recalcule lambda(t)
////		double S0		= ch_PTR->get_Cheyette()->get_S0() ;
////		double lambda_t = ch_PTR->B(t)/S0 + ch_PTR->A(t) ;
////	return  ch_PTR->A(t) / lambda_t ;
////}	
////
//////lambda(t) AU CARRE
////double lambda2(double t, CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR ch_PTR)
////{
////	//pas efficace, on recalcule lambda(t)
////		double S0		= ch_PTR->get_Cheyette()->get_S0() ;
////		double lambda_t = ch_PTR->B(t)/S0 + ch_PTR->A(t) ;
////	return  lambda_t * lambda_t ;
////}
////
////double v2(double t, CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR ch_PTR)
////{
////	return (NumericalMethods::qgaus(*(lambda2), 0,t)) ;
////}
////
////double numerateur(double t)
////{
////	return lambda2(t) * v2(t) * b(t) ;
////}
////double denom(double t)
////{
////	return lambda2(t) * v2(t) ;
////}
////
//////retourne b_barre
//////average over [0, t]
////double CheyetteVanillaSwaptionApproxPricer_Piterbarg::timeAverage(double t) const
////{
////	double (*ptrNumerateur)(double) = &numerateur;
////	double integrale_numerateur	= NumericalMethods::qgaus(ptrNumerateur, 0, t) ;
////
////	double (*ptrDenom)(double) = &denom;
////	double integrale_denom	= NumericalMethods::qgaus(ptrDenom, 0, t) ;
////
////	return integrale_numerateur/integrale_denom ;
////}
//
////POINTEURS SUR FONCTIONS MEMBRES PLUS COMPLIQUE :
////formulation 1 du displaced diffusion : fonctions A(t) et B(t)
////A(t) 
//double CheyetteDD_VanillaSwaptionApproxPricer::A(double t) const
//{
//	return Phi_t_s_bar_ - swapRateVolatility_1stDerivative(t) * s_bar_  ;
//}
//
////B(t) 
//double CheyetteDD_VanillaSwaptionApproxPricer::B(double t) const
//{
//	return swapRateVolatility_1stDerivative(t) ;
//}
//
////formulation 2 du displaced diffusion : fonctions lambda(t) et b(t)
////lambda(t) 
//double CheyetteDD_VanillaSwaptionApproxPricer::lambda(double t) const
//{
//	double S0 = get_Cheyette()->get_S0() ;
//	return  B(t)/S0 + A(t) ;
//}	
//
////b(t) 
//double CheyetteDD_VanillaSwaptionApproxPricer::b(double t) const
//{
//	return  A(t)/lambda(t) ;
//}	
//
////lambda(t) AU CARRE
///*
//double CheyetteVanillaSwaptionApproxPricer_Piterbarg::lambda2(double t)
//{
//	double l = lambda(t) ;
//	return  l * l ;
//}
//*/
//
////Les membres des classes ne peuvent pas être manipulés sans passer par la classe à laquelle ils appartiennent. 
////Par conséquent, il faut, lorsqu'on veut faire un pointeur sur un membre, indiquer le nom de sa classe. 
////Pour cela, la syntaxe suivante est utilisée :
////définition classe::* pointeur pour la déclaration du type
////(this->*cb_func)(); // Call function via pointer to current instance.
//double CheyetteDD_VanillaSwaptionApproxPricer::v2(double t)
//{
//	double (CheyetteVanillaSwaptionApproxPricer_Piterbarg::*ptrlambda2)(double) = 
//								&CheyetteVanillaSwaptionApproxPricer_Piterbarg::lambda2;
//
//	//return NumericalMethods::qgaus(ptrlambda2, 0, t) ;    //TODO pointeur sur fonction membre
//	return 1 ;
//}
//
//double CheyetteDD_VanillaSwaptionApproxPricer::timeAveraging_b_numerateur(double t)
//{
//	return lambda2(t) * v2(t) * b(t) ;
//}
//double CheyetteDD_VanillaSwaptionApproxPricer::timeAveraging_b_denom(double t)
//{
//	return lambda2(t) * v2(t) ;
//}
//
////retourne b_barre
////average over [0, t]
//double CheyetteDD_VanillaSwaptionApproxPricer::timeAverage(double t)
//{
//	//double (CheyetteVanillaSwaptionApproxPricer_Piterbarg::*ptrNumerateur)(double) = 
//	//							&CheyetteVanillaSwaptionApproxPricer_Piterbarg::numerateur;
//	//double integrale_numerateur	= NumericalMethods::qgaus(ptrNumerateur, 0, t) ;
//
//	//double (CheyetteVanillaSwaptionApproxPricer_Piterbarg::*ptrDenom)(double) = 
//	//							&CheyetteVanillaSwaptionApproxPricer_Piterbarg::denom;
//	//double integrale_denom	= NumericalMethods::qgaus(ptrDenom, 0, t) ;
//
//	// !!!! TODO !!
//		double integrale_numerateur	= 1 ;
//		double integrale_denom		= 1 ;
//
//	return integrale_numerateur/integrale_denom ;
//}
//
//
//double CheyetteDD_VanillaSwaptionApproxPricer::prixSwaptionApproxPiterbarg()
//{
//	////x_t // x_0 ..............................................................
//
//	////double x_t = this->Cheyette_->get_x0() ;
//	////double y_t = this->Cheyette_->get_y0() ;
//
//	//double annuite_t	= cheyetteDD_Model_->annuity(t_, buffer_UnderlyingSwap_, 0, 0) ;	//annuite à la date de valo
//	//double b_barre		= timeAverage_b(buffer_T0_) ;		//on moyennise sur [0, T_0 : date d'exercice de la swaption]
//	//double lambda_barre = timeAverage_lambda(buffer_T0_); 
//
//	////double S0			= S0_; // Cheyette_->get_S0() ;
//	//double K_tilde		= b_barre * buffer_UnderlyingSwap_.get_strike() + (1 - b_barre) * S0 ;
//
//	////double S_t			= S0_; // Cheyette_->txSwapFwd(t_, buffer_UnderlyingSwap_, x_t, y_t) ;
//	//double S_tilde_t	= b_barre * S0_ + (1 - b_barre) * S0 ;
//	////paramètre vol2 = \int_t^T \sigma^2 (s) ds
//	//
//	////TODO
//	////double vol2			= ;  //integralle...............................................
//	//double vol2 = b & lambda ?? ;
//
//	//double prix_BS		= NumericalMethods::Black_Price_vol2(S_tilde_t, K_tilde, vol2, buffer_T0_) / b_barre;
//
//	//return annuite_t * prix_BS ;
//
//	return 0 ;
//}
//
//
//double numericalIntegral_1D_riemann(std::vector<double> grid, boost::function<double(double)> f)
//{
////	for sum :)
//
//	return 0.0 ;
//}
///******************************************
//		Dirkmann approximation
//		**************************/
//
///*
//double CheyetteVanillaSwaptionApproxPricer_Piterbarg::prixSwaption_Dirkmann(double K, 
//																			double x_t, 
//																			double y_t)
//{
//	double prixSwaption(0) ;
//
//	double m			= Cheyette_->get_Cheyette_Parameter().sigma_.get_m().evaluate(0) ;
//	double L			= Cheyette_->get_Cheyette_Parameter().sigma_.getL() ;
//	
//	VanillaSwap swap	= Swaption_->getUnderlyingSwap() ;
//	double annuite		= Cheyette_->annuity(t_, swap, x_t, y_t) ;
//
//	double S_tilde_0N_t = m * annuite + (1- m) * L ;
//	double K_tilde		= (1-m) * L + m * K ;
//
//	//exercise date for the swaption
//
//	//  !!!!!!!!!!!!!!!!!!   verifier que date exercice swaption = get_Indexstart          !!!!!!!!!!!!!!!!!!!!!!!!!!!
//	
//	double T0 = Swaption_->getUnderlyingSwap().get_indexStart() ; 
//	
//	//  !!!!!!!!!!!!!!!!!!   definir fonction d'intégration          !!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//	double vol2 ;
//	
//	//prixSwaption = annuite/m * NumericalMethods::Black_Price_vol2(S_tilde_0N_t, K_tilde, vol2, T0) ;
//	
//	return prixSwaption ;
//}
//*/
//
//
////double CheyetteVanillaSwaptionApproxPricer_Piterbarg::price(const VanillaSwaption & swaption, const std::vector<double> & liborsInitValue )const 
////{
////	this->preCalculateALL(liborsInitValue  ,swaption);
////
////	const VanillaSwap & swap = swaption.getUnderlyingSwap();
////
////	double swaprate0  = this->swapRate_Analytical(swap, liborsInitValue) ;
////	double annuity0   = this->annuity0(swaption.getUnderlyingSwap(), liborsInitValue); // swapRate at time 0
////	double strike     = swap.get_strike() ;
////	double vol_approx = this->volBlack(swaption,liborsInitValue) ;
////	
////	LMM::Index swaptionIndexMaturity = swaption.get_indexMaturity();
////	const double & timeToMaturity = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);
////
////	double approxPrice = annuity0*NumericalMethods::Black_Price(swaprate0 ,strike , vol_approx , timeToMaturity) ;
////	//double approxPrice = NumericalMethods::Black_Price(swaprate0 ,strike , vol_approx , buffer_anuity)  ; //older code
////
////	return approxPrice;
////}
////
////double CheyetteVanillaSwaptionApproxPricer_Piterbarg::volBlack(const VanillaSwaption& vanillaswaption, const std::vector<double>& liborsInitValue) const
////{
////	this->preCalculateALL(liborsInitValue, vanillaswaption);
////
////	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
////
////	assert(lmm_->get_horizon() >= vanillaSwap.get_indexEnd()); //! if not cannot price it.
////	assert(lmm_->get_LMMTenorStructure()->get_tenorType() == vanillaSwap.get_lmmTenorStructureTenorType() );
////
////	//! Piterbarg Formula: YY TODO: can be simplified:
////	//  1. use the symmetric 
////	//  2. precalculate the matrix dSdLi vector
////	LMM::Index swaptionIndexMaturity = vanillaswaption.get_indexMaturity();
////	double maturityTime  = lmm_->get_LMMTenorStructure()->get_tenorDate(swaptionIndexMaturity);
////	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
////
////	double volSquare = 0.0;
////	for(size_t i=0; i<floatingLegPaymentIndexSchedule.size(); ++i)
////	{
////		size_t floatingLegPaymentIndex_i = floatingLegPaymentIndexSchedule[i]; // = i+1
////		size_t indexLibor_i = floatingLegPaymentIndex_i - 1;
////		//size_t indexT_i = indexLibor_i;
////		for(size_t j=0; j<floatingLegPaymentIndexSchedule.size(); ++j)
////		{
////			size_t floatingLegPaymentIndex_j = floatingLegPaymentIndexSchedule[j]; // = j+1
////			size_t indexLibor_j = floatingLegPaymentIndex_j - 1;
////
////			//size_t indexT_j     = indexLibor_j;
////			// tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]
////
////			// cumulated_covariance_tensor = lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j);
////			//volSquare += buffer_omega[i]*buffer_omega[j]*liborsInitValue[indexLibor_i]*liborsInitValue[indexLibor_j]*cumulated_covariance_tensor; 
////
////			double dS_dLi = buffer_dSdLi_[indexLibor_i];
////			double dS_dLj = buffer_dSdLi_[indexLibor_j];
////			double cumulated_covariance  = lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j); //L_i, L_j 's integral of vol in [T_0,T_k], T_k =swaptionIndexMaturity
////			double phi_i = buffer_liborAtExpensionInit_[indexLibor_i] + constShifts_[indexLibor_i]; 
////			double phi_j = buffer_liborAtExpensionInit_[indexLibor_j] + constShifts_[indexLibor_j];
////
////			volSquare += dS_dLi*dS_dLj*phi_i*phi_j*cumulated_covariance; 
////		}
////	}
////
////	double swapRate = swapRate_Analytical(vanillaSwap, liborsInitValue); // swapRate at time 0
////	double volBlack = std::sqrt(volSquare/maturityTime) / swapRate;
////
////	return volBlack;
////}
////
//////
//////void LmmApproxVanillaSwaptionPricer_Piterbarg::accumulateShiftedSwapRateAndStrike(
//////	double& out_shifted_swapRate_T0 ,
//////	double& out_shifted_strike      ,
//////	const std::vector<double> & omega0         ,
//////	const std::vector<double> & libor_shifts   ,
//////	const std::vector<size_t> & floatingIndices)const
//////{
//////	//-- Compute shifted swap rate and shifted strike 
//////	double swap_shift = 0.0;
//////
//////	for (auto i : floatingIndices)
//////		swap_shift +=  omega0[i-1]*libor_shifts[i-1]; //-- Check the index
//////
//////	out_shifted_swapRate_T0 += swap_shift;
//////	out_shifted_strike += swap_shift;	
//////}
////
//////! pre-calculation
////void CheyetteVanillaSwaptionApproxPricer_Piterbarg :: preCalculateLiborAtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
////{
////	assert(buffer_liborAtExpensionInit_.size() == liborsInitValue.size());
////	for(size_t i=0; i<liborsInitValue.size(); ++i)
////		buffer_liborAtExpensionInit_[i] = liborsInitValue[i];
////}
////
////void CheyetteVanillaSwaptionApproxPricer_Piterbarg :: preCalculate_dSdLi_AtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
////{
////	//YY TODO: to delete this line latter, this is only for precaution
////	for(size_t liborIndex=0; liborIndex < buffer_dSdLi_.size(); ++liborIndex) 
////	{
////		buffer_dSdLi_[liborIndex] = -1.0e10; 
////	}
////
////	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
////	const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
////
////	//for(size_t itr=0; itr < floatingLegPaymentIndexSchedule.size(); ++itr)  // other derivative don't calculate ... 
////	for(size_t itr=1; itr < buffer_dSdLi_.size(); ++itr)  // calculate more for MP
////	{
////		LMM::Index paymentIndex = itr;
////		LMM::Index liborIndex   = paymentIndex -1;
////		buffer_dSdLi_[liborIndex] = swapRate_1stDerivative(liborIndex);
////	}
////}
////
////void CheyetteVanillaSwaptionApproxPricer_Piterbarg :: preCalculateALL(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const
////{
////	const VanillaSwap& vanillaSwap = vanillaswaption.getUnderlyingSwap();
////
////	assert(lmm_->get_horizon() >= vanillaSwap.get_indexEnd()); //! if not cannot price it.
////	assert(lmm_->get_LMMTenorStructure()->get_tenorType() == vanillaSwap.get_lmmTenorStructureTenorType() );
////
////	buffer_floatingLegPaymentIndexSchedule_ = vanillaSwap.get_floatingLegPaymentIndexSchedule();
////	buffer_fixedLegPaymentIndexSchedule_    = vanillaSwap.get_fixedLegPaymentIndexSchedule();
////	buffer_deltaTFloatingLeg_               = vanillaSwap.get_DeltaTFloatLeg();
////	buffer_deltaTFixedLeg_                  = vanillaSwap.get_DeltaTFixedLeg();
////	buffer_swapStartIndex_                  = vanillaSwap.get_indexStart();
////	buffer_swapEndIndex_                    = vanillaSwap.get_indexEnd();
////
////	preCalculateLiborAtExpensionInit(liborsInitValue, vanillaswaption);
////	preCalculate_dSdLi_AtExpensionInit(liborsInitValue, vanillaswaption);
////
////	assert(buffer_swapStartIndex_ < buffer_swapEndIndex_) ;
////}
////
//
//
