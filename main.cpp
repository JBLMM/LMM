#include <LMM/Test/Tests.h>
#include <stdio.h>




int main()
{
	//-------------------------------------------test JB  ------------------------------------------
	//test_beginner();
	//vanillaSwapComparaisonExemple();
	//Test_McGeneticSwapLMMPricer();


	test_Integrator1D();


	//test_Noise();  //------------------------------------Bon SC
	//test_HGVolatility();  //--------------------------------------Bug: LNK2019 and 1120 
	//test_Functional(); //----------------------------------Bon
	//test_BlackGreek(); //----------------------------------Bon
	//test_McTerminalLmm(); //-------------------------------Condition not implemented yet

	//test_VanillaFRAPricer();  // TODO: test the drift !=0  case. ---------------------------Bon
	//test_VanillaCapletPricer(); // bug!	 ----------------------?
	//test_VanillaSwapPricer();  //-----------------------------bon
	//test_VanillaSwaptionPricer(); // bug!    ---------------------?
	//test_VanialSwaptionPricer_MCvsApprox(); //-------------------------------Condition not implemented yet

	//test_SwaptionMarketDataContainer();		//-----------------------------ne compile pas
	//test_CalibrationWithATMSwaptionVol();		//-----------------------------ne compile pas
	//test_CalibrationShiftWithSwaptionVolSkew(); //-----------------------------ne compile pas
	
	//test_LmmVirtualCorrelationCalibration();  //--------------------------------debug assertion failed
	//test_LmmCalibrationConfig(); //----------------------------------------pas de contenu
	//test_LmmGnuplotPrinterMatrix();  //--------------------------------------pas de contenu
	//test_Interpolation();   //------------------------------------------------pas de contenu
	//test_GMatrixMapping();  //------------------------------------------------ne compile pas
	//test_UpperTriangleVanillaSwaptionQuotes();//------------------------------------------------ne compile pas
	//test_LmmSwaptionMarketData();//------------------------------------------------ne compile pas
	//test_LmmABCDFinder();	//-----------------------------------------------------Erreur sur E:\\(chemin pour trouver les donnee input)
	//test_LmmVirtualCalibration();	//--------------------------------------------------bon
	//test_LmmCalibrationSensitivity();	// -------------------------------------------mouvais input chemin // bon
	//test_LmmCalibrationMarketData();	// -------------------------------------------mouvais input chemin  // bon
	//test_LmmRegularizedCalibrationMarketData();   //--------------------------------sans contenu     // bon

	getchar();
}