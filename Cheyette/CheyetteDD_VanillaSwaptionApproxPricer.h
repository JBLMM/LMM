#pragma once

#include "CheyetteDD_Model.h"						
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/numeric/Integrator1D.h>

#include "InverseFunction.h"

#include <LMM/numeric/NumericalMethods.h>  //pour le prix Black

#include <cassert>
#include <vector>
#include <boost/shared_ptr.hpp>

/**********************************************************

	approx pour Cheyette Displaced Diffusion
			approx cf papier de Yuan / Piterbarg

**********************************************************/

class CheyetteDD_VanillaSwaptionApproxPricer  
{

private:
	
	CheyetteDD_Model_PTR	cheyetteDD_Model_;  
	VanillaSwaption_PTR		swaption_ ;

	double s0_; //  en input // double s_bar_ ;       

	//appel fréquent aux éléments suivants -> buffer
	mutable VanillaSwap				buffer_UnderlyingSwap_ ;
	mutable double					buffer_T0_ ;
	mutable double					buffer_TN_ ;
	mutable std::vector<LMM::Index> buffer_fixedLegPaymentIndexSchedule_ ;
	mutable std::vector<double>		buffer_deltaTFixedLeg_ ;
	mutable courbeInput_PTR			buffer_courbeInput_ ;

public :
	//constructor  
	CheyetteDD_VanillaSwaptionApproxPricer(	const CheyetteDD_Model_PTR& cheyetteDD_Model, 
											const VanillaSwaption_PTR&	swaption); 
	//destructor
	virtual ~CheyetteDD_VanillaSwaptionApproxPricer(){};

	//getters
	CheyetteDD_Model_CONSTPTR get_CheyetteDD_Model() const {return cheyetteDD_Model_ ;}
	VanillaSwaption_PTR get_VanillaSwaption() const {return swaption_ ;}
	
	//calcul de y_barre(t)
	double calculate_y_bar(double t) const;								//sa valeur depend du paramètre k !
	// ! pendant la calibration, remettre à jour la valeur de y_barre ?

	//pas de y_t : figé à y_bar pour la calibration
	//dérivée du ZC de maturité T évaluée en t = 0
		double ZC_1stDerivative_on_xt(double T) const ;
		double ZC_2ndDerivative_on_xt(double T) const;

		
//	//Numerateur
//		double swapRateNumerator(double t) const; 
//		//derivee 1ère par rapport à x_t
//		double swapRateNumerator_1stDerivative(double t) const;
//		//derivee seconde par rapport à x_t
//		double swapRateNumerator_2ndDerivative(double t) const;
//
//
//	//Denominateur
//		double swapRateDenominator(double t) const;
//		//derivee 1ère par rapport à x_t
//		double swapRateDenominator_1stDerivative(double t) const;
//		//derivee seconde par rapport à x_t
//		double swapRateDenominator_2ndDerivative(double t) const;
//
//
//	//swapRate = swapNumerator / swapDenominator
//		double swapRate(double t) const;
//		double swapRate_1stDerivative(double t) const;
//		double swapRate_2ndDerivative(double t) const;
//
//	//SwapRateVolatility \Phi(t, S_t)
//	//dS(t) = \Phi(t, S_t) dW_t^{Q Annuite}
//
//		// d\Phi /ds (t, \bar{s})
//		double swapRateVolatility_1stDerivative(double t) const;
//
		//evaluee en t=0 et pour un taux de swap s_bar = s0_
	//double calculate_phi_0_s_bar() const; 

//		// approximation lineaire de \Phi(t, \bar{s})
//		double swapRateVolatility_approx_lineaire(double t) const;
//
///****************  parameter averaging  *********************
//*
//*	dX(t) = \lambda(t) ( (1-b(t)) X0 + b(t) X(t) ) dW(t)
//*	dY(t) = \lambda(t) ( (1- b )  X0  +  b   X(t) ) dW(t)
//*
//*	dS(t) = (A(t) + B(t) S(t)) dW^Q_A
//*	S(0) = S0_ connu
//************************************************************/
//		double A(double t) const;
//		double B(double t) const;
//
//		double b(double t) const;
//		double lambda(double t) const;
//		//double lambda2(double t) const;
//		double v2(double t) const;
//
//		//fonctions amies, passage d'une formulation de Displaced Diffusion à l'autre
//
//		//friend double b(double t, CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR ch_PTR) ;
//		//friend double lambda(double t, CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR ch_PTR) ;
//		//friend double lambda2(double t, CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR ch_PTR) ;
//		//friend double v2(double t, CheyetteVanillaSwaptionApproxPricer_Piterbarg_PTR ch_PTR) ;
//		//friend double numerateur(double t) ;
//		//friend double denom(double t) ;
//		
//		// \bar{b} = timeAveraging_b_numerateur/timeAveraging_b_denom
//		double timeAveraging_b_numerateur(double t) ;
//		double timeAveraging_b_denom(double t) ;
//		
//		//retourne b barre du displaced diffusion
//		double timeAverage(double t) ;	
//
//		//prix swaption approximé 
//		double prixSwaptionApproxPiterbarg() ;
//
//
//	////! precalculation: YY TODO: can be further optimized for calibration problem. 
//	//void preCalculateLiborAtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const;
//	//void preCalculate_dSdLi_AtExpensionInit(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const;
//	//virtual void preCalculateALL(const std::vector<double> & liborsInitValue, const VanillaSwaption& vanillaswaption) const; 
//
//
//	//double volBlack(const VanillaSwaption& vanillaSwaption, const std::vector<double>& liborsInitValue) const;
//	//// exactly the same as Robonato.
//	//double price(const VanillaSwaption & swaption, const std::vector<double> & libor_init )const;


};

typedef boost::shared_ptr<CheyetteDD_VanillaSwaptionApproxPricer> CheyetteDD_VanillaSwaptionApproxPricer_PTR;
typedef boost::shared_ptr<const CheyetteDD_VanillaSwaptionApproxPricer> CheyetteDD_VanillaSwaptionApproxPricer_CONSTPTR;

