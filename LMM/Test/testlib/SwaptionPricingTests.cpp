//#include "SwaptionPricingTests.h"
//#include "Approximation.h"
//#include "LmmCapletPricer.h"
//
//using namespace std;
//
//
//void setVolatility(size_t fullRank,
//	               size_t reducedRank,
//	               DeterministicVol& vol,
//	               std::vector<Real>& abcdParams,
//				   std::vector<Real>& g)
//{
//		//-- Vol Parameters 
//	Real a = -0.06; 
//	Real b = 0.17; 
//	Real c = 0.54; 
//	Real d = 0.17;
//
//	abcdParams[0] = a;
//	abcdParams[1] = b;
//	abcdParams[2] = c;
//	abcdParams[3] = d;
//
//	for (size_t i = 0; i < g.size(); ++i)
//		g[i] = 1.;
//
//	vol = DeterministicVol(reducedRank,fullRank,abcdParams,g);
//}
//
//void setModel(McLmm& model,
//	          size_t horizon,
//			  double& tenorPeriod,
//		      size_t& floatingTenor,
//			  size_t& fixingTenor,
//		      std::vector<double>& tenorDates,
//			  std::vector<double>& deltaT,
//		      std::vector<double>& liborInit,
//			  std::vector<double>& liborShifts)
//{
//	//-- Model parameters
//
//	tenorPeriod = 0.5;
//	floatingTenor = 6; // In months
//	fixingTenor = 12; // Idem
//
//	for (size_t i = 0; i < deltaT.size(); ++i)
//		deltaT[i+1] = tenorPeriod;
//
//	for (size_t i = 1; i < tenorDates.size(); ++i)
//		tenorDates[i] = tenorDates[i-1] + deltaT[i-1];
//
//	for (size_t i = 0; i < liborInit.size(); ++i)
//		liborInit[i] = 0.02013 + i*0.001;
//
//	for (size_t i = 0; i < liborShifts.size(); ++i)
//		liborShifts[i] = 0.;
//
//}
//
//void setSwapRate(VanillaSwap& swap, 
//	             McLmm& model, 
//				 double& swapRate,
//				 double& annuity,
//				 std::vector<double>& liborInit,
//			     std::vector<double>& bonds_0,
//				 size_t fixingTenor,
//				 size_t indexStart,
//			     size_t indexEnd)
//{
//		//-- Compute ZC bonds at time 0
//		bonds_0[0] = 1.;
//		for (size_t i = 1; i <= model.getHorizon()+1; ++i)
//			bonds_0[i] = bonds_0[i-1]/(1+model.getDeltaT()[i-1]*liborInit[i-1]);
//
//
//		//-- Compute Annuity at time 0
//		std::vector<size_t> fixingIndices = swap.getFixingPaymentIndices();
//		annuity = 0.;
//		for each (size_t i in fixingIndices) 
//		{
//			annuity += (fixingTenor/12.)*bonds_0[i];
//		}
//
//		//-- Compute swapRate at time 0
//		swapRate = (bonds_0[indexStart]-bonds_0[indexEnd])/annuity;
//		cout << "swapRate : " << swapRate << endl;
//
//		//-- Set ATM Strike
//		swap.strike() = swapRate;
//}
//
//
////-- Bug 
//void pricingSwaptionTest(size_t& nbSimulations, 
//	                     size_t horizon, 
//	                     size_t BM, 
//						 std::vector<double>& tenorDates,
//						 std::vector<double>& liborInit,
//						 McLmm& model,
//						 size_t indexStart, 
//						 size_t indexEnd)
//{
//	//size_t floatingTenor,fixingTenor;
//	//double tenorPeriod,swapRate,annuity;
//	//std::vector<double> abcdParams(4),g(horizon+1);
//	//std::vector<double> deltaT(horizon+1),liborShifts(horizon+1);
//	//std::vector<double> bonds_0(horizon+2);
//
//	//setParameters(nbSimulations,abcdParams,g,tenorPeriod,floatingTenor,fixingTenor,tenorDates,deltaT,liborInit,liborShifts);
//	//
//	////-- Set volatility
//	//DeterministicVol vol(BM,horizon+1,abcdParams,g);
//
//	////-- Set the model
//	//model = McTerminalLmm(horizon,deltaT,vol,liborShifts,liborInit);
//
//	////-- Vanilla Swap
//	//SwapInstruments::VanillaSwap vanillaSwap(indexStart,indexEnd,fixingTenor,floatingTenor);
//
//	//setSwapRate(vanillaSwap,model,swapRate,annuity,liborInit,bonds_0,fixingTenor,indexStart,indexEnd);
//
//	////-- Vanilla Swaption
//	//SwapInstruments::VanillaSwaption vanillaSwaption(vanillaSwap,tenorDates);
//
//	////-- Create the swaption pricer
//	//LmmSwaptionPricer swaptionPricer(vanillaSwaption,model);
//
//	//double mcPrice = 99999999999.999999, mcErr = 99999999999.999999;
//
//	////-- Compute swaption's MC price
//	//swaptionPricer.McSwaptionPrice(nbSimulations,mcPrice,mcErr,vanillaSwap,liborInit,ipc);
//
//	////-- Show results
//	//printMcResults(mcPrice,mcErr,nbSimulations);
//
//	////-- Compute Rebonato price
//	//Approximation approx(indexStart,indexEnd,vol,tenorDates);
//	//double rebonatoPrice = approx.RebonatoApprox(vanillaSwaption,swapRate,annuity,bonds_0,liborInit,liborShifts);
//
//	////-- Show results
//	//printRebonatoResult(rebonatoPrice,mcPrice);
//}
//
//// Bug 
//// Pass model as parameter
//void compareSwaptionWithCaplet()
//{
//	//size_t horizon = 4;
//	//size_t BM = 3;
//	//size_t indexStart = 2;
//	//size_t indexEnd = 3;
//	//double tenorLibor = 0.5;
//	//size_t nbSimulations;
//
//	//std::vector<double> liborInit(horizon+1),tenorDates(horizon+2);
//	//
//
//	////-- Price a swaption with only one fixing and floating leg => Caplet
//	//pricingSwaptionTest(nbSimulations,horizon,BM,tenorDates,liborInit,*model,indexStart,indexEnd);
//
//	////-- Set caplet' ATM strike
//	//double strike = liborInit[indexStart]; // ATM strike
//
//	////-- Create a Caplet equivalent to the previously created swaption
//	//Caplet caplet(tenorDates[indexStart+1],tenorLibor,strike,indexStart);
//
//	////-- Price the caplet
//	//double mcPrice = 99999999999999.99, mcErr = 99999999999999.99;
//	//LmmCapletPricer capletPricer(caplet,*model);
//	//capletPricer.McCapletPrice(nbSimulations,mcPrice,mcErr,liborInit,euler);
//
//	//printMcResults(mcPrice,mcErr,nbSimulations);
//
//	//delete model;
//}
//
//void printMcResults(double& mcPrice, double& mcErr, size_t nbSimulations)
//{
//	
//	Real estimatedStdErr = 3*mcErr/sqrt((Real)nbSimulations);
//	Real lowerIc = mcPrice - estimatedStdErr;
//	Real upperIc = mcPrice + estimatedStdErr;
//
//	cout << "Number of MC simulations : " << nbSimulations << endl << endl;
//	cout << "----- Monte-Carlo price : " << mcPrice << endl << endl;
//	cout << "Estimated standard error : " << estimatedStdErr << endl;
//	cout << "99% confidence interval : " << "[" << lowerIc << " ; " << upperIc << "]" << endl;
//	cout << endl << endl;
//}
//
//void printRebonatoResult(double& price, double& mcPrice)
//{
//	cout << "----- Rebonato price : " << price << endl << endl;
//	cout << "ERREUR ABSOLUE : " << mcPrice - price << endl;
//	cout << "Erreur relative : " << (mcPrice-price)/mcPrice << endl;
//}