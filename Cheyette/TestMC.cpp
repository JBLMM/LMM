#include "TestMC.h"

void UneTrajectoireEuler()
{
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
	CheyetteDD_Model::CheyetteDD_Parameter monStruct = CheyetteDD_Model::CheyetteDD_Parameter(k, sigma, m) ;
	CheyetteDD_Model_PTR modele_test_PTR(new CheyetteDD_Model(courbe_PTR_test, monStruct)) ;

	double strike          = 0.04;
	LMM::Index  indexStart = 0; 
	LMM::Index  indexEnd   = 8; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 15) );
	VanillaSwap w = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
	
	VanillaSwaption_PTR vs(new VanillaSwaption(w, OptionType::OptionType::CALL)) ;

	//RNGenerator_PTR rnGen(new RNGenerator(1234)); 

	//qG_VanillaSwaptionMCPricer MCPricer = qG_VanillaSwaptionMCPricer(modele_test_PTR, vs, rnGen);

	//MCpricer.
}