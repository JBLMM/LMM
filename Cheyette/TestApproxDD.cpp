#include <Cheyette\TestApproxDD.h>

CheyetteDD_Model::CheyetteDD_Parameter createCheyetteDD_Parameter()
{
	std::vector<double> x ;
	x.push_back(0) ; x.push_back(0.4) ; x.push_back(0.8) ; x.push_back(1.0) ;
	std::vector<double> y ;
	y.push_back(0.25) ; y.push_back(0.5) ; y.push_back(0.25) ; 

	std::cout << "creation d'une fonction piecewisecont sigma" << std::endl ;
	piecewiseconst_RR_Function sigma = piecewiseconst_RR_Function(x, y) ; 

	std::cout << "creation d'une fonction piecewisecont m_" << std::endl ;
	piecewiseconst_RR_Function m = piecewiseconst_RR_Function(x, y) ; 

	double k = 2.0 ;

	CheyetteDD_Model::CheyetteDD_Parameter monStruct = CheyetteDD_Model::CheyetteDD_Parameter(k, sigma, m) ;
	std::cout << "OK" << std::endl ;
	return monStruct ;
}

CourbeInput createCourbeInput()
{
	std::vector<double> listeMatu, tauxZC ;
	listeMatu.push_back(0) ; listeMatu.push_back(1) ; listeMatu.push_back(2) ; listeMatu.push_back(3) ; 
	listeMatu.push_back(4) ; listeMatu.push_back(5) ; listeMatu.push_back(10) ; listeMatu.push_back(15) ; 
	listeMatu.push_back(20) ; listeMatu.push_back(25) ;
	tauxZC.push_back(0.8/100) ; tauxZC.push_back(0.85/100) ; tauxZC.push_back(0.9/100) ; tauxZC.push_back(0.92/100) ; 
	tauxZC.push_back(0.95/100) ; tauxZC.push_back(1.00/100) ; tauxZC.push_back(1.5/100) ; tauxZC.push_back(2/100) ; 
	tauxZC.push_back(2.5/100) ; tauxZC.push_back(2.3/100) ;

	return CourbeInput(listeMatu, tauxZC) ;
}

void createDDModel()
{
	std::vector<double> listeMatu, tauxZC ;
	listeMatu.push_back(0) ; listeMatu.push_back(1) ; listeMatu.push_back(2) ; listeMatu.push_back(3) ; 
	listeMatu.push_back(4) ; listeMatu.push_back(5) ; listeMatu.push_back(10) ; listeMatu.push_back(15) ; 
	listeMatu.push_back(20) ; listeMatu.push_back(25) ;
	tauxZC.push_back(0.8/100) ; tauxZC.push_back(0.85/100) ; tauxZC.push_back(0.9/100) ; tauxZC.push_back(0.92/100) ; 
	tauxZC.push_back(0.95/100) ; tauxZC.push_back(1.00/100) ; tauxZC.push_back(1.5/100) ; tauxZC.push_back(2/100) ; 
	tauxZC.push_back(2.5/100) ; tauxZC.push_back(2.3/100) ;

	CheyetteDD_Model::CheyetteDD_Parameter CheyetteParam_test = createCheyetteDD_Parameter() ;

	courbeInput_PTR courbe_PTR_test(new CourbeInput(listeMatu, tauxZC));
	CheyetteDD_Model cheyetteModel_test = CheyetteDD_Model(courbe_PTR_test, CheyetteParam_test) ;


}


void createSwap()
{
	double strike          = 0.04;
	LMM::Index  indexStart = 0; 
	LMM::Index  indexEnd   = 2; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_12M; //Tenor::_1YR;
//	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 5) );   
//	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_1YR , 5) );  
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 2) );   
	
	VanillaSwap w = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);


	std::cout << "get_EndDate " << simulationStructure->get_EndDate() << std::endl ;
	std::cout << "get_horizon " << simulationStructure->get_horizon() << std::endl ;
	std::cout << "get_nbLIBOR " << simulationStructure->get_nbLIBOR() << std::endl ;
	std::cout << "get_tenorType " << simulationStructure->get_tenorType() << std::endl ;

	std::vector<double> vect_tenor_Dates = simulationStructure->get_tenorDate() ;
	for (int i = 0 ; i < vect_tenor_Dates.size() ; ++i)
	{
		std::cout << "vect tenor dates i = " << i << "  " << vect_tenor_Dates[i] << std::endl ;
	} 

	std::vector<double> vect_delta_tau = simulationStructure->get_deltaT() ;
	for (int i = 0 ; i < vect_delta_tau.size() ; ++i)
	{
		std::cout << "vect delta tau i = " << i << "  " << vect_delta_tau[i] << std::endl ;
	} 

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
	return x ;
}

//void TestApprox()
//{
//	piecewiseconst_RR_Function chi0 ; 
//	piecewiseconst_DD_R2R_Function sigma0 ;
//	double x0(0), y0(0), S0(1) ;
//
//	courbeInput_PTR courbe_PTR( new CourbeInput() ) ;
//	Cheyette_PTR Cheyette_Model_Test_PTR( new CheyetteDD_Model(courbe_PTR, CheyetteParam, x0, y0, S0) );
//	
//	double t = 0 ;// date valo 
//	double y_bar = 0.2 ; /// .................................  
//	double ExpansionPoint_s = 2 ; // ............................
//	double s_bar = 1 ;
//	double Phi_t_s_bar = 1 ;
//
//	VanillaSwaption s = createSwaption() ;
//	boost::shared_ptr<VanillaSwaption> swaption_PTR( new VanillaSwaption() );
//
//	CheyetteVanillaSwaptionApproxPricer_Piterbarg DD_approx = 
//									CheyetteVanillaSwaptionApproxPricer_Piterbarg(Cheyette_Model_Test_PTR, 
//																					swaption_PTR,
//																					t, 
//																					y_bar, 
//																					ExpansionPoint_s, 
//																					s_bar, 
//																					Phi_t_s_bar);
//}