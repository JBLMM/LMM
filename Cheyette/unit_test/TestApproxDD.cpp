#include <Cheyette\unit_test\TestApproxDD.h>



courbeInput_PTR createCourbeInput()
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

	return courbe_PTR_test ;
}

CheyetteDD_Model_PTR createCheyetteDD_Model()
{
	courbeInput_PTR courbe_PTR_test(createCourbeInput());

	std::vector<double> x, sigma_y, m_y, shift_y ;
	x.push_back(0) ; x.push_back(1) ; x.push_back(2) ; 
	m_y.push_back(0.25) ; m_y.push_back(0.5) ;
	sigma_y.push_back(0.25) ; sigma_y.push_back(0.5) ;
	shift_y.push_back(0.25) ; shift_y.push_back(0.5) ;

	piecewiseconst_RR_Function sigma = piecewiseconst_RR_Function(x, sigma_y) ; 
	piecewiseconst_RR_Function m = piecewiseconst_RR_Function(x, m_y) ; 
	piecewiseconst_RR_Function shift = piecewiseconst_RR_Function(x, shift_y) ; 
	double k = 0.5 ;

	CheyetteDD_Model_PTR cheyetteDD_Model_PTR_Test(new CheyetteDD_Model( courbe_PTR_test, 
															CheyetteDD_Model::CheyetteDD_Parameter(k, sigma, m, shift) )) ;
	cheyetteDD_Model_PTR_Test->show() ;
	return cheyetteDD_Model_PTR_Test ;
}

VanillaSwaption_PTR createSwap()
{
	double strike          = 0.04;
	LMM::Index  indexStart = 0; 
	LMM::Index  indexEnd   = 2; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_12M;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 2) );   
	
	VanillaSwap swap = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);

	std::cout << "get_EndDate " << simulationStructure->get_EndDate() << std::endl ;
	std::cout << "get_horizon " << simulationStructure->get_horizon() << std::endl ;
	std::cout << "get_nbLIBOR " << simulationStructure->get_nbLIBOR() << std::endl ;
	std::cout << "get_tenorType " << simulationStructure->get_tenorType() << std::endl ;

	std::vector<LMM::Index> vect_fixedLegPaymentIndexSchedule, vect_floatLegPaymentIndexSchedule ;
	vect_fixedLegPaymentIndexSchedule = swap.get_fixedLegPaymentIndexSchedule();
	vect_floatLegPaymentIndexSchedule = swap.get_floatingLegPaymentIndexSchedule() ;
	std::cout << "indices des flux fixes" << std::endl ;
	for (size_t i= 0 ; i < vect_fixedLegPaymentIndexSchedule.size() ; ++i)
	{
		std::cout << vect_fixedLegPaymentIndexSchedule[i] << std::endl ;
	}		//retourne 2 4 6 8
	std::cout << "indices des flux flottants" << std::endl ;
	for (size_t i= 0 ; i < vect_floatLegPaymentIndexSchedule.size() ; ++i)
	{
		std::cout << vect_floatLegPaymentIndexSchedule[i] << std::endl ;
	}		//retourne 1 2 3 4 5 6 7 8

	std::vector<double> vect_tenor_Dates = simulationStructure->get_tenorDate() ;
	for (size_t i = 0 ; i < vect_tenor_Dates.size() ; ++i)
	{
		std::cout << "vect tenor dates i = " << i << "  " << vect_tenor_Dates[i] << std::endl ;
	} 

	std::vector<double> vect_delta_tau = simulationStructure->get_deltaT() ;
	for (size_t i = 0 ; i < vect_delta_tau.size() ; ++i)
	{
		std::cout << "vect delta tau i = " << i << "  " << vect_delta_tau[i] << std::endl ;
	} 
	VanillaSwaption_PTR VanillaSwaption_PTR_Test(new VanillaSwaption(swap, OptionType::OptionType::CALL)) ;

	return VanillaSwaption_PTR_Test ;
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
	for (size_t i= 0 ; i < vect_fixedLegPaymentIndexSchedule.size() ; ++i)
	{
		std::cout << vect_fixedLegPaymentIndexSchedule[i] << std::endl ;
	}		//retourne 2 4 6 8
	std::cout << "indices des flux flottants" << std::endl ;
	for (size_t i= 0 ; i < vect_floatLegPaymentIndexSchedule.size() ; ++i)
	{
		std::cout << vect_floatLegPaymentIndexSchedule[i] << std::endl ;
	}		//retourne 1 2 3 4 5 6 7 8

}


VanillaSwaption_PTR createSwaption()
{
	double strike          = 0.04;
	LMM::Index  indexStart = 2; 
	LMM::Index  indexEnd   = 6; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 15) );
	VanillaSwap swap = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
	
	VanillaSwaption_PTR VanillaSwaption_PTR_Test(new VanillaSwaption(swap, OptionType::OptionType::CALL)) ;
	VanillaSwaption_PTR_Test->show() ;

	return VanillaSwaption_PTR_Test ;
}

CheyetteDD_VanillaSwaptionApproxPricer_PTR createApproxPricer_PTR()
{
	CheyetteDD_Model_PTR	CheyetteDD_Model_Test_PTR(createCheyetteDD_Model()) ;
	VanillaSwaption_PTR		VanillaSwaption_Test_PTR(createSwaption()) ;
	CheyetteDD_VanillaSwaptionApproxPricer_PTR ApproxPricerTest_PTR(
							new CheyetteDD_VanillaSwaptionApproxPricer(	CheyetteDD_Model_Test_PTR, 
																		VanillaSwaption_Test_PTR)); 
	return ApproxPricerTest_PTR ;
}


void test_Derivative_ZC()
{
	//ne marche pas : 
	//comme un passage par valeur / variable locale
	//CheyetteDD_VanillaSwaptionApproxPricer_PTR ApproxPricerTest_PTR(createApproxPricer_PTR()) ;
	//CheyetteDD_Model_CONSTPTR modele_test_PTR = ApproxPricerTest_PTR->get_CheyetteDD_Model() ;

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

	std::vector<double> x, sigma_y, m_y, shift_y ;
	x.push_back(0) ; x.push_back(1) ; x.push_back(2) ; 
	m_y.push_back(0.25) ; m_y.push_back(0.5) ;
	sigma_y.push_back(0.25) ; sigma_y.push_back(0.5) ;
	shift_y.push_back(0.25) ; shift_y.push_back(0.5) ;

	piecewiseconst_RR_Function sigma = piecewiseconst_RR_Function(x, sigma_y) ; 
	piecewiseconst_RR_Function m = piecewiseconst_RR_Function(x, m_y) ; 
	piecewiseconst_RR_Function shift = piecewiseconst_RR_Function(x, shift_y) ; 
	double k = 0.5 ;

	CheyetteDD_Model_PTR cheyetteDD_Model_PTR_Test(new CheyetteDD_Model( courbe_PTR_test, 
															CheyetteDD_Model::CheyetteDD_Parameter(k, sigma, m, shift) )) ;
	cheyetteDD_Model_PTR_Test->show() ;


	double strike          = 0.04;
	LMM::Index  indexStart = 2; 
	LMM::Index  indexEnd   = 6; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 15) );
	VanillaSwap swap = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
	
	VanillaSwaption_PTR VanillaSwaption_PTR_Test(new VanillaSwaption(swap, OptionType::OptionType::CALL)) ;
	VanillaSwaption_PTR_Test->show() ;

	CheyetteDD_VanillaSwaptionApproxPricer_PTR ApproxPricerTest_PTR(
		new CheyetteDD_VanillaSwaptionApproxPricer(cheyetteDD_Model_PTR_Test, VanillaSwaption_PTR_Test)) ;
	
//t = 0 avec la courbe spot
	std::cout << "derivee_1_classe T = 5Y   " << ApproxPricerTest_PTR->ZC_1stDerivative_on_xt(0., 5.0, 0.) << std::endl ;
	std::cout << "derivee_1_main   T = 5Y   "  << -exp(- 0.01*5) * (1 - exp(-5)) << std::endl ;
	std::cout << "derivee_1_classe T = 15Y  " << ApproxPricerTest_PTR->ZC_1stDerivative_on_xt(0., 15.0, 0.) << std::endl ;
	std::cout << "derivee_1_main   T = 15Y  "  << -exp(- 0.02*15) * (1 - exp(-15)) << std::endl ;
	std::cout << "derivee_1_classe T = 1Y   " << ApproxPricerTest_PTR->ZC_1stDerivative_on_xt(0., 1.0, 0.) << std::endl ;
	std::cout << "derivee_1_main   T = 1Y   "  << -exp(- 0.85/100) * (1 - exp(-1)) << std::endl ;
	std::cout << "   " << std::endl ;
	std::cout << "derivee_2_classe T = 1Y   " << ApproxPricerTest_PTR->ZC_2ndDerivative_on_xt(0., 1.0, 0.) << std::endl ;
	std::cout << "derivee_2_main   T = 1Y   "  << exp(- 0.85/100) * pow(1 - exp(-1),2) << std::endl ;
	std::cout << "   " << std::endl ;
	std::cout << "swap rate numerator   " << ApproxPricerTest_PTR->swapRateNumerator(0., 0.) << std::endl ;
	std::cout << "swap rate numerator   " << 1 - exp(- 20 * 2.5/100) << std::endl ;    //indexEnd : 40, et tenor : 0.5, donc T_N = 20Y

//t = 0.5
//	std::cout << " " << std::endl ;
//	std::cout << "------------  ZC  ------------------------------------------" << std::endl ;
//	std::cout << " " << std::endl ;
//
//	std::cout << "P(0.5, 1, 0.1, 0.1)  " << modele_test_PTR->P(0.5, 1, 0.1, 0.1)  << std::endl ;
//	double g = 1 - exp(-0.5) ;
//	std::cout << exp(- 0.85/100) / exp(- 0.825/100 * 1/2) * exp(- 0.1 * g - 1/2 * 0.1 * g * g) ;
//	std::cout << " " << std::endl ;
//
//	std::cout << "P(0.5, 1, 1, 1)      " << modele_test_PTR->P(0.5, 1, 1, 1)  << std::endl ;
//	std::cout << exp(- 0.85/100) / exp(- 0.825/100 * 1/2) * exp(- g - 1/2 * g * g) ;
//	std::cout << " " << std::endl ;
//
//	std::cout << "P(0.5, 1, 10, 10)    " << modele_test_PTR->P(0.5, 1, 10, 10)  << std::endl ;
//	std::cout << exp(- 0.85/100) / exp(- 0.825/100 * 1/2) * exp(- 10 * g - 1/2 * 10 * g * g) ;
//	std::cout << " " << std::endl ;
//
////t = 2
//	std::cout << "P(2, 10, 1, 1)    " << modele_test_PTR->P(2, 10, 1, 1)  << std::endl ;
//	g = 1 - exp(- 8) ;
//	std::cout << exp(- 1.5/100 * 10) / exp(- 0.9/100 * 2) * exp(- 1 * g - 1/2 * 1 * g * g) ;
//
//
//	std::cout << " " << std::endl ;
//	std::cout << "------------  test swap rate  ------------------------------" << std::endl ;
//	std::cout << " " << std::endl ;
//
//	std::cout << "buffer T0 " << ApproxPricerTest_PTR->get_buffer_T0_() << std::endl ;  //1er flux
//	std::cout << "buffer TN " << ApproxPricerTest_PTR->get_buffer_TN_() << std::endl ;  //dernier flux
//
//	// t = 0.5
//	double y_barre_t = approx.calculate_y_bar(0.5) ;
//	double ge1 = 1 - exp(- 0.5) ;
//
//	double PtT0 = exp( - 1 * 0.85/100) / exp( - 0.5 * 0.825/100) * exp(- 0.2 * ge1 - 1/2 * y_barre_t * ge1 * ge1) ;
//	double ge2 = 1 - exp(- 3.5) ;
//	double PtTN = exp( - 4 * 0.95/100) / exp( - 0.5 * 0.825/100) * exp(- 0.2 * ge2 - 1/2 * y_barre_t * ge2 * ge2) ;
//
//	//flux fixe 2Y
//	double ge3 = 1 - exp(- (2 - 0.5)) ;
//	double PtT2Y = exp( - 2 * 0.9/100) / exp( - 0.5 * 0.825/100) * exp(- 0.2 * ge3 - 1/2 * y_barre_t * ge3 * ge3) ;
//	//flux fixe 3Y
//	double ge4 = 1 - exp(- (3 - 0.5)) ;
//	double PtT3Y = exp( - 3 * 0.92/100) / exp( - 0.5 * 0.825/100) * exp(- 0.2 * ge4 - 1/2 * y_barre_t * ge4 * ge4) ;
//
//	std::cout << "swapRate(0.5, 0.2) : " << approx.swapRate(0.5, 0.2) << std::endl ;
//	std::cout << (PtT0 - PtTN) / (PtT2Y + PtT3Y + PtTN) << std::endl ;
//
//
//
//	std::cout << " " << std::endl ;
//	std::cout << "------------  test swap rate 1st derivative  --------------" << std::endl ;
//	std::cout << " " << std::endl ;
// 
//	std::cout << "swapRate(0.5, 0.2) numerateur 1st deriv :  " << approx.swapRateNumerator_1stDerivative(0.5, 0.2) << std::endl ;
//	std::cout << "swapRate(0.5, 0.2) numerateur deriv mano : " << -(1-exp(- 0.5)) * PtT0 + (1 - exp(-3.5)) * PtTN << std::endl ;
//
//	std::cout << "swapRate(0.5, 0.2) denom 1st deriv :  " << approx.swapRateDenominator_1stDerivative(0.5, 0.2) << std::endl ;
//	std::cout << "swapRate(0.5, 0.2) denom deriv mano : " <<	- (1-exp(- 1.5)) * PtT2Y 
//																- (1-exp(- 2.5)) * PtT3Y 
//																- (1-exp(- 3.5)) * PtTN << std::endl ;
//
//	double n, np ;
//	n = PtT0 - PtTN;
//	np = -(1-exp(- 0.5)) * PtT0 + (1 - exp(-3.5)) * PtTN;
//	double d, dp ;
//	d = PtT2Y + PtT3Y + PtTN ;
//	dp = - (1-exp(- 1.5)) * PtT2Y - (1-exp(- 2.5)) * PtT3Y - (1-exp(- 3.5)) * PtTN ;
//
//	std::cout << "swapRate(0.5, 0.2) 1st deriv :  " << approx.swapRate_1stDerivative(0.5, 0.2) << std::endl ;
//	std::cout << "swapRate(0.5, 0.2) 1st deriv :  " << (np * d - n * dp) / (d * d) << std::endl ;
//
//	std::cout << " " << std::endl ;
//	std::cout << "-----------  test fonction inverse  -----------------------" << std::endl ;
//	std::cout << " " << std::endl ;
//	std::cout << "swapRate(0.5, 2) : " << approx.swapRate(0.5, 2) << std::endl ;
//	std::cout << "inverse : " << approx.inverse(0.5, approx.swapRate(0.5, 2)) << " vs 2" << std::endl ;
//	std::cout << " " << std::endl ;
//	std::cout << "swapRate(0.5, 0.2) : " << approx.swapRate(0.5, 0.2) << std::endl ;
//	std::cout << "inverse : " << approx.inverse(0.5, approx.swapRate(0.5, 0.2)) << " vs 0.2" << std::endl ;
//	std::cout << " " << std::endl ;	
//	std::cout << "swapRate(1, 0.5) : " << approx.swapRate(1, 0.5) << std::endl ;
//	std::cout << "inverse : " << approx.inverse(1, approx.swapRate(1, 0.5)) << " vs 0.5" << std::endl ;
//	std::cout << " " << std::endl ;
//	std::cout << "swapRate(1, 5) : " << approx.swapRate(1, 5) << std::endl ;
//	std::cout << "inverse : " << approx.inverse(1, approx.swapRate(1, 5)) << " vs 5" << std::endl ;
//
//	std::cout << " " << std::endl ;
//	std::cout << "integrale v2 : " << approx.v2(1) << std::endl ;

}



//void test_y_barre(double t)
//{
//	//integrale main
//	std::vector<double> x, y ;
//	x.push_back(0) ; x.push_back(0.5) ; x.push_back(1) ; 
//	y.push_back(0.25) ; y.push_back(0.5) ;
//	double k(1) ;
//
//	courbeInput_PTR courbe_PTR_test(createCourbeInput());
//
//	double res_integrale ;
//	double domaine1 = y[0] * y[0] * (exp(2*k*std::min(0.5, t)) -               1          )/(2 * k) ;
//	double domaine2 = y[1] * y[1] * (exp(2*k*std::min(1.0, t)) - exp(2*k*std::min(0.5, t)))/(2 * k) ;
//	res_integrale = exp(- 2 * k * t) *(domaine1 + domaine2) ; 
//	
//	//integrale 	CheyetteDD_VanillaSwaptionApproxPricer
//	piecewiseconst_RR_Function sigma = piecewiseconst_RR_Function(x, y) ; 
//	piecewiseconst_RR_Function m = piecewiseconst_RR_Function(x, y) ; 
//	CheyetteDD_Model::CheyetteDD_Parameter monStruct(k, sigma, m) ;
//	CheyetteDD_Model_PTR modele_test_PTR(new CheyetteDD_Model(courbe_PTR_test, monStruct)) ;
//
//	double strike          = 0.04;
//	LMM::Index  indexStart = 0; 
//	LMM::Index  indexEnd   = 8; 
//	Tenor	floatingLegTenorType = Tenor::_6M;
//	Tenor	fixedLegTenorType    = Tenor::_1YR;
//	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 15) );
//	VanillaSwap w = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
//	
//	VanillaSwaption_PTR vs(new VanillaSwaption(w, OptionType::OptionType::CALL)) ;
//
//	double s0 = 3 ;
//	CheyetteDD_VanillaSwaptionApproxPricer abc = CheyetteDD_VanillaSwaptionApproxPricer(modele_test_PTR, vs); 
//	double integrale = abc.calculate_y_bar(t) ;
//
//	std::cout << "integrale_main" << res_integrale << std::endl ;
//	std::cout << "integrale_classe" << integrale << std::endl ;
//	
//	std::cout << "annuite" << std::endl ;
//	//std::cout <<  modele_test_PTR->annuity(w) << std::endl ;
//	//std::cout <<  exp(- 0.85/100) + exp(- 2 * 0.9/100) + exp(-3 * 0.92/100) + exp(-4 * 0.95/100) << std::endl ;
//
//	std::cout << abc.swapRateDenominator_1stDerivative(0, 0) << std::endl ;
//	std::cout <<    - (1 - exp(- 1)) * exp(- 0.85/100) 
//					- (1 - exp(- 2)) * exp(- 2 * 0.9/100) 
//					- (1 - exp(- 3)) * exp(- 3 * 0.92/100) 
//					- (1 - exp(- 4)) * exp(- 4 * 0.95/100) << std::endl ;
//}


void test_time_average()
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

	std::vector<double> x, sigma_y, m_y, shift_y ;
	x.push_back(0) ; x.push_back(1) ; x.push_back(2) ; 
	m_y.push_back(0.25) ; m_y.push_back(0.5) ;
	sigma_y.push_back(0.25) ; sigma_y.push_back(0.5) ;
	shift_y.push_back(0.25) ; shift_y.push_back(0.5) ;

	piecewiseconst_RR_Function sigma = piecewiseconst_RR_Function(x, sigma_y) ; 
	piecewiseconst_RR_Function m = piecewiseconst_RR_Function(x, m_y) ; 
	piecewiseconst_RR_Function shift = piecewiseconst_RR_Function(x, shift_y) ; 
	double k = 0.05 ;

	CheyetteDD_Model::CheyetteDD_Parameter monStruct(k, sigma, m, shift) ;
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

	double S0 = approx.get_buffer_s0_() ;
	double inv = approx.inverse(0, S0) ;
	std::cout << "dS(t) = (A(t) S(t) + B(t) ) dW_t^A" << std::endl ;
	std::cout << "A(0) = " << approx.swapRateVolatility_1stDerivative(0, inv) << std::endl ;
	std::cout << "B(0) = " << approx.calculate_phi_t_s_bar(0) - S0 * approx.swapRateVolatility_1stDerivative(0, inv) << std::endl ;
	std::cout << "  " << std::endl ;

	std::cout << "dS(t) = lambda(t) (b(t) S(t) + (1-b(t)) S0 ) dW_t A" << std::endl ;
	std::cout << "lambda(0) = " << approx.lambda(0) << std::endl ;
	std::cout << "b(0) = " << approx.b(0) << std::endl ;
	std::cout << "  " << std::endl ;


	std::cout << "gridSize = 11" << std::endl ;
	std::cout << "prixSwaption" << std::endl ;
	std::cout << approx.prixSwaptionApproxPiterbarg() << std::endl ;
	std::cout << "  " << std::endl ;

}

//void test_y_bar_cas_limite()
//{
//	courbeInput_PTR courbe_PTR_test(createCourbeInput());
//
//	std::vector<double> x, y, y_m ;
//	x.push_back(0) ; ; x.push_back(1) ; 
//	y.push_back(0.25) ;	//sigma = cste
//	y_m.push_back(0) ;	//m = 0
//	piecewiseconst_RR_Function sigma = piecewiseconst_RR_Function(x, y) ; 
//	piecewiseconst_RR_Function m = piecewiseconst_RR_Function(x, y_m) ; 
//
//	double k(1) ;
//
//	CheyetteDD_Model::CheyetteDD_Parameter monStruct(k, sigma, m) ;
//	CheyetteDD_Model_PTR modele_test_PTR(new CheyetteDD_Model(courbe_PTR_test, monStruct)) ;
//
//	double strike          = 0.04;
//	LMM::Index  indexStart = 2 ; //indice 1er flux
//	LMM::Index  indexEnd   = 8 ; //indice fernier flux
//	Tenor	floatingLegTenorType = Tenor::_6M;
//	Tenor	fixedLegTenorType    = Tenor::_1YR;
//	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 50) );
//	VanillaSwap swap = VanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
//	
//	VanillaSwaption_PTR swaption_ptr(new VanillaSwaption(swap, OptionType::OptionType::CALL)) ;
//
//	double S0 = 3 ; //annuite spot
//	CheyetteDD_VanillaSwaptionApproxPricer approx = CheyetteDD_VanillaSwaptionApproxPricer(modele_test_PTR, swaption_ptr); 
//
//	std::cout << "y_bar_0   : " << approx.calculate_y_bar(0) << " vs 0"<< std::endl ;
//	std::cout << "y_bar_0.5 : " << approx.calculate_y_bar(0.5) << " vs " << 0.25*0.25*0.03*0.03*(1-exp(-2 * 0.5))/2.<< std::endl ;
//	std::cout << "y_bar_1   : " << approx.calculate_y_bar(1) << " vs " << 0.25*0.25*0.03*0.03*(1-exp(-2 * 1))/2. << std::endl ;
//
//	double inverse_s0 ; 
//
////verification de phi_t_s_bar
//	std::cout << "  " << std::endl ;
//	inverse_s0 = approx.inverse(0, S0) ;
//	std::cout << "approx.calculate_phi_t_s_bar(0)   : " << approx.calculate_phi_t_s_bar(0) 
//		<< " vs " << 0.25*0.03 * approx.swapRate_1stDerivative(0,	inverse_s0) << std::endl ;
//	
//	inverse_s0 = approx.inverse(0.5, S0) ;
//	std::cout << "approx.calculate_phi_t_s_bar(0.5) : " << approx.calculate_phi_t_s_bar(0.5) 
//				<< " vs " << 0.25*0.03 * approx.swapRate_1stDerivative(0.5,	inverse_s0) << std::endl ;
//
//	inverse_s0 = approx.inverse(1, S0) ;
//	std::cout << "approx.calculate_phi_t_s_bar(1)   : " << approx.calculate_phi_t_s_bar(1) 
//				<< " vs " << 0.25*0.03 * approx.swapRate_1stDerivative(1,	inverse_s0) << std::endl ;
//
////verification de d \phi / ds (t, s_bar)
//	std::cout << "  " << std::endl ;
//	inverse_s0 = approx.inverse(0, S0) ;
//	std::cout << "approx. d phi / ds   : " << approx.swapRateVolatility_1stDerivative(0, inverse_s0)  
//		<< " vs " << 0.25*0.03 * approx.swapRate_2ndDerivative(0, inverse_s0) 
//													/ approx.swapRate_1stDerivative(0,	inverse_s0) << std::endl ;
//	
//	inverse_s0 = approx.inverse(0.5, S0) ;
//	std::cout << "approx. d phi / ds   : " << approx.swapRateVolatility_1stDerivative(0.5, inverse_s0)
//				<< " vs " << 0.25*0.03 * approx.swapRate_2ndDerivative(0.5, inverse_s0) 
//													/ approx.swapRate_1stDerivative(0.5,	inverse_s0) << std::endl ;
//
//	inverse_s0 = approx.inverse(1, S0) ;
//	std::cout << "approx. d phi / ds   : " << approx.swapRateVolatility_1stDerivative(1, inverse_s0)
//				<< " vs " << 0.25*0.03 * approx.swapRate_2ndDerivative(1, inverse_s0) 
//													/ approx.swapRate_1stDerivative(1,	inverse_s0) << std::endl ;
//	std::cout << "  " << std::endl ;
//	std::cout << "lambda_t : "  << std::endl ;
//	
//
//	//std::cout << "prixSwaption" << std::endl ;
//	//std::cout << approx.prixSwaptionApproxPiterbarg() << std::endl ;
//	//std::cout << "OK ! " << std::endl ;
//}
//	double f(double x)
//	{
//        return x;
//	}
//
//// \int_0^1 u \int_0^u s ds du = 1/8
//void test_incremental_integrale()
//{
//	double start = 0 ;
//	double end = 1 ;
//	size_t nbPoints = 100 + 1 ; //delta t = 1/100
//	std::vector<double> f_grids ;
//	for (size_t i = 0 ; i < nbPoints ; ++i)
//	{
//		f_grids.push_back(i/100.) ;		//modifier ici aussi le nb de points
//	}
//	numeric::IncrementalIntegrator1D_Riemann incr(start, end, nbPoints, f_grids); 
//	//for (size_t i = 0 ; i < nbPoints ; ++i)			//debogage
//	//{
//	//	std::cout << incr.get_value(i) << std::endl ;
//	//}
//	boost::function<double(double)> func1 = f;
//	std::cout << incr.integrate(func1) << std::endl ;
//	std::cout << 1/8. << std::endl ;
//}
//
//void test_incremental_b_barre()
//{
//	double start = 0 ;
//	double end = 1 ;
//	size_t nbPoints = 100 + 1 ; //delta t = 1/100
//	std::vector<double> f_grids ;
//	for (size_t i = 0 ; i < nbPoints ; ++i)
//	{
//		f_grids.push_back(i/100.) ;		//modifier ici aussi le nb de points
//	}
//	numeric::IncrementalIntegrator1D_Riemann incr(start, end, nbPoints, f_grids); 
//	//for (size_t i = 0 ; i < nbPoints ; ++i)			//debogage
//	//{
//	//	std::cout << incr.get_value(i) << std::endl ;
//	//}
//	boost::function<double(double)> func1 = f;
//	std::cout << incr.integrate(func1) << std::endl ;
//	std::cout << 1/8. << std::endl ;
//}
//
//

