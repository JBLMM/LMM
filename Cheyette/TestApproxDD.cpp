#include "TestApproxDD.h"
#include "CheyetteModel.h"

#include <iostream>

//creer une instance de CheyetteModel 
//avec vol locale piecewiseconst_DD_R2R_Function

//creation d'un swap

void createSwap()
{
	double strike          = 0.04;
	LMM::Index  indexStart = 1; 
	LMM::Index  indexEnd   = 2; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_12M; //Tenor::_1YR;
//	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 5) );   //OK
//	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_1YR , 5) );   //PAS OK !!
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 2) );   //OK
	
	VanillaSwap w = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);


	std::cout << "get_EndDate " << simulationStructure->get_EndDate() << std::endl ;
	std::cout << "get_horizon " << simulationStructure->get_horizon() << std::endl ;
	std::cout << "get_nbLIBOR " << simulationStructure->get_nbLIBOR() << std::endl ;
	std::cout << "get_tenorType " << simulationStructure->get_tenorType() << std::endl ;
	
	
}



VanillaSwaption createSwaption()
{
	double strike          = 0.04;
	LMM::Index  indexStart = 16; 
	LMM::Index  indexEnd   = 28; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 15) );
	VanillaSwap w = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
	
	VanillaSwaption x = VanillaSwaption(w, OptionType::OptionType::CALL);
	return (x) ;
}

void TestApprox()
{
	piecewiseconst_RR_Function chi0 ; 
	piecewiseconst_DD_R2R_Function sigma0 ;
	double x0(0), y0(0) ;

	CheyetteModel::Cheyette_Parameter CheyetteParam = initStruct(chi0, sigma0) ;
	courbeInput_PTR courbe_PTR( new courbeInput() ) ;
	Cheyette_PTR Cheyette_Model_Test_PTR( new CheyetteModel(courbe_PTR, CheyetteParam, x0, y0) );
	
	double t = 0 ;// date valo 
	double y_bar = 0.2 ; /// .................................  
	double ExpansionPoint_s = 2 ; // ............................

	VanillaSwaption s = createSwaption() ;
	boost::shared_ptr<VanillaSwaption> swaption_PTR( new VanillaSwaption() );

	CheyetteVanillaSwaptionApproxPricer_Piterbarg DD_approx = 
									CheyetteVanillaSwaptionApproxPricer_Piterbarg(Cheyette_Model_Test_PTR, 
																					swaption_PTR,
																					t, 
																					y_bar, 
																					ExpansionPoint_s);
}









