#ifndef _TestApproxDD_
#define _TestApproxDD_

#include "courbeInput.h"
#include <LMM/instrument/VanillaSwaption.h>
#include <Cheyette/Fonction.h>
#include <Cheyette/CheyetteDD_VanillaSwaptionApproxPricer.h>
#include <Cheyette/CheyetteDD_Model.h>
#include <Cheyette/courbeInput.h>

#include <iostream>
#include <vector>

//test de CheyetteDD_Model.h
//Cheyette Displaced Diffusion
	CheyetteDD_Model::CheyetteDD_Parameter createCheyetteDD_Parameter() ;
	CourbeInput createCourbeInput() ;
	void createDDModel() ;

//test de CheyetteDD_VanillaSwaptionApproxPricer.h
	void test_y_barre(double t) ;  //et test annuite, test swap rate denominator derivee
	void createSwap();

	void testSwap() ; //test swap (dates flux fixes et flottants) 

	VanillaSwaption createSwaption() ;
	//void TestApprox() ;  

	void test_Derivative_ZC() ;

	void test_time_average() ;

	void test_y_bar_cas_limite() ;
	
#endif //_TestApproxDD_