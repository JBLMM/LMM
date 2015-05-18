#include "CheyetteDD_Model.h"

//getter
CheyetteDD_Model::CheyetteDD_Parameter CheyetteDD_Model::get_CheyetteDD_Parameter() const
{
	return CheyetteDD_Parameter_;
}

//fonctions G(t, T), ZC B(t, T)...
//Displaced Diffusion : k constant, calcul explicite de G(t, T)
double CheyetteDD_Model::G(double t, double T)
{
	assert(t >= 0);  
	double k = CheyetteDD_Parameter_.k_ ;
	return (1/k * (1 - exp(- k * (T-t))) ) ;
}

//double CheyetteDD_Model::P(double t, double T, double x_t, double y_t)
//{
//	double g = G(t,T) ;
//	double P_0_t = courbeInput_PTR_->get_ZC0(t) ;
//	double P_0_T = courbeInput_PTR_->get_ZC0(T) ;
//	return P_0_t/P_0_T * exp(- x_t * g - 1/2 * y_t * g * g) ; 
//}

//f_0_t à coder dans la classe courbeInput
double CheyetteDD_Model::r_t(double f_0_t, double x_t) 
{
	return f_0_t + x_t ; 
}

//double CheyetteDD_Model::annuity(const VanillaSwap& vanillaSwap)
//{
//	//TODO assert // check tenor[0] = 0; c'est à dire le 1er pilier de la tenor structure englobe t = 0 (date de valo) 
//
//	double price = 0.0;
//	const std::vector<size_t>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//	double delta_T, ZC ;
//	size_t  fixedLegPaymentIndex ;
//	//double T_N = fixedLegPaymentIndexSchedule[fixedLegPaymentIndexSchedule.size() - 1] ;
//	
//	for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr)     // !!!!!!!!   check iteration !!!!!!!!!!!!
//	{
//		fixedLegPaymentIndex = fixedLegPaymentIndexSchedule[itr]; 
//		delta_T              = vanillaSwap.get_DeltaTFixedLeg(itr);
//		ZC = courbeInput_PTR_->get_ZC0(fixedLegPaymentIndex) ; //P(0, fixedLegPaymentIndex, 0, 0) ;
//
//		price += delta_T * ZC ;		
//	}
//	return price;
//}
//
//
//double CheyetteDD_Model::txSwapFwd(const VanillaSwap& vanillaSwap)
//{
//	const std::vector<size_t>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//
//	double T_0 = fixedLegPaymentIndexSchedule[0] ;
//	double ZC_T0 = courbeInput_PTR_->get_ZC0(T_0) ;	
//
//	double T_N = fixedLegPaymentIndexSchedule[fixedLegPaymentIndexSchedule.size() - 1] ;
//	double ZC_TN = courbeInput_PTR_->get_ZC0(T_N) ;	
//
//	double annuite = annuity(vanillaSwap) ;
//	return (ZC_T0 - ZC_TN)/ annuite ;
//}




