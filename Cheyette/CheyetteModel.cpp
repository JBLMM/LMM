#include "CheyetteModel.h"

//getter
CheyetteModel::Cheyette_Parameter CheyetteModel::get_Cheyette_Parameter()
{
	return (Cheyette_Parameter_) ;
}

CheyetteModel::Cheyette_Parameter initStruct(piecewiseconst_RR_Function chi, piecewiseconst_DD_R2R_Function sigma)
{
	CheyetteModel::Cheyette_Parameter maStruct;
	maStruct.chi_ = chi ;
	maStruct.sigma_ = sigma ;
	return maStruct ;
}


//fonctions G(t, T), ZC B(t, T)...
double CheyetteModel::G(double t, double T)
{
	assert(t >= 0) ;
	double k = Cheyette_Parameter_.chi_.evaluate(0) ;  
	assert(k != 0) ;
	return (1/k * (1 - exp(- k * (T-t))) ) ;
}

double CheyetteModel::P(double t, double T, double x_t, double y_t)
{
	double g = G(t,T) ;
	double P_0_t = courbeInput_PTR_->get_ZC_maturite_T(t) ;
	double P_0_T = courbeInput_PTR_->get_ZC_maturite_T(T) ;
	return (P_0_t/P_0_T * exp(- x_t * g - 1/2 * y_t * g * g) ) ; 
}

//f_0_t à coder dans la classe courbeInput
double CheyetteModel::r_t(double f_0_t, double x_t) 
{
	return (f_0_t + x_t) ; 
}

double CheyetteModel::annuity(	const size_t			indexValuationDate,
								const VanillaSwap&		vanillaSwap, 
								double					x_t, 
								double					y_t)
{
	assert(indexValuationDate <= vanillaSwap.get_indexStart()); //YY TODO: this test too slow, esp: within MC simulation

	double price = 0.0;
	const std::vector<size_t>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
	double delta_T, ZC ;
	size_t  fixedLegPaymentIndex ;
	//double T_N = fixedLegPaymentIndexSchedule[fixedLegPaymentIndexSchedule.size() - 1] ;
	
	for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr)
	{
		fixedLegPaymentIndex = fixedLegPaymentIndexSchedule[itr]; 
		delta_T              = vanillaSwap.get_DeltaTFixedLeg(itr);
		ZC = P(indexValuationDate, fixedLegPaymentIndex, x_t, y_t) ;

		price += delta_T * ZC ;		
	}
	return price;
}
