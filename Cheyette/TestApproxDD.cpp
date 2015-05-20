#include <Cheyette\TestApproxDD.h>

CheyetteDD_Model::CheyetteDD_Parameter createCheyetteDD_Parameter()
{
	std::vector<double> x, y ;
	x.push_back(0) ; x.push_back(0.5) ; x.push_back(1) ; 
	y.push_back(0.25) ; y.push_back(0.5) ;

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

void testSwap()
{
	double strike          = 0.04;
	LMM::Index  indexStart = 2; 
	LMM::Index  indexEnd   = 8; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 10) );
	VanillaSwap swap = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
	
	std::vector<LMM::Index> vect_fixedLegPaymentIndexSchedule, vect_floatLegPaymentIndexSchedule ;
	vect_fixedLegPaymentIndexSchedule = swap.get_fixedLegPaymentIndexSchedule();
	vect_floatLegPaymentIndexSchedule = swap.get_floatingLegPaymentIndexSchedule() ;
	std::cout << "indices des flux fixes" << std::endl ;
	for (int i= 0 ; i < vect_fixedLegPaymentIndexSchedule.size() ; ++i)
	{
		std::cout << vect_fixedLegPaymentIndexSchedule[i] << std::endl ;
	}		//retourne 2 4 6 8
	std::cout << "indices des flux flottants" << std::endl ;
	for (int i= 0 ; i < vect_floatLegPaymentIndexSchedule.size() ; ++i)
	{
		std::cout << vect_floatLegPaymentIndexSchedule[i] << std::endl ;
	}		//retourne 1 2 3 4 5 6 7 8

}


void test_y_barre(double t)
{
	//integrale main
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

	double res_integrale ;
	double domaine1 = y[0] * y[0] * (exp(2*k*std::min(0.5, t)) -               1          )/(2 * k) ;
	double domaine2 = y[1] * y[1] * (exp(2*k*std::min(1.0, t)) - exp(2*k*std::min(0.5, t)))/(2 * k) ;
	res_integrale = exp(- 2 * k * t) *(domaine1 + domaine2) ; 
	
	//integrale 	CheyetteDD_VanillaSwaptionApproxPricer
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

	CheyetteDD_VanillaSwaptionApproxPricer abc = CheyetteDD_VanillaSwaptionApproxPricer(modele_test_PTR, vs); 
	double integrale = abc.calculate_y_bar(t) ;

	std::cout << "integrale_main" << res_integrale << std::endl ;
	std::cout << "integrale_classe" << integrale << std::endl ;
	
	std::cout << "annuite" << std::endl ;
	//std::cout <<  modele_test_PTR->annuity(w) << std::endl ;
	//std::cout <<  exp(- 0.85/100) + exp(- 2 * 0.9/100) + exp(-3 * 0.92/100) + exp(-4 * 0.95/100) << std::endl ;

	std::cout << abc.swapRateDenominator_1stDerivative(0, 0) << std::endl ;
	std::cout <<    - (1 - exp(- 1)) * exp(- 0.85/100) 
					- (1 - exp(- 2)) * exp(- 2 * 0.9/100) 
					- (1 - exp(- 3)) * exp(- 3 * 0.92/100) 
					- (1 - exp(- 4)) * exp(- 4 * 0.95/100) << std::endl ;
}

void test_Derivative_ZC()
{
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

	std::vector<double> x, y ;
	x.push_back(0) ; x.push_back(0.5) ; x.push_back(1) ; 
	y.push_back(0.25) ; y.push_back(0.5) ;
	piecewiseconst_RR_Function sigma = piecewiseconst_RR_Function(x, y) ; 
	piecewiseconst_RR_Function m = piecewiseconst_RR_Function(x, y) ; 

	double k(1) ;

	CheyetteDD_Model::CheyetteDD_Parameter monStruct = CheyetteDD_Model::CheyetteDD_Parameter(k, sigma, m) ;
	CheyetteDD_Model_PTR modele_test_PTR(new CheyetteDD_Model(courbe_PTR_test, monStruct)) ;

	double strike          = 0.04;
	LMM::Index  indexStart = 2 ; //indice 1er flux
	LMM::Index  indexEnd   = 8 ; //indice fernier flux
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 50) );
	VanillaSwap swap = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
	
	VanillaSwaption_PTR swaption_ptr(new VanillaSwaption(swap, OptionType::OptionType::CALL)) ;

	CheyetteDD_VanillaSwaptionApproxPricer approx = CheyetteDD_VanillaSwaptionApproxPricer(modele_test_PTR, swaption_ptr); 

	//std::cout << "derivee_1_classe T = 5Y   " << approx.ZC_1stDerivative_on_xt(0, 5, 0) << std::endl ;
	//std::cout << "derivee_1_main   T = 5Y   "  << -exp(- 0.01*5) * (1 - exp(-5)) << std::endl ;
	//std::cout << "derivee_1_classe T = 15Y  " << approx.ZC_1stDerivative_on_xt(0, 15.0, 0) << std::endl ;
	//std::cout << "derivee_1_main   T = 15Y  "  << -exp(- 0.02*15) * (1 - exp(-15)) << std::endl ;
	//std::cout << "derivee_1_classe T = 1Y   " << approx.ZC_1stDerivative_on_xt(0, 1, 0) << std::endl ;
	//std::cout << "derivee_1_main   T = 1Y   "  << -exp(- 0.85/100) * (1 - exp(-1)) << std::endl ;
	//std::cout << "   " << std::endl ;
	//std::cout << "derivee_2_classe T = 1Y   " << approx.ZC_2ndDerivative_on_xt(0, 1, 0) << std::endl ;
	//std::cout << "derivee_2_main   T = 1Y   "  << exp(- 0.85/100) * pow(1 - exp(-1),2) << std::endl ;
	//std::cout << "   " << std::endl ;
	//std::cout << "swap rate numerator   " << approx.swapRateNumerator(0, 0) << std::endl ;
	//std::cout << "swap rate numerator   " << 1 - exp(- 20 * 2.5/100) << std::endl ;    //indexEnd : 40, et tenor : 0.5, donc T_N = 20Y

	std::cout << " " << std::endl ;
	std::cout << "------------  ZC  ------------------------------------------" << std::endl ;
	std::cout << " " << std::endl ;

	std::cout << "P(0.5, 1, 0.1, 0.1)  " << modele_test_PTR->P(0.5, 1, 0.1, 0.1)  << std::endl ;
	double g = 1 - exp(-0.5) ;
	std::cout << exp(- 0.85/100) / exp(- 0.825/100 * 1/2) * exp(- 0.1 * g - 1/2 * 0.1 * g * g) ;
	std::cout << " " << std::endl ;

	std::cout << "P(0.5, 1, 1, 1)      " << modele_test_PTR->P(0.5, 1, 1, 1)  << std::endl ;
	std::cout << exp(- 0.85/100) / exp(- 0.825/100 * 1/2) * exp(- g - 1/2 * g * g) ;
	std::cout << " " << std::endl ;

	std::cout << "P(0.5, 1, 10, 10)    " << modele_test_PTR->P(0.5, 1, 10, 10)  << std::endl ;
	std::cout << exp(- 0.85/100) / exp(- 0.825/100 * 1/2) * exp(- 10 * g - 1/2 * 10 * g * g) ;
	std::cout << " " << std::endl ;

	std::cout << "P(2, 10, 1, 1)    " << modele_test_PTR->P(2, 10, 1, 1)  << std::endl ;
	g = 1 - exp(- 8) ;
	std::cout << exp(- 1.5/100 * 10) / exp(- 0.9/100 * 2) * exp(- 1 * g - 1/2 * 1 * g * g) ;


	std::cout << " " << std::endl ;
	std::cout << "------------  test swap rate  ------------------------------" << std::endl ;
	std::cout << " " << std::endl ;

	std::cout << "buffer T0 " << approx.get_buffer_T0_() << std::endl ;  //1er flux
	std::cout << "buffer TN " << approx.get_buffer_TN_() << std::endl ;  //dernier flux

	// t = 0.5
	double y_barre_t = approx.calculate_y_bar(0.5) ;
	double ge1 = 1 - exp(- 0.5) ;

	double PtT0 = exp( - 1 * 0.85/100) / exp( - 0.5 * 0.825/100) * exp(- 0.2 * ge1 - 1/2 * y_barre_t * ge1 * ge1) ;
	double ge2 = 1 - exp(- 3.5) ;
	double PtTN = exp( - 4 * 0.95/100) / exp( - 0.5 * 0.825/100) * exp(- 0.2 * ge2 - 1/2 * y_barre_t * ge2 * ge2) ;

	//flux fixe 2Y
	double ge3 = 1 - exp(- (2 - 0.5)) ;
	double PtT2Y = exp( - 2 * 0.9/100) / exp( - 0.5 * 0.825/100) * exp(- 0.2 * ge3 - 1/2 * y_barre_t * ge3 * ge3) ;
	//flux fixe 3Y
	double ge4 = 1 - exp(- (3 - 0.5)) ;
	double PtT3Y = exp( - 3 * 0.92/100) / exp( - 0.5 * 0.825/100) * exp(- 0.2 * ge4 - 1/2 * y_barre_t * ge4 * ge4) ;

	std::cout << "swapRate(0.5, 0.2) : " << approx.swapRate(0.5, 0.2) << std::endl ;
	std::cout << (PtT0 - PtTN) / (PtT2Y + PtT3Y + PtTN) << std::endl ;



	std::cout << " " << std::endl ;
	std::cout << "------------  test swap rate 1st derivative  --------------" << std::endl ;
	std::cout << " " << std::endl ;




	std::cout << " " << std::endl ;
	std::cout << "-----------  test fonction inverse  -----------------------" << std::endl ;
	std::cout << " " << std::endl ;
	std::cout << "swapRate(0.5, 2) : " << approx.swapRate(0.5, 2) << std::endl ;
	std::cout << "inverse : " << approx.inverse(1, approx.swapRate(0.5, 2)) << " vs 2" << std::endl ;
	std::cout << "swapRate(0.5, 0.2) : " << approx.swapRate(0.5, 0.2) << std::endl ;
	std::cout << "inverse : " << approx.inverse(1, approx.swapRate(0.5, 0.2)) << " vs 0.2" << std::endl ;
	std::cout << "swapRate(1, 0.5) : " << approx.swapRate(1, 0.5) << std::endl ;
	std::cout << "inverse : " << approx.inverse(1, approx.swapRate(1, 0.5)) << " vs 0.5" << std::endl ;
	std::cout << "swapRate(1, 5) : " << approx.swapRate(1, 5) << std::endl ;
	std::cout << "inverse : " << approx.inverse(1, approx.swapRate(1, 5)) << " vs 5" << std::endl ;
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