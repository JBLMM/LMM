#include <iostream>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/instrument/VanillaSwap.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/McTerminalLmm.h>
#include <LMM/LmmModel/McSpotLmm.h>
#include <LMM/pricer/McLmmVanillaSwapPricer.h>

//! check FRA
#include <LMM/pricer/LmmVanillaFRAPricer.h>
#include <LMM/instrument/VanillaFRA.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>

#include <LMM/Test/Test_createLmmModel.h>
//----------------------------------------------------------------------------------------------
//
//                                       Test_VanillaSwap
//
//----------------------------------------------------------------------------------------------
VanillaSwap get_VanillaSwap()
{
	double strike = 0.0;
	LMM::Index  indexStart = 12; 
	LMM::Index  indexEnd   = 28; 
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 15) );

	VanillaSwap vanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
	return vanillaSwap;
}

//----------------------------------------------------------------------------------------------
//
//                                       Test_VanillaSwapPricer
//
//----------------------------------------------------------------------------------------------

//Lmm_PTR get_lmm_for_swap(std::vector<double>& liborsInitValue_op)
//{
//	//! McTerminalLmm
//	//! LMMTenorStructure
//	Tenor tenorType = Tenor::_6M;
//	size_t horizonYear = 15;
//	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(tenorType, horizonYear));
//	//lmmTenorStructure->print("test_VanillaSwapPricer_Tenor.csv");
//
//	//! volatility function
//	//! volatility function
//	double a = -0.06;
//	double b = 0.17;
//	double c = 0.54;
//	double d = 0.17;
//	Shifted_HGVolatilityParam::ABCDParameter abcdParam (a,b,c,d);
//
//	double g_constParam = 1.0;
//	double shift_constParam = 0.0;
//	ConstShifted_HGVolatilityParam_PTR hgParam( new ConstShifted_HGVolatilityParam(lmmTenorStructure,abcdParam,g_constParam,shift_constParam));
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
//	//correlation->print("test_VanillaSwapPricer_Correlation.csv");
//
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
//	//hgVolatilityFunction->print("test_VanillaSwapPricer_Volatility.csv");
//
//	//! Dispersion
//	Dispersion dispersion(hgVolatilityFunction);
//
//
//	Lmm_PTR shiftedLmm(new Lmm(dispersion));
//	return shiftedLmm;
//}
//
//
//McLmm_PTR get_McLmm_for_swap(bool TerminalOrSpotProb, 
//							 MCSchemeType::MCSchemeType mcSchemeType, 
//							 const Lmm_PTR& lmm_ptr,
//							 const std::vector<double>& liborsInitValue)
//{
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
//	
//}

void test_McLmm( bool terminalOrSpotProb, 
	            MCSchemeType::MCSchemeType mcSchemeType,
				size_t minNbSimulation, 
				size_t maxNbSimulation,
				double bnSimualtionPowBase,
				//! output
				std::vector<size_t>& nbSimulationVector,  // output
				std::vector<double>& swapNPVResultVector, // output
				double& analyticalSwapPrice)              // output
{
	VanillaSwap vanillaSwap = get_VanillaSwap();

	Tenor  tenorType          = Tenor::_6M;
	size_t horizonYear        = 15; 
	double liborsInitValueFwd = 0.04;
	double shift              = 0.01;
	TEST::TEST_ShiftedLmmAndMcLmm testHelper =TEST::TEST_ShiftedLmmAndMcLmm(tenorType, 
		horizonYear,						   
		liborsInitValueFwd,
		terminalOrSpotProb,
		mcSchemeType,
		shift);
	Lmm_PTR   lmm = testHelper.get_lmm();
	McLmm_PTR mclmm = testHelper.get_mcLmm();
	std::vector<double> liborsInitValue = testHelper.get_liborsInitValues();

	McLmmVanillaSwapPricer mcLmmVanillaSwapPricer(mclmm);

	//std::vector<size_t> nbSimulationVector;
	//std::vector<double> swapNPVResultVector;
	nbSimulationVector.clear();
	swapNPVResultVector.clear();

	analyticalSwapPrice = mcLmmVanillaSwapPricer.swapNPV_Analytical_1(vanillaSwap,liborsInitValue);
	std::cout << "analyticalSwapPrice = "<< analyticalSwapPrice << std::endl;

	for(size_t i=0;; ++i)
	{
		size_t nbSimulation = (size_t)std::pow(bnSimualtionPowBase,(int)i)*minNbSimulation;
		if(nbSimulation>maxNbSimulation)
			break;
		nbSimulationVector.push_back(nbSimulation);
		double mcPrice = mcLmmVanillaSwapPricer.swapNPV(vanillaSwap, nbSimulation);
		swapNPVResultVector.push_back(mcPrice);
		mcLmmVanillaSwapPricer.resetGeneratorToinitSeed(); // for the next generation begin with the same seed as this one...
	}
}



void test_VanillaSwapPricer()
{
	//! FRA pricing to test 
	
	size_t minNbSimulation = 100;
	size_t maxNbSimulation = 500000;
	double bnSimualtionPowBase = 2;

	std::vector<size_t> nbSimulationVector;
	std::vector<double> swapNPVResultVector;
	double analyticalSwapPrice=0.0;

	std::string fileName = "test_VanillaSwapPricer.csv"; //+ MCSchemeType::mcSchemeType2String(mcSchemeType) +".csv";
	std::string path = LMMPATH::get_output_path() + fileName;

	//! Terminal Euler
	test_McLmm( true, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector, swapNPVResultVector, analyticalSwapPrice);
	std::vector<double> analyticalSwapPriceConstVector(nbSimulationVector.size(), analyticalSwapPrice);
	
	PrintElement_PTR nbSimulationVector_print   = PrintElement_PTR(new VectorPrintElement<std::vector<size_t> >("nbSimulationVector", nbSimulationVector));
	PrintElement_PTR analyticalSwapPrice_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("analyticalSwapPrice", analyticalSwapPriceConstVector));
	PrintElement_PTR Euler_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Euler_swapNPVResultVector", swapNPVResultVector));

	//! Terminal PC
	test_McLmm( true, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
				nbSimulationVector,swapNPVResultVector, analyticalSwapPrice);
	PrintElement_PTR PC_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("PC_swapNPVResultVector", swapNPVResultVector));


	////! Terminal IPC
	//test_McLmm( true, MCSchemeType::IPC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
	//			nbSimulationVector,swapNPVResultVector, analyticalSwapPrice);
	//PrintElement_PTR IPC_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("IPC_swapNPVResultVector", swapNPVResultVector));


	//! Spot Euler
	test_McLmm( false, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,swapNPVResultVector, analyticalSwapPrice);
	PrintElement_PTR Spot_EULER_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_EULER_swapNPVResultVector_print", swapNPVResultVector));

	//! Spot PC
	test_McLmm( false, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,swapNPVResultVector, analyticalSwapPrice);
	PrintElement_PTR Spot_PC_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_PC_swapNPVResultVector_print", swapNPVResultVector));


	std::vector<PrintElement_PTR> elements_print;
	elements_print.push_back(nbSimulationVector_print);
	elements_print.push_back(analyticalSwapPrice_print);
	elements_print.push_back(Euler_swapNPVResultVector_print);
	elements_print.push_back(PC_swapNPVResultVector_print);
	//elements_print.push_back(IPC_swapNPVResultVector_print);
	elements_print.push_back(Spot_EULER_swapNPVResultVector_print);
	elements_print.push_back(Spot_PC_swapNPVResultVector_print);


	Printer printer(path, elements_print);
	printer.print();
}
