#include <Cheyette\Pricer\CheyetteDD_VanillaSwaptionApproxPricer.h>
#include "LMM/numeric/NumericalMethods.h"

#include <cassert>
#include <vector>
#include <iostream> 
//#include <boost/pointer_cast.hpp>


const size_t gridSize  = 11 ;


CheyetteDD_VanillaSwaptionApproxPricer::CheyetteDD_VanillaSwaptionApproxPricer
						(const CheyetteDD_Model_PTR&	cheyetteDD_Model, 
						const VanillaSwaption_PTR&		swaption) 
						:cheyetteDD_Model_(cheyetteDD_Model), swaption_(swaption)  					
{
	// domaine de validité de l'approximation : //assert( ... ); 

	buffer_courbeInput_						= cheyetteDD_Model->get_courbeInput_PTR() ;
	buffer_UnderlyingSwap_					= swaption->getUnderlyingSwap() ;
	buffer_T0_								= swaption->getUnderlyingSwap().get_StartDate() ;		//1ere date de fixing
	buffer_TN_								= swaption->getUnderlyingSwap().get_EndDate() ;			//date dernier flux (setté en T_{N-1})
	buffer_fixedLegPaymentIndexSchedule_	= buffer_UnderlyingSwap_.get_fixedLegPaymentIndexSchedule() ;
	buffer_floatingLegPaymentIndexSchedule_	= buffer_UnderlyingSwap_.get_floatingLegPaymentIndexSchedule() ;
	buffer_deltaTFixedLeg_					= buffer_UnderlyingSwap_.get_DeltaTFixedLeg() ;

	initialize_y_bar(buffer_T0_, gridSize);	
	
	buffer_s0_								= swapRate0();
}

//fonction qui intervient pour le calcul de y bar
//sera integrée sur s entre 0 et t 
double CheyetteDD_VanillaSwaptionApproxPricer::to_integrate_y_bar(double s) const
{
	double k			= cheyetteDD_Model_->get_CheyetteDD_Parameter().k_ ;
	double sigma_r_0	= cheyetteDD_Model_->sigma_r(s, 0) ;			//sigma_r^0(t) = sigma_r(t, x_t, y_t) 
	return exp(2 * k * s) * sigma_r_0 * sigma_r_0 ; 
}

//integrale jusqu'à t 
// y_bar(t) = exp( - 2 k t ) * integrale_0^t( exp(2 k s) sigma_r^0(s) sigma_r^0(s) ds ) 
void CheyetteDD_VanillaSwaptionApproxPricer::initialize_y_bar(double t, size_t gridSize) const
{
	double gridStart = 0 ;
	double gridEnd = t ;
	double k			= cheyetteDD_Model_->get_CheyetteDD_Parameter().k_ ;

	//constructeur pour le schema d'integration
	numeric::IncrementalIntegrator1D_Riemann integral(gridStart, gridEnd, gridSize);
	//constructeur pour la fonction à intégrer
	boost::function<double(double)> f = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::to_integrate_y_bar, *this, _1);

	integral.vecteur_integrate(f) ;
	//integral.integrate(f) ;  

	std::vector<double> vect_grids = integral.get_grids() ;
	std::vector<double> vect_values = integral.get_values() ;
	//multiplication de l'integrale par exp( - 2 k t)
	for (size_t i = 0 ; i < vect_values.size() ; ++i){
		vect_values[i] = exp( - 2 * k * t) * vect_values[i] ;
	}
	buffer_y_bar_.set_grid_(vect_grids) ;
	buffer_y_bar_.set_value_(vect_values) ;	
}


//double CheyetteDD_VanillaSwaptionApproxPricer::calculate_y_bar(double t) const
//{
//	//sigma_r^0(t) = sigma_r(t, 0, 0)
//	//int_0^t sigma_r^0(s)^2 exp(- 2k (t-s) ) ds
//	double k = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().k_  ;
//	//les abscisses et ordonnees de la fonction sigma
//	std::vector<double> sigma_x = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().sigma_.getx_() ;
//	std::vector<double> sigma_y = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().sigma_.gety_() ;
//
//	std::vector<double> m_x = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().m_.getx_() ;
//	std::vector<double> m_y = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().m_.gety_() ;
//
//	courbeInput_PTR courbe = get_CheyetteDD_Model()->get_courbeInput_PTR() ;
//
//	if (m_x != sigma_x){std::cout << "Displaced Diffusion: piliers de m(t) et de sigma(t) ne coincident pas" << std::endl; }
//	assert(m_x == sigma_x);
//
//	if (t > sigma_x[sigma_x.size() - 1])
//	{
//		std::cout << "t : " << t << " , sigma_x[sigma_x.size() - 1] : " << sigma_x[sigma_x.size() - 1] << std::endl ;
//		std::cout << "extrapolation de sigma pour l'integration" << std::endl; 
//	}
//
//	assert(t - 0.1 <= sigma_x[sigma_x.size() - 1]);
//
//	std::vector<double> x = sigma_x ; //= m_x ;
//	double integrale(0), y ;
// 
////	double r0 = courbe->get_f_0_t(0) ;
//	piecewiseconst_RR_Function shift = CheyetteDD_Parameter_.shift_ ;
//	
//	
//	for (size_t i = 0 ; i < x.size() - 1 ; ++i) //on peut optimiser avec un while
//	{
//		double t_i = x[i] ;
//		y = sigma_y[i] * (m_y[i] * courbe->get_f_0_t(t_i) + (1 - m_y[i]) * r0) ;
//		integrale += y * y * (exp(2 * k * std::min(x[i+1], t)) - exp(2 * k * std::min(x[i], t))) / (2 * k);
//	}
//	return exp(- 2 * k * t) * integrale ; 
//}
//



double CheyetteDD_VanillaSwaptionApproxPricer::ZC_1stDerivative_on_xt(double t, double T, double x_t) const
{
	assert(0 <= T && T <= buffer_TN_) ;
	double res ;
	
	if (t == 0){     //courbe spot
		double tauxZC = buffer_courbeInput_->get_tauxZC0(T) ;
		double P_0_T  = exp(- tauxZC * T) ;
		double g = cheyetteDD_Model_->G(0,T) ;
		res = - g * P_0_T ;											//- G(0, T) P(0, T) 
	}
	else{			//modèle
		double y_bar_t = buffer_y_bar_(t) ;
		double ZC = cheyetteDD_Model_->P(t, T, x_t, y_bar_t) ;		//approximation
		double g = cheyetteDD_Model_->G(t,T) ;
		res = - g * ZC ;
	}
	return res ;	
}

double CheyetteDD_VanillaSwaptionApproxPricer::ZC_2ndDerivative_on_xt(double t, double T, double x_t) const
{
	assert(0 <= T && T <= buffer_TN_) ;
	double y_bar_t = buffer_y_bar_(t) ;
	double res ;
	
	if (t == 0){
		double tauxZC = buffer_courbeInput_->get_tauxZC0(T) ;
		double P_0_T  = exp(- tauxZC * T) ;
		double g = cheyetteDD_Model_->G(0,T) ;
		res = g * g * P_0_T ;				//- G(0, T)^2 P(0, T) 
	}
	else{			
		double ZC = cheyetteDD_Model_->P(t, T, x_t, y_bar_t) ;
		double g = cheyetteDD_Model_->G(t,T) ;
		res = g * g * ZC ;
	}
	return res ;					
}

// Numerator = P(t, T0) - P(t, TN)
//si t = 0 : appel à la courbe spot
//si t > 0 : passage du paramètre x_t pour la fonction inverse
double CheyetteDD_VanillaSwaptionApproxPricer::swapRateNumerator(double t, double x_t) const 
{
	assert( t >= 0 ) ;
	double y_bar_t = buffer_y_bar_(t) ;
	double ZC_T0, ZC_TN ;
	if (t == 0){  // bad comparision   // prefer to do: if (t==0) { check x_t ==0 }
		ZC_T0 = exp( - buffer_courbeInput_->get_tauxZC0(buffer_T0_) * buffer_T0_) ;
		ZC_TN = exp( - buffer_courbeInput_->get_tauxZC0(buffer_TN_) * buffer_TN_) ; 
	}
	else
	{
		ZC_T0 = cheyetteDD_Model_->P(t, buffer_T0_, x_t, y_bar_t) ; //tester si OK en t = 0 avec y barre
		ZC_TN = cheyetteDD_Model_->P(t, buffer_TN_, x_t, y_bar_t) ;	
	}
	return  ZC_T0 - ZC_TN ; 
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRateNumerator_1stDerivative(double t, double x_t) const
{
	return ZC_1stDerivative_on_xt(t, buffer_T0_, x_t) - ZC_1stDerivative_on_xt(t, buffer_TN_, x_t) ;
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRateNumerator_2ndDerivative(double t, double x_t) const
{
	return ZC_2ndDerivative_on_xt(t, buffer_T0_, x_t) - ZC_2ndDerivative_on_xt(t, buffer_TN_, x_t) ;
}

// Denominator = \sum delta_k P(t,T_k) en t = 0
double CheyetteDD_VanillaSwaptionApproxPricer::swapRateDenominator(double t, double x_t) const 
{
	double y_bar_t = buffer_y_bar_(t) ;
	double price = 0.0;
	double dateEchangeFluxFixe, delta_T, ZC ;

	double fixed_tenor = buffer_UnderlyingSwap_.get_fixedLegTenorType().YearFraction() ;
	double float_tenor = buffer_UnderlyingSwap_.get_floatingLegTenorType().YearFraction() ;
	double tenor_ref = std::min(fixed_tenor, float_tenor) ;  //le plus petit 

	if (t == 0)
	{
		//somme sur tous les flux fixes
		for(size_t itr = 0; itr < buffer_fixedLegPaymentIndexSchedule_.size(); ++itr) 
		{
			//convertit l'indice/le numero du flux en la date de tombée du flux (ex : flux numero 2 survient à date 1Y)
			dateEchangeFluxFixe = buffer_fixedLegPaymentIndexSchedule_[itr] * tenor_ref ;

			delta_T = buffer_UnderlyingSwap_.get_DeltaTFixedLeg(itr);	
			ZC		= exp( - buffer_courbeInput_->get_tauxZC0(dateEchangeFluxFixe) * dateEchangeFluxFixe);
			price += delta_T * ZC ;		
		}
	}else
	{
		for(size_t itr = 0; itr < buffer_fixedLegPaymentIndexSchedule_.size(); ++itr) 
		{
			dateEchangeFluxFixe = buffer_fixedLegPaymentIndexSchedule_[itr] * tenor_ref ;
			delta_T = buffer_UnderlyingSwap_.get_DeltaTFixedLeg(itr) ;	
			ZC		= cheyetteDD_Model_->P(t, dateEchangeFluxFixe, x_t, y_bar_t) ;
			price += delta_T * ZC ;
		}
	}
	return price;
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRateDenominator_1stDerivative(double t, double x_t) const
{
	double result = 0;
	double dateEchangeFluxFixe ;

	double fixed_tenor = buffer_UnderlyingSwap_.get_fixedLegTenorType().YearFraction() ;
	double float_tenor = buffer_UnderlyingSwap_.get_floatingLegTenorType().YearFraction() ;
	double tenor_ref = std::min(fixed_tenor, float_tenor) ;  

	for(size_t itr = 0; itr < buffer_fixedLegPaymentIndexSchedule_.size(); ++itr) 
	{
		dateEchangeFluxFixe = buffer_fixedLegPaymentIndexSchedule_[itr] * tenor_ref ;
		result += buffer_deltaTFixedLeg_[itr] * ZC_1stDerivative_on_xt(t, dateEchangeFluxFixe, x_t) ;	
	}
	return result;
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRateDenominator_2ndDerivative(double t, double x_t) const
{
	double result = 0;
	double dateEchangeFluxFixe ;

	double fixed_tenor = buffer_UnderlyingSwap_.get_fixedLegTenorType().YearFraction() ;
	double float_tenor = buffer_UnderlyingSwap_.get_floatingLegTenorType().YearFraction() ;
	double tenor_ref = std::min(fixed_tenor, float_tenor) ;  

	for(size_t itr = 0; itr < buffer_fixedLegPaymentIndexSchedule_.size(); ++itr) 
	{
		dateEchangeFluxFixe = buffer_fixedLegPaymentIndexSchedule_[itr] * tenor_ref ;
		result += buffer_deltaTFixedLeg_[itr] * ZC_2ndDerivative_on_xt(t, dateEchangeFluxFixe, x_t) ;	
	}
	return result;
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRate0() const
{
	return swapRate(0, 0);
}
double CheyetteDD_VanillaSwaptionApproxPricer::swapRate(double t, double x_t) const
{
	double n      = swapRateNumerator(t, x_t);
	double d      = swapRateDenominator(t, x_t);
	assert (abs(d) >= 0.001) ;
	return n/d;
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRate_1stDerivative(double t, double x_t) const
{
	double n   = swapRateNumerator(t, x_t);
	double n_1 = swapRateNumerator_1stDerivative(t, x_t); 

	double d   = swapRateDenominator(t, x_t);
	double d_1 = swapRateDenominator_1stDerivative(t, x_t);

	return (n_1*d - n*d_1)/(d*d);
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRate_2ndDerivative(double t, double x_t) const
{
	double n	= swapRateNumerator(t, x_t);
	double n_1	= swapRateNumerator_1stDerivative(t, x_t); 
	double n_2	= swapRateNumerator_2ndDerivative(t, x_t); 

	double d	= swapRateDenominator(t, x_t);
	double d_1	= swapRateDenominator_1stDerivative(t, x_t);
	double d_2	= swapRateDenominator_2ndDerivative(t, x_t);

	double result = (n_2 * d - n * d_2) * d*d - (n_1 * d - n * d_1) * 2 * d * d_1 ;
	result /= (d*d*d*d);

	return result;
}

/************************  fonction pour inverser     *************************
*************************     Newton - Raphson        ************************/
struct Newton_Raphson_struct
{
private:
	boost::function<double(double)> f_ ;
	boost::function<double(double)> f_derivative_ ;
	double target_ ;
	
public:
	//! constructor
	Newton_Raphson_struct(	boost::function<double(double)>& f,
							boost::function<double(double)>& f_derivative, 
							double target) 
				: f_(f), f_derivative_(f_derivative), target_(target){}

	//! evaluation
	//callable function object that accepts one parameter and returns a tuple:
	//the tuple should have two elements containing the evaluation of the function and it's first derivative.
	boost::math::tuple<double, double> operator()(double x)
	{
		return boost::math::make_tuple(
			f_(x) - target_,
			f_derivative_(x));
	}
};

//S(t, x_t) = swapRate(t, x_t) = s 
//retourne le x_t correspondant
double CheyetteDD_VanillaSwaptionApproxPricer::inverse(double t, double s) const
{                                                                  //swapRate(double t, double x_t)
	boost::function<double(double)> f				= 
				boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::swapRate, this, t, _1);
	boost::function<double(double)> f_derivative	= 
				boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::swapRate_1stDerivative, this, t, _1);
	
	Newton_Raphson_struct NR_struct(f, f_derivative, s);   //struct qui contient f, f', target = s
	double initial_guess	= 0.1 ;
	double min				= 10e-5;
	double max				= 10.0;
    size_t nDigits			= 15;
	boost::uintmax_t nMaxIter  = 20 ;
	double result_newton_raphson = boost::math::tools::newton_raphson_iterate(NR_struct, initial_guess, min, max, nDigits);
	return result_newton_raphson;

}



//derivee par rapport à x_t
//prend le paramètre x_t
//en paramètre donner f-1(S) = x_t
double CheyetteDD_VanillaSwaptionApproxPricer::swapRateVolatility_1stDerivative(double t, double x_t) const
{
	double sigma_r_t	= cheyetteDD_Model_->sigma_r(t, x_t) ;
	
	return sigma_r_t *	swapRate_2ndDerivative(t, x_t) / swapRate_1stDerivative(t, x_t) 
					+ cheyetteDD_Model_->sigma_r_t_1stDerivative(t, x_t) ; 
	
}

//evaluee en t=0 et pour un taux de swap s_bar = s0_
double CheyetteDD_VanillaSwaptionApproxPricer::calculate_phi_t_s_bar(double t) const
{
	//retourne dS(t)/dx(t) sigma_r(t) (t=0, inverse, y_bar)
	double s_bar = inverse(t, buffer_s0_) ;
	double sigma_r_t_sbar	= cheyetteDD_Model_->sigma_r(t, s_bar) ;	
	//return swapRate_1stDerivative(t, s_bar) * (m.evaluate(t) * s_bar + (1-m.evaluate(t))* r0) * sigma.evaluate(t)  ;
	return swapRate_1stDerivative(t, s_bar) * sigma_r_t_sbar ;
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRateVolatility_approx_lineaire(double t, double s) const
{
	//dérivée à évaluer en x_t = S^{-1}(s)
	//calcul de l'inverse de S(t, x_t) = s 
	//retourne x_t
	double inverse_s0 = inverse(t, buffer_s0_) ;
	return ( calculate_phi_t_s_bar(t) + swapRateVolatility_1stDerivative(t, inverse_s0) * (s - buffer_s0_) ) ;
}


/****************  parameter averaging  *********************
*
*	dX(t) = \lambda(t) ( (1-b(t)) X0 + b(t) X(t) ) dW(t)
*	dY(t) = \lambda(t) ( (1- b )  X0  +  b   X(t) ) dW(t)
*
*	dS(t) = (A(t) + B(t) S(t)) dW^Q_A
************************************************************/


//A(t) 
double CheyetteDD_VanillaSwaptionApproxPricer::A(double t) const
{
	double y_bar_t = buffer_y_bar_(t) ;
	double inverse_s0 = inverse(t, buffer_s0_) ;	
	return calculate_phi_t_s_bar(t) - swapRateVolatility_1stDerivative(t, inverse_s0) * buffer_s0_  ;
}

//B(t) 
double CheyetteDD_VanillaSwaptionApproxPricer::B(double t) const
{
	double y_bar_t = buffer_y_bar_(t) ;
	double inverse_s0 = inverse(t, buffer_s0_) ;
	return swapRateVolatility_1stDerivative(t, inverse_s0) ;
}

//lambda(t) 
double CheyetteDD_VanillaSwaptionApproxPricer::lambda(double t) const
{
	return  B(t) / buffer_s0_ + A(t) ;
}	
double CheyetteDD_VanillaSwaptionApproxPricer::lambda2(double t) const
{
	double l = lambda(t) ;
	return l * l ;
}	
//b(t) 
double CheyetteDD_VanillaSwaptionApproxPricer::b(double t) const
{
	//std::cout << "appel de b(" << t << ") : " << A(t) / lambda(t) << std::endl ;
	return  A(t) / lambda(t) ;
}	

////integrale de 0 à t de lambda^2(u) du
//double CheyetteDD_VanillaSwaptionApproxPricer::v2(double t) const
//{
//	double gridStart = 0.0;
//	double gridEnd = t ;
//	numeric::Integrator1D_Riemann integral_Riemann(gridStart, gridEnd, gridSize);
//
//	boost::function<double(double)> func = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::lambda2, this, _1);
//	
//	double result = integral_Riemann.integrate (func);
//	return result ;
//}

double CheyetteDD_VanillaSwaptionApproxPricer::f_outer_num(double t) const
{
	return lambda2(t)  * b(t) ;  //OK (fonction outer seulement ie sans v^2(u)
}
double CheyetteDD_VanillaSwaptionApproxPricer::f_outer_denom(double t) const
{
	return lambda2(t)  ;		//OK (fonction outer seulement ie sans v^2(u)
}



//retourne b_barre
//average over [0, t]
//gridSize : nb de points pour integrale Riemann
//gridSize + 1 : pour 100 mettre 101, delta_t = 1/100
double CheyetteDD_VanillaSwaptionApproxPricer::timeAverage(double t) const	
{
	double gridStart = 0.0;
	double gridEnd = t ; 

//integrale numerateur

	numeric::IncrementalIntegrator2D_Riemann int2D(gridStart, gridEnd, gridSize) ;
	boost::function<double(double)> f_inner = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::lambda2, *this, _1);
	boost::function<double(double)> f_outer = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::f_outer_num, *this, _1);
	
//	double integrale_numerateur = int2D.integrate(f_outer_num, f_inner) ;

//integrale denominateur
	boost::function<double(double)> f_denom = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::f_outer_denom, *this, _1);

//	double integrale_denom = int2D.integrate(f_outer_denom, f_inner);

	std::cout << "ATTENTION au double appel de IncrementalIntegrator2D_Riemann" << std::endl ;
	std::cout << "pas OK !!!!!!" << std::endl ;
	return 0 ;
//	return integrale_numerateur/integrale_denom ;
}

double CheyetteDD_VanillaSwaptionApproxPricer::prixSwaptionApproxPiterbarg() const
{
	double annuity0 = swapRateDenominator(0, 0) ;	//en t = 0 c'est l'annuité(0)

	std::cout << "debut du calcul de b_barre" << std::endl ;
	std::cout << "avec time average Incremental Riemann class " << std::endl ;
	double b_barre  = timeAverage(buffer_T0_) ;		//average jusqu'à T0, date d'entrée dans le swap
	std::cout << "fin du calcul de b_barre" << std::endl ;

	std::cout << "  " << std::endl ;
	std::cout << "strike K : " << buffer_UnderlyingSwap_.get_strike() << ", b_barre : " << b_barre << ", s0 : " << buffer_s0_ << std::endl ;
	double K_tilde	= b_barre * buffer_UnderlyingSwap_.get_strike() + (1 - b_barre) * buffer_s0_ ;
	
	std::cout << "debut du calcul de la variance (integrale de lambda_t)" << std::endl ;
	double gridStart = 0.0;
	double gridEnd = buffer_T0_ ;

	numeric::Integrator1D_Riemann integral_Riemann(gridStart, gridEnd, gridSize) ;

	boost::function<double(double)> func1 = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::lambda2, this, _1);
	double integrale = integral_Riemann.integrate(func1);
	std::cout << "fin du calcul de la variance (integrale de lambda_t)" << std::endl ;

	double vol = sqrt(integrale) ;

	return annuity0 / b_barre * NumericalMethods::Black_Price_vol2(1, K_tilde, vol, buffer_T0_) ;
}


//retourne b_barre
//average over [0, t]
//gridSize : nb de points pour integrale Riemann
//gridSize + 1 : pour 100 mettre 101, delta_t = 1/100
//double CheyetteDD_VanillaSwaptionApproxPricer::timeAverage(double t, size_t gridSize) const	
//{
//	double gridStart = 0.0;
//	double gridEnd = t ; 
//
//	std::vector<double> f_grids(gridSize) ;
//	for (size_t i = 0 ; i < gridSize ; ++i)
//	{
//		f_grids[i] = lambda2(i * (gridEnd-gridStart)/(gridSize-1)) ;  //valeur de lambda2
//	}
//
////integrale numerateur
//	numeric::IncrementalIntegrator1D_Riemann integral(gridStart, gridEnd, gridSize);
//	boost::function<double(double)> func1 = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::timeAveraging_b_numerateur, *this, _1);
//
//	//double integrale_numerateur = integral_num_Riemann.integrate(func1);
//	double integrale_numerateur = integral.integrate(func1);
//
////integrale denominateur
//	boost::function<double(double)> func2 = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::timeAveraging_b_denom, *this, _1);
//	//double integrale_denom = integral_denom_Riemann.integrate(gridStart, gridEnd, gridSize, func2);
//	double integrale_denom = integral.integrate(func2);
//
//	return integrale_numerateur/integrale_denom ;
//
//}

//gridSize : nb de points pour integrale Riemann
//gridSize + 1 : pour 100 mettre 101, delta_t = 1/100
//double CheyetteDD_VanillaSwaptionApproxPricer::timeAverage2(double t, size_t gridSize) const
//{
//	double gridStart = 0.0;
//	double gridEnd = t ;
//	double delta = (gridEnd-gridStart)/(gridSize - 1);  //nb de points ou est évaluée la fonction
//	std::vector<double> grid(gridSize);
//
//	grid[0] = gridStart ;
//	for(size_t i=1; i<gridSize; ++i)
//	{
//		grid[i] = grid[i-1] + delta;
//	}
////vecteur lambda2(.) : valeurs de lambda2(u) sur les points de la grille de Riemann
//	std::vector<double> vect_lambda2(gridSize) ;
//	for (size_t i = 0 ; i < gridSize ; ++i)
//	{
//		vect_lambda2[i] = lambda2(grid[i]) ; 
//		//std::cout << "vect_lambda2[" << i << "] : " << vect_lambda2[i] << std::endl ; 
//	}
////vecteur integrale de 0 à u lambda2(.) : valeurs de l'integrale de 0 à t_i
////approx par trapèzes
//	std::vector<double> vect_int_lambda2(gridSize) ;
//	vect_int_lambda2[0] = 0 ;
//	for (size_t i = 1 ; i < gridSize ; ++i)
//	{
//		vect_int_lambda2[i] = vect_int_lambda2[i - 1] + (vect_lambda2[i-1] + vect_lambda2[i])/2 * delta ; 
//		//std::cout << "vect_int_lambda2[" << i << "] : " << vect_int_lambda2[i] << std::endl ; 
//	}
//
////integrales numerateur et denominateur
//	   //! Riemann sum
//       double result_num = 0.0;
//       double result_denom = 0.0;
//	   double integrale_0_u_approx ;
//	   for(size_t i=0; i<grid.size()-1; ++i)
//	   {
//		   double t1 = grid[i];
//		   double t2 = grid[i+1];
//		   double t_mid = (t1+t2)/2;
//		   integrale_0_u_approx = 1/2.0 * (vect_int_lambda2[i] + vect_int_lambda2[i + 1]) ;
//		   result_num += integrale_0_u_approx * lambda2(t_mid) * b(t_mid) * (t2-t1);   
//		   result_denom += integrale_0_u_approx * lambda2(t_mid) * (t2-t1);   
//			//std::cout << "integrale_0_u_approx : " << integrale_0_u_approx << ", res_numerateur : " << result_num << ", res_denom : " << result_denom << std::endl ;
//	   }
//
//	return result_num/result_denom ;
//}


