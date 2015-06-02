#include "TestMC.h"


void UneTrajectoireEuler()
{
	unsigned long seed = 47;
	RNGenerator_PTR  rnGenerator(new McGenerator(seed));

	std::vector<double> x, y ;
	x.push_back(0) ; x.push_back(0.5) ; x.push_back(1) ; 
	y.push_back(0.25) ; y.push_back(0.5) ;
	double k(1) ;

	std::vector<double> listeMatu, tauxZC ;
	listeMatu.push_back(0) ;	tauxZC.push_back(0.8/100) ; 
	listeMatu.push_back(1) ;	tauxZC.push_back(0.85/100) ; 
	listeMatu.push_back(2) ;	tauxZC.push_back(0.9/100) ; 
	listeMatu.push_back(3) ;	tauxZC.push_back(0.92/100) ;  
	listeMatu.push_back(4) ;	tauxZC.push_back(0.95/100) ; 
	listeMatu.push_back(5) ;	tauxZC.push_back(1.00/100) ; 
	listeMatu.push_back(10) ;	tauxZC.push_back(1.5/100) ; 
	listeMatu.push_back(15) ;	tauxZC.push_back(2.0/100) ;  
	listeMatu.push_back(20) ;	tauxZC.push_back(2.5/100) ;
	listeMatu.push_back(25) ;	tauxZC.push_back(2.3/100) ;
	courbeInput_PTR courbe_PTR_test(new CourbeInput(listeMatu, tauxZC));

	piecewiseconst_RR_Function sigma = piecewiseconst_RR_Function(x, y) ; 
	piecewiseconst_RR_Function m = piecewiseconst_RR_Function(x, y) ;
	piecewiseconst_RR_Function shift = piecewiseconst_RR_Function(x, y) ;
	CheyetteDD_Model::CheyetteDD_Parameter monStruct = CheyetteDD_Model::CheyetteDD_Parameter(k, sigma, m, shift) ;
	CheyetteDD_Model_PTR modele_test_PTR(new CheyetteDD_Model(courbe_PTR_test, monStruct)) ;

	double fwdProbaT = 1 ;
	std::vector<double>			datesOfSimulation ;	
	datesOfSimulation.push_back(0.5) ; datesOfSimulation.push_back(1) ; 
	std::vector<size_t>			discretizationBetweenDates ;
	discretizationBetweenDates.push_back(10) ; discretizationBetweenDates.push_back(5) ;

	MC_Cheyette MC_Cheyette_Test(modele_test_PTR, rnGenerator, fwdProbaT, datesOfSimulation, discretizationBetweenDates) ;


	std::vector<double> x_t = MC_Cheyette_Test.get_x_t_Cheyette_() ;
	std::vector<double> y_t = MC_Cheyette_Test.get_y_t_Cheyette_() ;

	//DEBUG
	for (size_t i = 0 ; i < x_t.size() ; ++i)    
	{			
		std::cout << "x_t " << i << " : " << x_t[i] << ", y_t " << i << " : " << y_t[i] << std::endl ;
	}


}

//void test_McLmm_for_Caplet( bool terminalOrSpotProb, 
//						MCSchemeType::MCSchemeType mcSchemeType,
//						size_t minNbSimulation, 
//						size_t maxNbSimulation,
//						double bnSimualtionPowBase,
//						//! output
//						std::vector<size_t>& nbSimulationVector,  // output
//						std::vector<double>& capletResultVector, // output
//						double& analytical_capletPrice)              // output
////{
//
//	unsigned long seed = 47;
//	RNGenerator_PTR  rnGenerator(new McGenerator(seed));
//	VanillaCaplet_PTR vanillaCaplet = get_VanillaCaplet_PTR();
//
//
//	Tenor  tenorType          = Tenor::_6M;
//	size_t horizonYear        = 15; 
//	double liborsInitValueFwd = 0.04;
//	double shift              = 0.0;
//	TEST::TEST_ShiftedLmmAndMcLmm testHelper =TEST::TEST_ShiftedLmmAndMcLmm(tenorType, 
//																			horizonYear,						   
//																			liborsInitValueFwd,
//																			terminalOrSpotProb,
//																			mcSchemeType,
//																			shift);
//	Lmm_PTR   lmm = testHelper.get_lmm();
//	McLmm_PTR mclmm = testHelper.get_mcLmm();
//	std::vector<double> liborsInitValue = testHelper.get_liborsInitValues();
//
//
//	//std::vector<double> liborsInitValue;
//	//Lmm_PTR lmm = get_lmm_for_Caplet(liborsInitValue); // init also the libor. 
//	//McLmm_PTR mclmm_ptr = get_McLmm_for_Caplet(TerminalOrSpotProb, mcSchemeType,lmm, liborsInitValue);
//
//	McLmmVanillaCapletPricer mcLmmVanillaCapletPricer(mclmm);
//
//	//! analytical swaptionApprox price
//	LmmVanillaCapletPricer lmmVanillaCapletPricer(lmm); // YY: maybe a problem of polymorphisms for smart_pointer ??? 
//	analytical_capletPrice = lmmVanillaCapletPricer.price(*vanillaCaplet, liborsInitValue);
//	std::cout << "analyticalCapletPrice = "<< analytical_capletPrice <<  std::endl;
//	double analytical_impliedVol = lmmVanillaCapletPricer.convertPriceToBlackVol(*vanillaCaplet,liborsInitValue,analytical_capletPrice);
//	std::cout << "analytical_impliedVol = "<< analytical_impliedVol <<  std::endl;
//
//	//! MC FRA price, TODO: seems a bad thing, these vectors.
//	nbSimulationVector.clear();
//	capletResultVector.clear();
//
//	for(size_t i=0;; ++i)
//	{
//		size_t nbSimulation = (size_t)std::pow(bnSimualtionPowBase,(int)i)*minNbSimulation;
//		if(nbSimulation>maxNbSimulation)
//			break;
//		double mcPrice = mcLmmVanillaCapletPricer.price(*vanillaCaplet, nbSimulation);
//
//		std::cout << "mc_price = "<< mcPrice <<  std::endl;
//		double mc_impliedVol = lmmVanillaCapletPricer.convertPriceToBlackVol(*vanillaCaplet,liborsInitValue,mcPrice);
//		std::cout << "mc_analytical_impliedVol = "<< mc_impliedVol <<  std::endl;
//
//
//		nbSimulationVector.push_back(nbSimulation);
//		capletResultVector.push_back(mcPrice);
//		mcLmmVanillaCapletPricer.resetGeneratorToinitSeed(); // for the next generation begin with the same seed as this one...
//	}
//}
//
//

