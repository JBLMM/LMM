//#include <LMM/Test/Tests.h>
#include <stdio.h>

#include <Cheyette/TestApproxDD.h>
#include <Cheyette/CheyetteDD_Model.h>
//#include <Cheyette/CheyetteDD_VanillaSwaptionApproxPricer.h>




int main()
{
	
	//test_beginner();							//test_JB
	//vanillaSwapComparaisonExemple();			//test_JB		
	//Test_McGeneticSwapLMMPricer();			//test_JB
	//createDDModel_xyz() ;
	
	/************    tests JL   ************************/
	//createSwap() ;
	//createDDModel() ;
	//test_y_barre(0) ;
	//test_y_barre(0.5) ;
	//test_y_barre(1.0) ;
	test_Derivative_ZC() ;

	//test_Integrator1D();

	//testSwap() ; 

	//test_Integrator1D();

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