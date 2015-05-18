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
	void createSwap();
	VanillaSwaption createSwaption() ;
	//void TestApprox() ;



#endif //_TestApproxDD_