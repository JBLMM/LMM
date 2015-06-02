#pragma once

#include <Cheyette\CheyetteModel\CourbeInput.h>	
#include <LMM/instrument/VanillaSwaption.h>
#include <Cheyette/Fonction.h>
#include <Cheyette/Pricer/CheyetteDD_VanillaSwaptionApproxPricer.h>
#include <Cheyette/CheyetteModel/CheyetteDD_Model.h>
#include <Cheyette/CheyetteModel/CourbeInput.h>
#include <LMM/numeric/Integrator1D.h>

#include <iostream>
#include <vector>


courbeInput_PTR createCourbeInput() ;

CheyetteDD_Model_PTR createCheyetteDD_Model() ;

VanillaSwaption_PTR createSwap();

void testSwap() ; //test swap (dates flux fixes et flottants) 

VanillaSwaption_PTR createSwaption() ;

CheyetteDD_VanillaSwaptionApproxPricer_PTR createApproxPricer_PTR() ;

void test_Derivative_ZC() ;

//void test_y_barre(double t) ;  //et test annuite, test swap rate denominator derivee

void test_time_average() ;


//	void test_y_bar_cas_limite() ;
//
//	void test_incremental_integrale() ;
//	void test_incremental_b_barre() ;
//	
//	void TestApprox() ;  
