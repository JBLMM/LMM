#pragma once

#include "Fonction.h"
#include "courbeInput.h"

#include <vector>
#include <iostream>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <LMM/instrument/VanillaSwap.h> 
#include <LMM/helper/LMMTenorStructure.h>


/*  -----------------------------------------------------------

	Cheyette Model: 

	dx(t) = (y(t) - \Chi(t) x(t) ) dt + \sigma_r(t) dW(t)       //en fait \Chi(t) = \Chi plus loin
	y(t) = ( \sigma^2_r(t) - 2 \Chi(t) y(t) ) dt 

   -----------------------------------------------------------*/

/*  -----------------------------------------------------------
	Cheyette Model Displaced Diffusion (DD)

	dx(t) = (y(t) - \Chi(t) x(t) ) dt + \sigma_r(t) dW(t)       //en fait \Chi(t) = \Chi plus loin
	y(t) = ( \sigma^2_r(t) - 2 \Chi(t) y(t) ) dt 

	\sigma_r(t) = (m(t) r(t) + (1 - m(t)) r_0 ) \sigma(t)  := sigma(t, x)  //pour le moment (?) m est une constante

	2 parameters: m(t) and sigma(t) assumed to be piecewise constant time-dependent
	m(t) \in [0,1]
   -----------------------------------------------------------*/



class CheyetteModel   //en fait c'est le displaced diffusion
{
public:
	struct Cheyette_Parameter
	{
		piecewiseconst_RR_Function chi_ ;			//en pratique constante
		piecewiseconst_DD_R2R_Function sigma_ ;	//vol locale sigma(t,x) !
		//piecewiseconst_R2R_Function sigma_(); 
	 
	};

private:
	courbeInput_PTR courbeInput_PTR_ ; 
	Cheyette_Parameter Cheyette_Parameter_;
	double x0_, y0_ ;							//state variables at t = 0 

public:

	CheyetteModel(courbeInput_PTR courbeInput_PTR, const Cheyette_Parameter& CheyetteParam, double x0, double y0) 
		: courbeInput_PTR_(courbeInput_PTR), Cheyette_Parameter_(CheyetteParam), x0_(x0), y0_(y0) {}

	//getters
	Cheyette_Parameter get_Cheyette_Parameter() ;
	double get_x0(){return x0_ ;}
	double get_y0(){return y0_ ;}

	//fonctions G(t, T), ZC B(t, T)...
	double G(double t, double T);  
	double P(double t, double T, double x_t, double y_t);
	double r_t(double f_0_t, double x_t);

	//annuite A_0N(0)
	//TODO pour A_nN(t) avec une autre date de valo
	double annuity(	const size_t				indexValuationDate,
					const VanillaSwap&			vanillaSwap, 
					double						x_t, 
					double						y_t) ;
};

CheyetteModel::Cheyette_Parameter initStruct(piecewiseconst_RR_Function chi, piecewiseconst_DD_R2R_Function sigma) ;

typedef boost::shared_ptr<CheyetteModel> Cheyette_PTR;
typedef boost::shared_ptr<const CheyetteModel> Cheyette_CONSTPTR;











