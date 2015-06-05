#include <LMM/Test/Tests.h>
#include <stdio.h>



#include <Cheyette/unit_test/TestFonction.h>
#include <Cheyette/unit_test/TestIntegrator1D_2D.h>
#include <Cheyette/unit_test/TestApproxDD.h>
#include <Cheyette/CheyetteModel/CheyetteDD_Model.h>
#include <Cheyette/Pricer/CheyetteDD_VanillaSwaptionMCPricer.h>
#include <LMM/numeric/Integrator1D.h>
#include <Cheyette/unit_test/TestMC.h>



int main()
{

	//test_beginner();							//test_JB
	//vanillaSwapComparaisonExemple();			//test_JB		

	Test_McGeneticSwapLMMPricer();			//test_JB
	//Test_McGeneticTargetSwapLMMPricing();		//test_JB


	//test_Integrator1D();

	//createDDModel_xyz() ;
	
	/************    tests JL   ************************/
	//TestFonctionConstanteMorceaux() ;
	//TestInterpolation_RR_Function() ;
	//TestCompositionMultiplicationFonction() ;

	//TestIntegrator1D_Riemann() ;
	//TestIncrementalIntegrator1D_Riemann() ;

	//CheyetteDD_VanillaSwaptionApproxPricer_PTR test = createApproxPricer_PTR() ;

//a reprendre : 
	//test_Derivative_ZC() ;  //passage par valeur ?!

	//TestIncrementalIntegrator2D_Riemann() ;    //attention probleme lors de 2 appels consecutifs à la fonction integrate
	//std::cout << "pb lors de 2 appels consecutifs a integrale" << std::endl ;

	//createSwap() ;
	//createDDModel() ;
	//test_y_barre(0) ;
	//test_y_barre(0.5) ;
	//test_y_barre(1.0) ;
	//test_Derivative_ZC() ;
	
	//test_time_average() ; 
	//test_y_bar_cas_limite() ;
	//test_Integrator1D();

	//testSwap() ; 

	//test_incremental_integrale() ;

	//UneTrajectoireEuler() ;


	/************  tests LMM   *************************/
	//test_Noise();  
	//test_HGVolatility();  
	//test_Functional(); 
	//test_BlackGreek(); 
	//test_McTerminalLmm(); 

	//test_VanillaFRAPricer();  
	//test_VanillaCapletPricer(); 
	//test_VanillaSwapPricer();  
	//test_VanillaSwaptionPricer();    
	//test_VanialSwaptionPricer_MCvsApprox(); 


	//test_Noise();  
	//test_HGVolatility();  
	//test_Functional(); 
	//test_BlackGreek(); 
	//test_McTerminalLmm(); 

	//test_VanillaFRAPricer();  
	//test_VanillaCapletPricer(); 
	//test_VanillaSwapPricer();  
	//test_VanillaSwaptionPricer();    
	//test_VanialSwaptionPricer_MCvsApprox(); 

	//test_SwaptionMarketDataContainer();		
	//test_CalibrationWithATMSwaptionVol();		
	//test_CalibrationShiftWithSwaptionVolSkew();
	
	//test_LmmVirtualCorrelationCalibration();  
	//test_LmmCalibrationConfig(); 
	//test_LmmGnuplotPrinterMatrix();  
	//test_Interpolation();  
	//test_GMatrixMapping(); 
	//test_UpperTriangleVanillaSwaptionQuotes(); 
	//test_LmmSwaptionMarketData(); 
	//test_LmmABCDFinder();	
	//test_LmmVirtualCalibration();	
	//test_LmmCalibrationSensitivity();	
	//test_LmmCalibrationMarketData();	
	//test_LmmRegularizedCalibrationMarketData();   

	getchar();
}