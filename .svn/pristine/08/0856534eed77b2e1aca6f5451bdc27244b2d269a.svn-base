#define BOOST_TEST_MODULE LMM_unit_test_swap
#include <boost/test/included/unit_test.hpp>


#include <iostream>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/instrument/VanillaSwap.h>
#include <LMM/ModelMCLMM/Correlation.h>
#include <LMM/ModelMCLMM/HGVolatilityFunction.h>
#include <LMM/ModelMCLMM/McTerminalLmm.h>
#include <LMM/ModelMCLMM/McSpotLmm.h>
#include <LMM/pricer/McLmmVanillaSwapPricer.h>


BOOST_AUTO_TEST_SUITE(lmm_test_swap)
//----------------------------------------------------------------------------------------------
//
//                                       Test_VanillaSwap
//
//----------------------------------------------------------------------------------------------
LMM::VanillaSwap get_VanillaSwap()
{
	double strike = 0.04;
	LMM::Index  indexStart = 12; // 12
	LMM::Index  indexEnd   = 18; // 28;
	Tenor	floatingLegTenorType = Tenor::_6M;
	Tenor	fixedLegTenorType    = Tenor::_1YR;
	LMMTenorStructure_PTR simulationStructure(new LMMTenorStructure(Tenor::_6M , 15) );

	LMM::VanillaSwap vanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, simulationStructure);
	return vanillaSwap;
}



//----------------------------------------------------------------------------------------------
//
//                                       Test_VanillaSwapPricer
//
//----------------------------------------------------------------------------------------------
MCLmm_PTR get_McLmm(bool TerminalOrSpotProb, MCSchemeType::MCSchemeType mcSchemeType, std::vector<double>& liborsInitValue_op)
{
//! McTerminalLmm
	//! LMMTenorStructure
	Tenor tenorType = Tenor::_6M;
	size_t horizonYear = 15;
	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(tenorType, horizonYear));
	lmmTenorStructure->print("test_VanillaSwapPricer_Tenor.csv");

	//! volatility function
	double a = -0.06;
	double b = 0.17;
	double c = 0.54;
	double d = 0.17;
	DefaultHGVolatilityParam abcdParams(a,b,c,d, lmmTenorStructure);
	
	VolatilityFunction_PTR hgVolatilityFunction (new HGVolatilityFunction(abcdParams,  lmmTenorStructure)); 
	hgVolatilityFunction->print("test_VanillaSwapPricer_Volatility.csv");

	//! Correlation 1
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = lmmTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.04;
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.
	correlation->print("test_VanillaSwapPricer_Correlation.csv");

	//! Dispersion
	Dispersion dispersion(correlation, hgVolatilityFunction);

	//McLmm
	double shiftValue = 0.0; // -0.01;
	std::vector<double> shifts(lmmTenorStructure->get_horizon()+1, shiftValue);

	double fwdRate = 0.1;
	std::vector<double> liborsInitValue(lmmTenorStructure->get_horizon()+1, fwdRate);
	liborsInitValue_op = liborsInitValue;

	unsigned long seed = 47;
	RNGenerator_PTR  rnGenerator(new McGenerator(seed));

	if(TerminalOrSpotProb)
	{
		MCLmm_PTR mclmm(new McTerminalLmm (dispersion, shifts, liborsInitValue, rnGenerator, mcSchemeType));
		return mclmm;
	}
	else
	{
		MCLmm_PTR mclmm(new McSpotLmm (dispersion, shifts, liborsInitValue, rnGenerator, mcSchemeType));
		return mclmm;
	}
	
}

void test_McLmm( bool TerminalOrSpotProb, 
	            MCSchemeType::MCSchemeType mcSchemeType,
				size_t minNbSimulation, 
				size_t maxNbSimulation,
				double bnSimualtionPowStep,
				//! output
				std::vector<size_t>& nbSimulationVector,  // output
				std::vector<double>& swapNPVResultVector, // output
				double& analyticalSwapPrice)              // output
{
	LMM::VanillaSwap vanillaSwap = get_VanillaSwap();
	std::vector<double> liborsInitValue;
	MCLmm_PTR mclmm = get_McLmm(TerminalOrSpotProb, mcSchemeType,liborsInitValue);

	MCLmmVanillaSwapPricer mcLmmVanillaSwapPricer(mclmm);

	//std::vector<size_t> nbSimulationVector;
	//std::vector<double> swapNPVResultVector;
	nbSimulationVector.clear();
	swapNPVResultVector.clear();

	for(size_t i=0;; ++i)
	{
		size_t nbSimulation = (size_t)std::pow(bnSimualtionPowStep,(int)i)*minNbSimulation;
		if(nbSimulation>maxNbSimulation)
			break;
		nbSimulationVector.push_back(nbSimulation);
		swapNPVResultVector.push_back(mcLmmVanillaSwapPricer.swapNPV(vanillaSwap, nbSimulation));
		mcLmmVanillaSwapPricer.resetGeneratorToinitSeed(); // for the next generation begin with the same seed as this one...
	}

	analyticalSwapPrice = mcLmmVanillaSwapPricer.swapNPV_Analytical_1(vanillaSwap,liborsInitValue);
}



BOOST_AUTO_TEST_CASE(test_VanillaSwap)
{
	LMM::VanillaSwap vanillaSwap = get_VanillaSwap();
	
	std::cout<<vanillaSwap<<std::endl;

	BOOST_CHECK(true);
}


BOOST_AUTO_TEST_CASE(test_VanillaSwapPricer)
{
	size_t minNbSimulation = 100;
	size_t maxNbSimulation = 1000; //300000;
	double bnSimualtionPowStep = 2;

	std::vector<size_t> nbSimulationVector;
	std::vector<double> swapNPVResultVector;
	double analyticalSwapPrice=0.0;

	std::string fileName = "test_VanillaSwapPricer.csv"; //+ MCSchemeType::mcSchemeType2String(mcSchemeType) +".csv";
	std::string path = LMM::get_output_path() + fileName;

	//! Terminal Euler
	test_McLmm( true, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowStep,
		nbSimulationVector, swapNPVResultVector, analyticalSwapPrice);
	std::vector<double> analyticalSwapPriceConstVector(nbSimulationVector.size(), analyticalSwapPrice);

	std::cout << analyticalSwapPrice << std::endl;
	
	PrintElement_PTR nbSimulationVector_print   = PrintElement_PTR(new VectorPrintElement<std::vector<size_t> >("nbSimulationVector", nbSimulationVector));
	PrintElement_PTR analyticalSwapPrice_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("analyticalSwapPrice", analyticalSwapPriceConstVector));
	PrintElement_PTR Euler_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Euler_swapNPVResultVector", swapNPVResultVector));

	//! Terminal PC
	test_McLmm( true, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowStep,
				nbSimulationVector,swapNPVResultVector, analyticalSwapPrice);
	PrintElement_PTR PC_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("PC_swapNPVResultVector", swapNPVResultVector));


	//! Terminal IPC
	test_McLmm( true, MCSchemeType::IPC,minNbSimulation,maxNbSimulation,bnSimualtionPowStep,
				nbSimulationVector,swapNPVResultVector, analyticalSwapPrice);
	PrintElement_PTR IPC_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("IPC_swapNPVResultVector", swapNPVResultVector));


	//! Spot Euler
	test_McLmm( false, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowStep,
		nbSimulationVector,swapNPVResultVector, analyticalSwapPrice);
	PrintElement_PTR Spot_EULER_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_EULER_swapNPVResultVector_print", swapNPVResultVector));

	//! Spot PC
	test_McLmm( false, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowStep,
		nbSimulationVector,swapNPVResultVector, analyticalSwapPrice);
	PrintElement_PTR Spot_PC_swapNPVResultVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_PC_swapNPVResultVector_print", swapNPVResultVector));


	std::vector<PrintElement_PTR> elements_print;
	elements_print.push_back(nbSimulationVector_print);
	elements_print.push_back(analyticalSwapPrice_print);
	elements_print.push_back(Euler_swapNPVResultVector_print);
	elements_print.push_back(PC_swapNPVResultVector_print);
	elements_print.push_back(IPC_swapNPVResultVector_print);
	elements_print.push_back(Spot_EULER_swapNPVResultVector_print);
	elements_print.push_back(Spot_PC_swapNPVResultVector_print);


	Printer printer(path, elements_print);
	printer.print();

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()