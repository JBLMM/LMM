//#include "LmmSwaptionPricer.h"
//#include "Approximation.h"
//
//
//LmmSwaptionPricer::LmmSwaptionPricer(const VanillaSwaption& swaption, 
//	                                 McLmm& lmmModel) 
//{
//	swaption_ = swaption;
//	lmmModel_ = &lmmModel;
//}
//
//
//double LmmSwaptionPricer::annuity(SwapInstruments::VanillaSwap& swap, 
//								  const std::vector<double>& numeraires) // YY the name is not good, numeraire can be P(.,T_N) or B(.)
//{
//    double fixTenorPerYear = ((double)(swap.get_fixingTenor()))/12;
//	size_t nbFixingPayments = swap.getFixingPaymentIndices().size();
//	
//	double annuity = 0.;
//	for each (size_t indexFixing in swap.getFixingPaymentIndices())
//		annuity += fixTenorPerYear / numeraires[indexFixing]; 
//	
//	annuity *= numeraires[swap.get_indexStart()];
//
//	return annuity;
//}
//
//double LmmSwaptionPricer::swapRate(SwapInstruments::VanillaSwap& swap, const matrix_ &libors, const std::vector<double>& numeraires)
//{
//	return swapPvFloating(swap,numeraires,libors)/annuity(swap,numeraires);
//}
//
//
//double LmmSwaptionPricer::swapPvFloating(SwapInstruments::VanillaSwap& swap,
//		                                 const std::vector<double>& numeraires, 
//						                 const std::vector<std::vector<double>> &libors)
//{
//	if (numeraires.size() != lmmModel_->getHorizon()+2)
//		throw("Error: numeraire's size should be equal to model's number of dates");
//
//
//	double floatTenorPerYear = ((double)swap.get_floatingTenor())/12;
//	size_t nbFloatingPayments = swap.getFloatingPaymentIndices().size();
//
//	double pvFloating = 0.; 
//	size_t indexFloatingInit = swap.getFloatingPaymentIndices()[0];
//
//
//	for each (size_t indexFloating in swap.getFloatingPaymentIndices())
//		pvFloating += floatTenorPerYear * libors[indexFloating-1][indexFloatingInit-1] / numeraires[indexFloating];
//	
//	pvFloating *= numeraires[swap.get_indexStart()];
//
//	return pvFloating;
//}
//
//
//double LmmSwaptionPricer::swapPvFixing(SwapInstruments::VanillaSwap& swap,
//		                               const std::vector<double>& numeraires)
//{
//	return (swap.get_strike())*annuity(swap,numeraires);
//}
//
//
//double LmmSwaptionPricer::npvSwaption(SwapInstruments::VanillaSwap& swap,
//		                              const std::vector<double>& numeraires,
//					                  const matrix_ &libors)
//{
//	double swapNpv = swapPvFloating(swap,numeraires,libors) - swapPvFixing(swap,numeraires);
//	return std::max(swapNpv,0.);
//}
//
//void LmmSwaptionPricer::McSwaptionPrice(size_t nbSimulations,
//		                                Real &mcPrice, 
//										Real &mcStdErr, 
//										SwapInstruments::VanillaSwap& swap,						  			
//										const std::vector<double>& libors_T0,
//										ApproxType approx)
//{
//	size_t N = lmmModel_->getHorizon();
//
//	//-- MC loop
//	mcPrice = 0.; 
//	mcStdErr = 0.;
//	for (size_t simulIndex = 0; simulIndex < nbSimulations; ++simulIndex) 
//	{
//		//-- Libor simulation 
//		lmmModel_->LmmSimulation(approx);
//
//		//-- Numeraire computations
//		lmmModel_->computeNumeraires();
//
//		matrix_ liborsMatrix = lmmModel_->getLiborMatrix();
//
//		//-- Compute ZC bonds with simulated libors
//		/*std::vector<double> bonds;
//		swap.computeBonds_T0(N,bonds,liborsMatrix);*/
//
//		//double swaptionPayoff = npvSwaption(swap,bonds,liborsMatrix);
//		double swaptionPayoff = npvSwaption(swap,lmmModel_->getNumeraire(),liborsMatrix);
//
//		
//		/*Real B_i = lmmModel_->Spot_numeraire(swap.get_indexStart(),liborsMatrix);*/
//
//		Real numeraire_pricing_date = lmmModel_->getNumeraire(0);
//		Real numeraire_maturity_date = lmmModel_->getNumeraire(swap.get_indexStart());
//	
//		//double x = swaptionPayoff*numeraire; 
//		double x = (numeraire_pricing_date/numeraire_maturity_date)*swaptionPayoff;
//		
//		mcPrice += x;
//		mcStdErr += x*x;
//	}
//
//	mcPrice /= nbSimulations;
//
//	mcStdErr /= (nbSimulations-1);
//	mcStdErr -= (((Real)nbSimulations)/(nbSimulations-1))*mcPrice*mcPrice;
//	mcStdErr = sqrt(mcStdErr);
//}
//
//
////--------------------------------------------------------------------//
////                               TESTS                                //
////--------------------------------------------------------------------//
//
////-- Tests with ABCD Vol
////-- TODO: Create another tests method with piecewise cst vol
//void LmmSwaptionPricer::test_computeSwaptionPrices()
//{
//	//-- TODO : Replace with swaption pricing
//
//	size_t M = 50000;
//
//	size_t N_libor = lmmModel_->getHorizon();
//	size_t nbFactors = lmmModel_->getVolMod()->getNbFactors();
//	std::vector<double> tenorDates = lmmModel_->getTenorDates();
//	std::vector<double> tenorPeriods = lmmModel_->getDeltaT();
//	std::vector<double> liborShifts = lmmModel_->getShifts();
//
//	size_t liborTenor = 6;
//	size_t fixingTenor = 12; // In months
//	size_t floatingtenor = 6; // Idem
//
//	//-- Get libors a time 0
//	std::vector<double> libors_0(N_libor+1);
//	for (size_t i = 0; i < N_libor+1; ++i)
//		libors_0[i] = lmmModel_->getLiborMatrix()[i][0];
//
//	//-- Set volatility model
//	Real a = -0.06; 
//	Real b = 0.40; 
//	Real c = 0.54; 
//	Real d = 0.10;
//
//	/*Real a = 0.;
//	Real b = 0.; 
//	Real c = 0.54; 
//	Real d = 0.1;*/
//
//	std::vector<Real> abcdParams(4);
//	abcdParams[0] = a;
//	abcdParams[1] = b;
//	abcdParams[2] = c;
//	abcdParams[3] = d;
//
//	std::vector<double> g;
//	for (size_t i = 0; i < N_libor+1; ++i)
//		g.push_back(1. + 0.0001*i *pow(-1.,i));
//
//	DeterministicVol vol(nbFactors,N_libor+1,abcdParams,g);
//
//	ApproxType approx = euler;
//
//	double mcPrice = 9999999.99999;
//	double mcErr = 9999999.99999;
//
//	ofstream outputFile;
//	outputFile.open("swaption_spot_euler.xls");
//
//	outputFile << "Swaption prices under terminal measure" << endl << endl;
//	
//	outputFile << ";";
//	for (size_t indexEnd = 4; indexEnd <= N_libor+1; ++indexEnd)
//		outputFile << indexEnd << ";" << ";"<<";";
//
//	outputFile << endl;
//	
//
//	//***********************************************************//
//	//                        MC PRICES                          //
//	//***********************************************************//
//
//	//-- Loop on index start
//	for (size_t indexStart = 2; indexStart < N_libor+1; indexStart=indexStart+2)
//	{
//		outputFile << indexStart << ";";
//		
//		for (size_t cpt=0; cpt < indexStart-2; ++cpt) // Adjust to the right column
//			outputFile << ";";
//
//		//-- Loop on index end
//		for (size_t indexEnd = indexStart+2; indexEnd <= N_libor+1; ++indexEnd)
//		{
//			//-- vanilla swap
//			SwapInstruments::VanillaSwap vanillaSwap(indexStart,indexEnd,fixingTenor,floatingtenor,liborTenor);
//
//			std::vector<size_t> floatingIndices = vanillaSwap.getFloatingPaymentIndices();
//			std::vector<size_t> fixingIndices = vanillaSwap.getFixingPaymentIndices();
//
//			//-- Compute ZC bonds at time 0
//			std::vector<double> bonds_0(N_libor+2);
//			bonds_0[0] = 1.;
//			for (size_t i = 1; i <= N_libor+1; ++i)
//				bonds_0[i] = bonds_0[i-1]/(1+tenorPeriods[i-1]*libors_0[i-1]);
//
//			//-- Compute Annuity at time 0
//			double annuity = 0.;
//			for each (size_t i in fixingIndices) 
//				annuity += (fixingTenor/12.)*bonds_0[i];
//			
//
//			//-- Compute swapRate at time 0
//			double swapRate = (bonds_0[indexStart]-bonds_0[indexEnd])/annuity;
//	
//
//			//-- Set ATM Strike
//			vanillaSwap.strike() = swapRate;
//
//			//-- Vanilla Swaption
//			SwapInstruments::VanillaSwaption vanillaSwaption(vanillaSwap,tenorDates);
//
//			//-- Create the swaption pricer
//			LmmSwaptionPricer swaptionPricer(vanillaSwaption,*lmmModel_);
//
//			//-- Compute swaption's MC price
//			swaptionPricer.McSwaptionPrice(M,mcPrice,mcErr,vanillaSwap,libors_0,approx);
//
//			Real estimatedStdErr = 3*mcErr/sqrt((Real)M);
//
//
//			//Approximation approx(indexStart,indexEnd,vol,tenorDates);
//			Approximation approx(vol,tenorDates);
//			double squaredRebonatoVol = 99999.99999;
//			double strike = vanillaSwaption.getUnderlyingSwap().get_strike();
//			double rebonatoPrice = approx.RebonatoApprox(indexStart,strike,floatingIndices,squaredRebonatoVol,swapRate,annuity,bonds_0,libors_0,liborShifts);
//
//			double relativeError = (mcPrice - rebonatoPrice)/mcPrice;
//
//			outputFile << mcPrice << ";" << rebonatoPrice << ";" << relativeError <<";";
//		}
//
//		outputFile << endl;
//	}
//
//	outputFile.close();
//	cout << "END OF WRITING" << endl;
//}