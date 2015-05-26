#include "CheyetteDD_Model.h"

//getter
CheyetteDD_Model::CheyetteDD_Parameter CheyetteDD_Model::get_CheyetteDD_Parameter() const
{
	return CheyetteDD_Parameter_;
}

//fonctions G(t, T), ZC B(t, T)...
//Displaced Diffusion : k constant, calcul explicite de G(t, T)
double CheyetteDD_Model::G(double t, double T) const
{
	assert(t >= 0);  
	double k = CheyetteDD_Parameter_.k_ ;
	return (1/k * (1 - exp(- k * (T-t))) ) ;
}

double CheyetteDD_Model::P(double t, double T, double x_t, double y_t) const 
{
	double g = G(t,T) ;
	double P_0_t = exp( - courbeInput_PTR_->get_tauxZC0(t) * t ) ;
	double P_0_T = exp( - courbeInput_PTR_->get_tauxZC0(T) * T ) ;
	return P_0_T/P_0_t * exp(- x_t * g - 1/2 * y_t * g * g) ; 
}

//f_0_t à coder dans la classe courbeInput
double CheyetteDD_Model::r_t(double t, double x_t) const 
{
	double f_0_t = courbeInput_PTR_->get_f_0_t(t) ;
	return f_0_t + x_t ; 
}

//double CheyetteDD_Model::annuity(double t, double x_t, double y_t, const VanillaSwap& vanillaSwap)
//{
//	//TODO assert // check tenor[0] = 0; c'est à dire le 1er pilier de la tenor structure englobe t = 0 (date de valo) 
//
//	double price = 0.0;
//	const std::vector<size_t>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//	double dateEchangeFluxFixe, delta_T, ZC ;
//
//	double fixed_tenor = vanillaSwap.get_fixedLegTenorType().YearFraction() ;
//	double float_tenor = vanillaSwap.get_floatingLegTenorType().YearFraction() ;
//	double tenor_ref = std::min(fixed_tenor, float_tenor) ;  //le plus petit 
//
//	if (t == 0)
//	{
//		//somme sur tous les flux fixes
//		for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr) 
//		{
//			//convertit l'indice/le numero du flux en la date de tombée du flux (ex : flux numero 2 survient à date 1Y)
//			dateEchangeFluxFixe = fixedLegPaymentIndexSchedule[itr] * tenor_ref ;
//
//			delta_T = vanillaSwap.get_DeltaTFixedLeg(itr);	
//			ZC		= exp( - courbeInput_PTR_->get_tauxZC0(dateEchangeFluxFixe) * dateEchangeFluxFixe);
//			price += delta_T * ZC ;		
//		}
//	}else
//	{
//		for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr) 
//		{
//			dateEchangeFluxFixe = fixedLegPaymentIndexSchedule[itr] * tenor_ref ;
//			delta_T = vanillaSwap.get_DeltaTFixedLeg(itr) ;	
//			ZC		= P(t, dateEchangeFluxFixe, x_t, y_t) ;
//			price += delta_T * ZC ;			
//	}
//	return price;
//}


//double CheyetteDD_Model::txSwap(const VanillaSwap& vanillaSwap)
//{
//	const std::vector<size_t>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//
//	double fixed_tenor = vanillaSwap.get_fixedLegTenorType().YearFraction() ;
//	double float_tenor = vanillaSwap.get_floatingLegTenorType().YearFraction() ;
//	double tenor_ref = std::min(fixed_tenor, float_tenor) ;  //le plus petit 
//
//	double T_0 = fixedLegPaymentIndexSchedule[0] * tenor_ref ;
//	double ZC_T0 = exp( - courbeInput_PTR_->get_tauxZC0(T_0) * T_0) ;	
//
//	double T_N = fixedLegPaymentIndexSchedule[fixedLegPaymentIndexSchedule.size() - 1] * tenor_ref ;
//	double ZC_TN = exp( - courbeInput_PTR_->get_tauxZC0(T_N) * T_N)  ;	
//
//	double annuite = annuity(vanillaSwap) ;
//	return (ZC_T0 - ZC_TN)/ annuite ;
//}


//S(t, x_t, y_t) = S(t, x_t, y_bar_t) = S(t, x_t)  :  utile pour pouvoir inverser cette fonction par NR
//double S(double t, double x_t, double y_t, const VanillaSwap& vanillaSwap)
//{
//
//
//}



