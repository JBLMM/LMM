#include "CheyetteDD_VanillaSwaptionApproxPricer.h"
#include "LMM/numeric/NumericalMethods.h"

#include <cassert>
#include <vector>
#include <iostream> 
#include <boost/pointer_cast.hpp>


const size_t gridSize  = 10;


CheyetteDD_VanillaSwaptionApproxPricer::CheyetteDD_VanillaSwaptionApproxPricer
						(const CheyetteDD_Model_PTR&	cheyetteDD_Model, 
						const VanillaSwaption_PTR&		swaption, 
						double s0) : s0_(s0)
{
	// domaine de validité de l'approximation :
	//assert(lmm_->get_dispersionRef().get_VolatilityFunction_PTR()->isConstShift()); 
	cheyetteDD_Model_	= cheyetteDD_Model ;
	swaption_			= swaption ;

	//pour S0 : 
	//remplir en faisant appel au fichier input de calibration avec données de marché 
	


	buffer_UnderlyingSwap_				= swaption->getUnderlyingSwap() ;
	buffer_T0_							= swaption->getUnderlyingSwap().get_StartDate() ;		//1ere date de fixing
	buffer_TN_							= swaption->getUnderlyingSwap().get_EndDate() ;			//date dernier flux (setté en T_{N-1})
	buffer_fixedLegPaymentIndexSchedule_= buffer_UnderlyingSwap_.get_fixedLegPaymentIndexSchedule() ;
	buffer_deltaTFixedLeg_				= buffer_UnderlyingSwap_.get_DeltaTFixedLeg() ;

	buffer_courbeInput_					= cheyetteDD_Model->get_courbeInput_PTR() ;
}


double CheyetteDD_VanillaSwaptionApproxPricer::calculate_y_bar(double t) const
{
	//sigma_r^0(t) = sigma_r(t, 0, 0)
	//int_0^t sigma_r^0(s)^2 exp(- 2k (t-s) ) ds
	double k = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().k_  ;
	//les abscisses et ordonnees de la fonction sigma
	std::vector<double> sigma_x = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().sigma_.getx_() ;
	std::vector<double> sigma_y = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().sigma_.gety_() ;

	std::vector<double> m_x = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().m_.getx_() ;
	std::vector<double> m_y = this->get_CheyetteDD_Model()->get_CheyetteDD_Parameter().m_.gety_() ;

	courbeInput_PTR courbe = get_CheyetteDD_Model()->get_courbeInput_PTR() ;

	if (m_x != sigma_x){std::cout << "Displaced Diffusion: piliers de m(t) et de sigma(t) ne coincident pas" << std::endl; }
	assert(m_x == sigma_x);

	if (t > sigma_x[sigma_x.size() - 1])
	{
		std::cout << "t : " << t << " , sigma_x[sigma_x.size() - 1] : " << sigma_x[sigma_x.size() - 1] << std::endl ;
		std::cout << "extrapolation de sigma pour l'integration" << std::endl; 
	}

	assert(t - 0.1 <= sigma_x[sigma_x.size() - 1]);

	std::vector<double> x = sigma_x ; //= m_x ;
	double integrale(0), y ;
 
	double r0 = courbe->get_f_0_t(0) ;
	
	for (size_t i = 0 ; i < x.size() - 1 ; ++i) //on peut optimiser avec un while
	{
		y = sigma_y[i] * (m_y[i] * courbe->get_f_0_t(t) + (1 - m_y[i]) * r0) ;
		integrale += y * y * (exp(2 * k * std::min(x[i+1], t)) - exp(2 * k * std::min(x[i], t))) / (2 * k);
	}
	return exp(- 2 * k * t) * integrale ; 
}


double CheyetteDD_VanillaSwaptionApproxPricer::ZC_1stDerivative_on_xt(double t, double T, double x_t) const
{
	assert(0 <= T && T <= buffer_TN_) ;
	double y_bar_t = calculate_y_bar(t) ;
	double res ;
	
	if (t == 0){
		double tauxZC = buffer_courbeInput_->get_tauxZC0(T) ;
		double P_0_T  = exp(- tauxZC * T) ;
		double g = cheyetteDD_Model_->G(0,T) ;
		res = - g * P_0_T ;				//- G(0, T) P(0, T) 
	}
	else{			
		double ZC = cheyetteDD_Model_->P(t, T, x_t, y_bar_t) ;
		double g = cheyetteDD_Model_->G(t,T) ;
		res = - g * ZC ;
	}
	return res ;	
}

double CheyetteDD_VanillaSwaptionApproxPricer::ZC_2ndDerivative_on_xt(double t, double T, double x_t) const
{
	assert(0 <= T && T <= buffer_TN_) ;
	double y_bar_t = calculate_y_bar(t) ;
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
	double y_bar_t = calculate_y_bar(t) ;
	double ZC_T0, ZC_TN ;
	if (t == 0){
		ZC_T0 = exp( - buffer_courbeInput_->get_tauxZC0(buffer_T0_) * buffer_T0_) ;
		ZC_TN = exp( - buffer_courbeInput_->get_tauxZC0(buffer_TN_) * buffer_TN_) ; 
	}
	else
	{
		ZC_T0 = cheyetteDD_Model_->P(t, buffer_T0_, x_t, y_bar_t) ;
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
	double y_bar_t = calculate_y_bar(t) ;
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

/********************************************************************************/

//derivee par rapport à x_t
//prend le paramètre x_t
//en paramètre donner f-1(S) = x_t
double CheyetteDD_VanillaSwaptionApproxPricer::swapRateVolatility_1stDerivative(double t, double x_t) const
{
	CheyetteDD_Model::CheyetteDD_Parameter parameter = cheyetteDD_Model_->get_CheyetteDD_Parameter() ;
	piecewiseconst_RR_Function sigma = parameter.sigma_ ;
	piecewiseconst_RR_Function m = parameter.m_ ;

	courbeInput_PTR courbe = get_CheyetteDD_Model()->get_courbeInput_PTR() ;
	double r0 = courbe->get_f_0_t(0) ;
	
	return (m.evaluate(t) * x_t + (1-m.evaluate(t))* r0) * sigma.evaluate(t) * 
				swapRate_2ndDerivative(t, x_t) / swapRate_1stDerivative(t, x_t) 
					+ m.evaluate(t) *sigma.evaluate(t) ; //+ sigma.d_sigma_r_dx(t) ;

}

//evaluee en t=0 et pour un taux de swap s_bar = s0_
double CheyetteDD_VanillaSwaptionApproxPricer::calculate_phi_t_s_bar(double t) const
{
	//retourne dS(t)/dx(t) sigma_r(t) (t=0, inverse, y_bar)
	
	double s_bar = inverse(t, s0_) ;

	CheyetteDD_Model::CheyetteDD_Parameter parameter = cheyetteDD_Model_->get_CheyetteDD_Parameter() ;
	piecewiseconst_RR_Function sigma = parameter.sigma_ ;
	piecewiseconst_RR_Function m = parameter.m_ ;

	//en t = 0 
	//return swapRate_1stDerivative(0, s_bar) * sigma.evaluate(0)  ;

	courbeInput_PTR courbe = get_CheyetteDD_Model()->get_courbeInput_PTR() ;
	double r0 = courbe->get_f_0_t(0) ;

	return swapRate_1stDerivative(t, s_bar) * (m.evaluate(t) * s_bar + (1-m.evaluate(t))* r0) * sigma.evaluate(t)  ;
}

double CheyetteDD_VanillaSwaptionApproxPricer::swapRateVolatility_approx_lineaire(double t, double s) const
{
	//dérivée à évaluer en x_t = S^{-1}(s)
	//calcul de l'inverse de S(t, x_t) = s 
	//retourne x_t
	double inverse_s0 = inverse(t, s0_) ;
	return ( calculate_phi_t_s_bar(t) + swapRateVolatility_1stDerivative(t, inverse_s0) * (s - s0_) ) ;
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
	double y_bar_t = calculate_y_bar(t) ;
	double inverse_s0 = inverse(t, s0_) ;	
	return calculate_phi_t_s_bar(t) - swapRateVolatility_1stDerivative(t, inverse_s0) * s0_  ;
}

//B(t) 
double CheyetteDD_VanillaSwaptionApproxPricer::B(double t) const
{
	double y_bar_t = calculate_y_bar(t) ;
	double inverse_s0 = inverse(t, s0_) ;
	return swapRateVolatility_1stDerivative(t, inverse_s0) ;
}

//lambda(t) 
double CheyetteDD_VanillaSwaptionApproxPricer::lambda(double t) const
{
	return  B(t) / s0_ + A(t) ;
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

//integrale de 0 à t de lambda^2(u) du
double CheyetteDD_VanillaSwaptionApproxPricer::v2(double t) const
{
	double gridStart = 0.0;
	double gridEnd = t ;
	numeric::Integrator1D_Riemann integral_Riemann(gridStart, gridEnd, gridSize);

	boost::function<double(double)> func = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::lambda2, this, _1);
	
	double result = integral_Riemann.integrate (func);
	return result ;
}

double CheyetteDD_VanillaSwaptionApproxPricer::timeAveraging_b_numerateur(double t) const
{
	return lambda2(t)  * b(t) ;  //* v2(t) cf incremental integral
}
double CheyetteDD_VanillaSwaptionApproxPricer::timeAveraging_b_denom(double t) const
{
	return lambda2(t)  ;		//* v2(t) cf incremental integral
}

//retourne b_barre
//average over [0, t]
//gridSize : nb de points pour integrale Riemann
//gridSize + 1 : pour 100 mettre 101, delta_t = 1/100
double CheyetteDD_VanillaSwaptionApproxPricer::timeAverage(double t, size_t gridSize) const	
{
	double gridStart = 0.0;
	double gridEnd = t ; 

	std::vector<double> f_grids(gridSize) ;
	for (size_t i = 0 ; i < gridSize ; ++i)
	{
		f_grids[i] = lambda2(i * (gridEnd-gridStart)/(gridSize-1)) ;  //valeur de lambda2
	}

//integrale numerateur
	numeric::IncrementalIntegrator1D_Riemann integral(gridStart, gridEnd, gridSize, f_grids);
	boost::function<double(double)> func1 = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::timeAveraging_b_numerateur, *this, _1);

	//double integrale_numerateur = integral_num_Riemann.integrate(func1);
	double integrale_numerateur = integral.integrate(func1);

//integrale denominateur
	boost::function<double(double)> func2 = boost::bind(&CheyetteDD_VanillaSwaptionApproxPricer::timeAveraging_b_denom, *this, _1);
	//double integrale_denom = integral_denom_Riemann.integrate(gridStart, gridEnd, gridSize, func2);
	double integrale_denom = integral.integrate(func2);

	return integrale_numerateur/integrale_denom ;

}

//gridSize : nb de points pour integrale Riemann
//gridSize + 1 : pour 100 mettre 101, delta_t = 1/100
double CheyetteDD_VanillaSwaptionApproxPricer::timeAverage2(double t, size_t gridSize) const
{
	double gridStart = 0.0;
	double gridEnd = t ;
	double delta = (gridEnd-gridStart)/(gridSize - 1);  //nb de points ou est évaluée la fonction
	std::vector<double> grid(gridSize);

	grid[0] = gridStart ;
	for(size_t i=1; i<gridSize; ++i)
	{
		grid[i] = grid[i-1] + delta;
	}
//vecteur lambda2(.) : valeurs de lambda2(u) sur les points de la grille de Riemann
	std::vector<double> vect_lambda2(gridSize) ;
	for (size_t i = 0 ; i < gridSize ; ++i)
	{
		vect_lambda2[i] = lambda2(grid[i]) ; 
		//std::cout << "vect_lambda2[" << i << "] : " << vect_lambda2[i] << std::endl ; 
	}
//vecteur integrale de 0 à u lambda2(.) : valeurs de l'integrale de 0 à t_i
//approx par trapèzes
	std::vector<double> vect_int_lambda2(gridSize) ;
	vect_int_lambda2[0] = 0 ;
	for (size_t i = 1 ; i < gridSize ; ++i)
	{
		vect_int_lambda2[i] = vect_int_lambda2[i - 1] + (vect_lambda2[i-1] + vect_lambda2[i])/2 * delta ; 
		//std::cout << "vect_int_lambda2[" << i << "] : " << vect_int_lambda2[i] << std::endl ; 
	}

//integrales numerateur et denominateur
	   //! Riemann sum
       double result_num = 0.0;
       double result_denom = 0.0;
	   double integrale_0_u_approx ;
	   for(size_t i=0; i<grid.size()-1; ++i)
	   {
		   double t1 = grid[i];
		   double t2 = grid[i+1];
		   double t_mid = (t1+t2)/2;
		   integrale_0_u_approx = 1/2.0 * (vect_int_lambda2[i] + vect_int_lambda2[i + 1]) ;
		   result_num += integrale_0_u_approx * lambda2(t_mid) * b(t_mid) * (t2-t1);   
		   result_denom += integrale_0_u_approx * lambda2(t_mid) * (t2-t1);   
			//std::cout << "integrale_0_u_approx : " << integrale_0_u_approx << ", res_numerateur : " << result_num << ", res_denom : " << result_denom << std::endl ;
	   }

	return result_num/result_denom ;
}

double CheyetteDD_VanillaSwaptionApproxPricer::prixSwaptionApproxPiterbarg(size_t gridSize) const
{
	double annuity0 = swapRateDenominator(0, 0) ;	//en t = 0 c'est l'annuité(0)

	std::cout << "debut du calcul de b_barre" << std::endl ;
	std::cout << "avec time average Incremental Riemann class " << std::endl ;
	double b_barre  = timeAverage(buffer_T0_, gridSize) ;		//average jusqu'à T0, date d'entrée dans le swap
	//std::cout << "avec time average approx " << std::endl ;
	//double b_barre  = timeAverage2(buffer_T0_, gridSize) ;		//average jusqu'à T0, date d'entrée dans le swap
	std::cout << "fin du calcul de b_barre" << std::endl ;

	std::cout << "  " << std::endl ;
	std::cout << "strike K : " << buffer_UnderlyingSwap_.get_strike() << ", b_barre : " << b_barre << ", s0 : " << s0_ << std::endl ;
	double K_tilde	= b_barre * buffer_UnderlyingSwap_.get_strike() + (1 - b_barre) * s0_ ;
	
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
