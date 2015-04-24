#include <iostream>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/McTerminalLmm.h>
#include <LMM/LmmModel/McSpotLmm.h>
#include <LMM/pricer/McLmmVanillaSwaptionPricer.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Piterbarg.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>

#include <LMM/Test/Test_createLmmModel.h>

//----------------------------------------------------------------------------------------------
//
//                                       Test_VanillaSwaption
//
//----------------------------------------------------------------------------------------------
VanillaSwaption get_VanillaSwaption()
{
	double strike          = 0.04;
	LMM::Index  indexStart = 16; 
	LMM::Index  indexEnd   = 28; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 15) );

	VanillaSwap  vanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);

	LMM::Index  indexMaturity = indexStart;
	VanillaSwaption vanillaSwaption(vanillaSwap,OptionType::CALL);
	return vanillaSwaption;
}

//Lmm_PTR get_lmm(std::vector<double>& liborsInitValue_op)
//{
//	//! McTerminalLmm
//	//! LMMTenorStructure
//	Tenor tenorType = Tenor::_6M;
//	size_t horizonYear = 15;
//	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(tenorType, horizonYear));
//	//lmmTenorStructure->print("test_VanillaSwaptionPricer_Tenor.csv");
//
//	//! volatility function
//	double a = -0.06;
//	double b = 0.17;
//	double c = 0.54;
//	double d = 0.17;
//	Shifted_HGVolatilityParam::ABCDParameter abcdParam (a,b,c,d);
//
//	double g_constParam = 1.0;
//	double shift_constParam = 0.0;
//
//	ConstShifted_HGVolatilityParam_PTR hgParam( new ConstShifted_HGVolatilityParam(lmmTenorStructure,abcdParam,g_constParam,shift_constParam));
//
//
//	//! Correlation 1
//	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
//	size_t correlFullRank = lmmTenorStructure->get_horizon()+1;
//	size_t correlReducedRank = nbFactor;
//	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
//	double correlAlpha = 0.0;
//	double correlBeta  = 0.1;
//	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
//	correlation->calculate(); // for print.
//	//correlation->print("test_VanillaSwaptionPricer_Correlation.csv");
//
//	//McLmm
//	double shiftValue = 0.0; // -0.01;
//	std::vector<double> shifts(lmmTenorStructure->get_horizon()+1, shiftValue);
//
//	double fwdRate = 0.04;
//	std::vector<double> liborsInitValue(lmmTenorStructure->get_horizon()+1, fwdRate);
//	liborsInitValue_op = liborsInitValue;
//
//	ConstShifted_HGVolatilityFunction_PTR hgVolatilityFunction (new ConstShifted_HGVolatilityFunction(lmmTenorStructure, correlation, hgParam)); 
//	//hgVolatilityFunction->print("test_VanillaSwaptionPricer_Volatility.csv");
//
//	//! Dispersion
//	Dispersion dispersion( hgVolatilityFunction);
//
//	Lmm_PTR lmm (new Lmm(dispersion));
//	return lmm;
//}
//
////----------------------------------------------------------------------------------------------
////
////                                       Test_VanillaSwapPricer
////
////----------------------------------------------------------------------------------------------
//McLmm_PTR get_McLmm_for_swaption(bool TerminalOrSpotProb,
//								 MCSchemeType::MCSchemeType mcSchemeType, 
//								 const Lmm_PTR& lmm_ptr,
//								 const std::vector<double>& liborsInitValue)
//{
//	
//	unsigned long seed = 47;
//	RNGenerator_PTR  rnGenerator(new McGenerator(seed));
//
//	if(TerminalOrSpotProb)
//	{
//		McLmm_PTR mclmm(new McTerminalLmm (lmm_ptr, liborsInitValue, rnGenerator, mcSchemeType));
//		return mclmm;
//	}
//	else
//	{
//		McLmm_PTR mclmm(new McSpotLmm (lmm_ptr, liborsInitValue, rnGenerator, mcSchemeType));
//		return mclmm;
//	}
//}

void test_McLmm2( bool terminalOrSpotProb, 
				MCSchemeType::MCSchemeType mcSchemeType,
				size_t minNbSimulation, 
				size_t maxNbSimulation,
				double bnSimualtionPowBase,
				//! output
				std::vector<size_t>& nbSimulationVector,  // output
				std::vector<double>& swapNPVResultVector, // output
				double& analyticalSwaptionPrice_Robonato,
				double& analyticalSwaptionPrice_Piterbarg)              // output
{
	VanillaSwaption vanillaSwaption = get_VanillaSwaption();

	//Lmm_PTR lmm_ptr = get_lmm(liborsInitValue);
	//McLmm_PTR mclmm_ptr = get_McLmm_for_swaption(TerminalOrSpotProb, mcSchemeType, lmm_ptr, liborsInitValue);

	Tenor  tenorType          = Tenor::_6M;
	size_t horizonYear        = 15; 
	double liborsInitValueFwd = 0.04;
	double shift              = 0.02;
	TEST::TEST_ShiftedLmmAndMcLmm testHelper =TEST::TEST_ShiftedLmmAndMcLmm(tenorType, 
																			horizonYear,						   
																			liborsInitValueFwd,
																			terminalOrSpotProb,
																			mcSchemeType,
																			shift);
	Lmm_PTR   lmm = testHelper.get_lmm();
	McLmm_PTR mclmm = testHelper.get_mcLmm();
	std::vector<double> liborsInitValue = testHelper.get_liborsInitValues();


	McLmmVanillaSwaptionPricer mcLmmVanillaSwaptionPricer(mclmm);

	//! analytical swaptionApprox price
	LmmVanillaSwaptionApproxPricer_Rebonato lmmVanillaSwaptionApproxPricer_Rebonato(lmm); // YY: maybe a problem of polymorphism for smart_pointer ??? 
	analyticalSwaptionPrice_Robonato = lmmVanillaSwaptionApproxPricer_Rebonato.price(vanillaSwaption, liborsInitValue);
	std::cout << "analyticalSwaptionPrice_Robonato = "<< analyticalSwaptionPrice_Robonato <<  std::endl;

	LmmVanillaSwaptionApproxPricer_Piterbarg lmmVanillaSwaptionApproxPricer_Piterbarg(lmm); 
	analyticalSwaptionPrice_Piterbarg  = lmmVanillaSwaptionApproxPricer_Piterbarg.price(vanillaSwaption, liborsInitValue);
	std::cout << "analyticalSwaptionPrice_Piterbarg = "<< analyticalSwaptionPrice_Piterbarg <<  std::endl;

	double strikeBump = 0.001;
	double skew = lmmVanillaSwaptionApproxPricer_Rebonato.skew(vanillaSwaption, liborsInitValue, strikeBump);
	std::cout << "--------- shift = " << shift << "  ---- skew = " << skew << std::endl;


	// ---------------------------------- delete latter --------------------------------
	VanillaSwap vanillaSwap  = vanillaSwaption.getUnderlyingSwap(); 
	double fwd               = lmmVanillaSwaptionApproxPricer_Rebonato.swapRate_Analytical(vanillaSwap, liborsInitValue); 
	double K                 = vanillaSwap.get_strike();
	double annuity0          = lmmVanillaSwaptionApproxPricer_Rebonato.annuity0(vanillaSwap, liborsInitValue);
	LMM::Index swaptionIndexMaturity = vanillaSwaption.get_indexMaturity();
	double T                 = 4;

	double impliedVol_Robonato = NumericalMethods::Black_impliedVolatility(analyticalSwaptionPrice_Robonato/annuity0,  fwd, K, T);
	std::cout << " ---- analytical Robonato impliedVol = " << impliedVol_Robonato << std::endl;
	//double reprice_Robnato = annuity0*NumericalMethods::Black_Price(fwd, K , impliedVol_Robonato, T); 
	//std::cout << " ---- analytical Robonato- reprice  = " << reprice_Robnato << std::endl;

	double impliedVol_Piterbarg = NumericalMethods::Black_impliedVolatility(analyticalSwaptionPrice_Piterbarg/annuity0, fwd, K, T);
	std::cout << " ---- analytical Piterbarg impliedVol = " << impliedVol_Piterbarg << std::endl;
	//double reprice_Piterbarg = annuity0*NumericalMethods::Black_Price(fwd, K , impliedVol_Piterbarg, T); 
	//std::cout << " ---- analytical Piterbarg- reprice  = " << reprice_Piterbarg << std::endl;
	// ---------------------------------- delete latter --------------------------------


	//! MC swaption price, TODO: seems a bad thing, these vectors.
	nbSimulationVector.clear();
	swapNPVResultVector.clear();

	for(size_t i=0;; ++i)
	{
		size_t nbSimulation = (size_t)std::pow(bnSimualtionPowBase,(int)i)*minNbSimulation;
		if(nbSimulation>maxNbSimulation)
			break;
		double price = mcLmmVanillaSwaptionPricer.price(vanillaSwaption, nbSimulation);


		double impliedVol = NumericalMethods::Black_impliedVolatility(price/annuity0, fwd, K, T);
		std::cout << " ---- MC impliedVol = " << impliedVol << std::endl;
		double reprice = annuity0*NumericalMethods::Black_Price(fwd, K , impliedVol, T); 
		std::cout << " ---- MC reprice  = " << reprice << std::endl;

		nbSimulationVector.push_back(nbSimulation);
		swapNPVResultVector.push_back(price);
		mcLmmVanillaSwaptionPricer.resetGeneratorToinitSeed(); // for the next generation begin with the same seed as this one...
	}
}



void test_VanillaSwaptionPricer()
{
	size_t minNbSimulation = 1000;
	size_t maxNbSimulation = 600000;
	double bnSimualtionPowBase = 2;

	std::vector<size_t> nbSimulationVector;
	std::vector<double> swaptionPriceVector;
	double analytical_swaptionPrice_Robonato=0.0;
	double analytical_swaptionPrice_Piterbarg=0.0;

	std::string fileName = "test_VanillaSwaptionPricer.csv"; //+ MCSchemeType::mcSchemeType2String(mcSchemeType) +".csv";
	std::string path = LMMPATH::get_output_path() + fileName;

	//! Terminal Euler
	test_McLmm2( true, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector, swaptionPriceVector, analytical_swaptionPrice_Robonato, analytical_swaptionPrice_Piterbarg);
	std::vector<double> analyticalRobonato_swaptionPriceConstVector(nbSimulationVector.size(), analytical_swaptionPrice_Robonato);
	std::vector<double> analyticalPiterbarg_swaptionPriceConstVector(nbSimulationVector.size(), analytical_swaptionPrice_Piterbarg);

	PrintElement_PTR nbSimulationVector_print        = PrintElement_PTR(new VectorPrintElement<std::vector<size_t> >("nbSimulationVector", nbSimulationVector));
	PrintElement_PTR analyticalRobonato_swaptionPrice_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("analyticalRobonato_swaptionPrice", analyticalRobonato_swaptionPriceConstVector));
	PrintElement_PTR analyticalPiterbarg_swaptionPrice_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("analyticalPiterbarg_swaptionPrice", analyticalPiterbarg_swaptionPriceConstVector));
	PrintElement_PTR Euler_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Euler_swapNPVResultVector", swaptionPriceVector));

	////! Terminal PC
	//test_McLmm2( true, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
	//	nbSimulationVector,swaptionPriceVector, analytical_swaptionPrice_Robonato, analytical_swaptionPrice_Piterbarg);
	//PrintElement_PTR PC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("PC_swapNPVResultVector", swaptionPriceVector));


	////! Terminal IPC
	//test_McLmm2( true, MCSchemeType::IPC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
	//	nbSimulationVector,swaptionPriceVector, analytical_swaptionPrice_Robonato, analytical_swaptionPrice_Piterbarg);
	//PrintElement_PTR IPC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("IPC_swapNPVResultVector", swaptionPriceVector));


	////! Spot Euler
	//test_McLmm2( false, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
	//	nbSimulationVector,swaptionPriceVector, analytical_swaptionPrice_Robonato, analytical_swaptionPrice_Piterbarg);
	//PrintElement_PTR Spot_EULER_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_EULER_swapNPVResultVector_print", swaptionPriceVector));

	//! Spot PC
	test_McLmm2( false, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,swaptionPriceVector, analytical_swaptionPrice_Robonato, analytical_swaptionPrice_Piterbarg);
	PrintElement_PTR Spot_PC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_PC_swapNPVResultVector_print", swaptionPriceVector));


	std::vector<PrintElement_PTR> elements_print;
	elements_print.push_back(nbSimulationVector_print);
	elements_print.push_back(analyticalRobonato_swaptionPrice_print);
	elements_print.push_back(analyticalPiterbarg_swaptionPrice_print);
	elements_print.push_back(Euler_swaptionPriceVector_print);
	//elements_print.push_back(PC_swaptionPriceVector_print);
	//elements_print.push_back(IPC_swaptionPriceVector_print);
	//elements_print.push_back(Spot_EULER_swaptionPriceVector_print);
	elements_print.push_back(Spot_PC_swaptionPriceVector_print);


	Printer printer(path, elements_print);
	printer.print();


}


