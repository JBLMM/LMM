#pragma once

#include "CheyetteModel.h"						
#include <LMM/instrument/VanillaSwaption.h>
#include "InverseFunction.h"
#include <LMM/numeric/NumericalMethods.h>  //pour le prix Black

#include <cassert>
#include <vector>
#include <boost/shared_ptr.hpp>

/**********************************************************
		approx pour Cheyette Displaced Diffusion
			approx cf papier de Yuan
			approx Dirkmann (cf slides)
**********************************************************/

class CheyetteVanillaSwaptionApproxPricer_Piterbarg  
{

private:
	
	Cheyette_PTR		Cheyette_;  //en fait pointe vers un Cheyette Displaced Diffusion
	VanillaSwaption_PTR Swaption_ ;

	double		t_ ;				//date de valorisation
	double		y_bar_ ;			// \bar{y} : valeur ou est figée y
	double		ExpansionPoint_s_ ;	// \bar{s} : point ou sont evaluees les fonctions/dérivées ZC

	double s_bar_ ;       
	double Phi_t_s_bar_ ;

	//appel fréquent aux éléments suivants -> buffer
	double buffer_T0_ ;
	double buffer_TN_ ;
	VanillaSwap buffer_UnderlyingSwap_ ;
	std::vector<LMM::Index> buffer_fixedLegPaymentIndexSchedule_ ;
	std::vector<double> buffer_deltaTFixedLeg_ ;

public :

	//constructor  
	CheyetteVanillaSwaptionApproxPricer_Piterbarg(	const Cheyette_PTR& CheyetteModel, 
													const VanillaSwaption_PTR& Swaption, 
													const double t, const double y_bar, const double ExpansionPoint_s, 
													const double s_bar, const double Phi_t_s_bar);
	//destructor
	virtual ~CheyetteVanillaSwaptionApproxPricer_Piterbarg(){};


		double ZC_1stDerivative(double t, double T, double x_t, double y_t) const ;
		double ZC_2ndDerivative(double t, double T, double x_t, double y_t) const;


	//Numerateur
		double swapRateNumerator(double x_t, double y_t) const; 
		//derivee 1ère par rapport à x_t
		double swapRateNumerator_1stDerivative(double x_t, double y_t) const;
		//derivee seconde par rapport à x_t
		double swapRateNumerator_2ndDerivative(double x_t, double y_t) const;


	//Denominateur
		double swapRateDenominator(double x_t, double y_t) const;
		//derivee 1ère par rapport à x_t
		double swapRateDenominator_1stDerivative(double x_t, double y_t) const;
		//derivee seconde par rapport à x_t
		double swapRateDenominator_2ndDerivative(double x_t, double y_t) const;


	//swapRate = swapNumerator / swapDenominator
		double swapRate(double x_t, double y_t) const;
		double swapRate_1stDerivative(double x_t, double y_t) const;
		double swapRate_2ndDerivative(double x_t, double y_t) const;

	//SwapRateVolatility \Phi(t, S_t)
	//dS(t) = \Phi(t, S_t) dW_t^{Q Annuite}

		// d\Phi /ds (t, s)
		double swapRateVolatility_1stDerivative(double s) const;

		// approximation lineaire de \Phi(t, s)
		double swapRateVolatility_approx_lineaire(double s) const;

/****************  parameter averaging  *********************
*
*	dX(t) = \lambda(t) ( (1-b(t)) X0 + b(t) X(t) ) dW(t)
*	dY(t) = \lambda(t) ( (1- b )  X0  +  b   X(t) ) dW(t)
*
*	dS(t) = (A(t) + B(t) S(t)) dW^Q_A
************************************************************/
		double CheyetteVanillaSwaptionApproxPricer_Piterbarg::A(double t) const ;
		double CheyetteVanillaSwaptionApproxPricer_Piterbarg::B(double t) const ;

		double CheyetteVanillaSwaptionApproxPricer_Piterbarg::b(double t) const ;
		double CheyetteVanillaSwaptionApproxPricer_Piterbarg::lambda(double t) const ;

		double v2(double t) const ;
		double timeAverage(double s) const;
		

	//prix swaption approximé 


/******************************************
		Dirkmann approximation
		**************************/

		double prixSwaption_Dirkmann(double K, 
									double t_valo, 
									double x_t, 
									double y_t) ;

	////! precalculation: YY TODO: can be further optimized for calibration problem. 
	//void preCalculateLiborAtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const;
	//void preCalculate_dSdLi_AtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const;
	//virtual void preCalculateALL(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const; 


	//double volBlack(const VanillaSwaption& vanillaSwaption, const std::vector<double>& liborsInitValue) const;
	//// exactly the same as Robonato.
	//double price(const VanillaSwaption & swaption, const std::vector<double> & libor_init )const;


};

typedef boost::shared_ptr<CheyetteVanillaSwaptionApproxPricer_Piterbarg> CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR;
typedef boost::shared_ptr<const CheyetteVanillaSwaptionApproxPricer_Piterbarg> CheyetteVanillaSwaptionApproxPricer_Piterbarg_CONSTPTR;

