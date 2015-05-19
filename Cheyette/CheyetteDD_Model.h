#pragma once

#include "Fonction.h"
#include <Cheyette/courbeInput.h>

#include <vector>
#include <iostream>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <LMM/instrument/VanillaSwap.h> 
#include <LMM/helper/LMMTenorStructure.h>

/*  -----------------------------------------------------------
	Cheyette Model Displaced Diffusion (DD)

	dx(t) = (y(t) - k * x(t) ) dt + \sigma_r(t) dW(t)       //k_ : parametre (constant) de mean reversion)      
	y(t) = ( \sigma^2_r(t) - 2 k y(t) ) dt 

	\sigma_r(t) = (m(t) r(t) + (1 - m(t)) r_0 ) \sigma(t)  := sigma(t, x)  

	2 parameters: m(t) and sigma(t) assumed to be piecewise constant time-dependent
	m(t) \in [0,1]
   -----------------------------------------------------------*/


class CheyetteDD_Model  
{
public:
	struct CheyetteDD_Parameter  // tout ce qu-il faudra calibrer
	{
		double						k_;			// add comment please // c'est une constante à vérifier 
	    piecewiseconst_RR_Function	sigma_ ;	
		piecewiseconst_RR_Function	m_;   
		
		//structure constructors 
		CheyetteDD_Parameter(){}
		CheyetteDD_Parameter(double k, piecewiseconst_RR_Function sigma, piecewiseconst_RR_Function m)
				: k_(k), sigma_(sigma), m_(m)
		{
			assert(k > 0) ;
			if(sigma_.getx_() != m_.getx_())
			{
				std::cout << "Warning: pillars of m differ from pillars of sigma" << std::endl ;
			}
		}
	};

private:
	courbeInput_PTR			courbeInput_PTR_ ;             // P(0,t)
	CheyetteDD_Parameter	CheyetteDD_Parameter_;

public:
	CheyetteDD_Model(courbeInput_PTR courbeInput_PTR, const CheyetteDD_Parameter& CheyetteParam)
		: courbeInput_PTR_(courbeInput_PTR), CheyetteDD_Parameter_(CheyetteParam)
	{}

	//getters
	courbeInput_PTR			get_courbeInput_PTR() const{return courbeInput_PTR_ ;}
	CheyetteDD_Parameter	get_CheyetteDD_Parameter() const ;

	//fonctions G(t, T), ZC B(t, T)...
	double G(double t, double T);  
	
	//double P(double t, double T, double x_t, double y_t);  //ZC B(0,t) : donné en input sinon stochastique

	double r_t(double f_0_t, double x_t);

	//annuite A_0N(0)
	double annuity(const VanillaSwap& vanillaSwap) ;

	//?????? 
	//double txSwapFwd(const VanillaSwap&	vanillaSwap) ;

};

typedef boost::shared_ptr<CheyetteDD_Model>       CheyetteDD_Model_PTR;
typedef boost::shared_ptr<const CheyetteDD_Model> CheyetteDD_Model_CONSTPTR;











