#include <iostream>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/McTerminalLmm.h>
#include <LMM/LmmModel/McSpotLmm.h>

#include <LMM/instrument/VanillaFRA.h>
#include <LMM/pricer/McLmmVanillaFRAPricer.h>
#include <LMM/pricer/LmmVanillaFRAPricer.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>

#include <LMM/Test/Test_createLmmModel.h>
// OK

//----------------------------------------------------------------------------------------------
//
//                                       Test_FRA
//
//----------------------------------------------------------------------------------------------
VanillaFRA_PTR get_VanillaFRA_PTR()	
{
	LMM::Index  indexFixing   = 13; 
	LMM::Index  indexPayement = indexFixing + 1;
	Tenor       underlyingLiborTenorType = Tenor::_6M;

	size_t horizonYear = 15;
	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(underlyingLiborTenorType, horizonYear));
	
	return VanillaFRA_PTR(new VanillaFRA(indexFixing, indexPayement, underlyingLiborTenorType, lmmTenorStructure));
}

//Lmm_PTR get_lmm_for_FRA(std::vector<double>& liborsInitValue_op)
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
//	double d = 0.37;
//	DefaultHGVolatilityParam abcdParams(a,b,c,d,lmmTenorStructure);
//
//	
//	//hgVolatilityFunction->print("test_VanillaSwaptionPricer_Volatility.csv");
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
//
//	//McLmm
//	double shiftValue = 0.0; // -0.01;
//	std::vector<double> shifts(lmmTenorStructure->get_horizon()+1, shiftValue);
//
//	double fwdRate = 0.04;
//	std::vector<double> liborsInitValue(lmmTenorStructure->get_horizon()+1, fwdRate);
//	liborsInitValue_op = liborsInitValue;
//
//	Shifted_HGVolatilityFunction_PTR hgVolatilityFunction (new Shifted_HGVolatilityFunction(abcdParams,  lmmTenorStructure, correlation, shifts)); 
//	//correlation->print("test_VanillaSwaptionPricer_Correlation.csv");
//
//	//! Dispersion
//	Dispersion dispersion(hgVolatilityFunction);
//
//
//
//	Lmm_PTR lmm_ptr (new Lmm(dispersion));
//	return lmm_ptr;
//}

////----------------------------------------------------------------------------------------------
////
////                                       Test_VanillaCapletPricer
////
////----------------------------------------------------------------------------------------------
//McLmm_PTR get_McLmm_for_FRA(bool TerminalOrSpotProb, 
//							MCSchemeType::MCSchemeType mcSchemeType, 
//							const Lmm_PTR lmm_ptr,
//							const std::vector<double>& liborsInitValue)
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
//
//}

void test_McLmm_for_FRA( bool terminalOrSpotProb, 
				 MCSchemeType::MCSchemeType mcSchemeType,
				 size_t minNbSimulation, 
				 size_t maxNbSimulation,
				 double bnSimualtionPowBase,
				 //! output
				 std::vector<size_t>& nbSimulationVector,  // output
				 std::vector<double>& swapNPVResultVector, // output
				 double& analyticalFRAPrice)              // output
{
	VanillaFRA_PTR vanillaFRA = get_VanillaFRA_PTR();


	Tenor  tenorType          = Tenor::_6M;
	size_t horizonYear        = 15; 
	double liborsInitValueFwd = 0.04;
	double shift              = 0.01;
	TEST::TEST_ShiftedLmmAndMcLmm testHelper =TEST::TEST_ShiftedLmmAndMcLmm(    tenorType, 
																				horizonYear,						   
																				liborsInitValueFwd,
																				terminalOrSpotProb,
																				mcSchemeType,
																				shift);
	Lmm_PTR lmm_ptr = testHelper.get_lmm();
	McLmm_PTR mclmm_ptr = testHelper.get_mcLmm();
	std::vector<double> liborsInitValue = testHelper.get_liborsInitValues();


	McLmmVanillaFRAPricer mcLmmVanillaFRAPricer(mclmm_ptr);

	//! analytical swaptionApprox price
	LmmVanillaFRAPricer lmmVanillaFRAPricer(lmm_ptr); // YY: maybe a problem of polymorphism for smart_pointer ??? 
	analyticalFRAPrice = lmmVanillaFRAPricer.price(*vanillaFRA, liborsInitValue);
	std::cout << "analyticalFRAPrice = "<< analyticalFRAPrice <<  std::endl;

	//! MC FRA price, TODO: seems a bad thing, these vectors.
	nbSimulationVector.clear();
	swapNPVResultVector.clear();

	for(size_t i=0;; ++i)
	{
		size_t nbSimulation = (size_t)std::pow(bnSimualtionPowBase,(int)i)*minNbSimulation;
		if(nbSimulation>maxNbSimulation)
			break;
		double price = mcLmmVanillaFRAPricer.price(*vanillaFRA, nbSimulation);

		nbSimulationVector.push_back(nbSimulation);
		swapNPVResultVector.push_back(price);
		mcLmmVanillaFRAPricer.resetGeneratorToinitSeed(); // for the next generation begin with the same seed as this one...
	}
}



void test_VanillaFRAPricer()
{
	size_t minNbSimulation = 1;
	size_t maxNbSimulation = 110000;
	double bnSimualtionPowBase = 2;

	std::vector<size_t> nbSimulationVector;
	std::vector<double> FRAPriceVector;
	double analytical_FRAPrice=0.0;

	std::string fileName = "test_VanillaSwaptionPricer.csv"; //+ MCSchemeType::mcSchemeType2String(mcSchemeType) +".csv";
	std::string path = LMMPATH::get_output_path() + fileName;

	//! Terminal Euler
	test_McLmm_for_FRA( true, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector, FRAPriceVector, analytical_FRAPrice);
	std::vector<double> analytical_swaptionPriceConstVector(nbSimulationVector.size(), analytical_FRAPrice);

	PrintElement_PTR nbSimulationVector_print        = PrintElement_PTR(new VectorPrintElement<std::vector<size_t> >("nbSimulationVector", nbSimulationVector));
	PrintElement_PTR analytical_swaptionPrice_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("analytical_swaptionPrice", analytical_swaptionPriceConstVector));
	PrintElement_PTR Euler_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Euler_FRAResultVector", FRAPriceVector));

	//! Terminal PC
	test_McLmm_for_FRA( true, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,FRAPriceVector, analytical_FRAPrice);
	PrintElement_PTR PC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("PC_FRAResultVector", FRAPriceVector));


	////! Terminal IPC
	//test_McLmm2( true, MCSchemeType::IPC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
	//	nbSimulationVector,swaptionPriceVector, analytical_swaptionPrice);
	//PrintElement_PTR IPC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("IPC_swapNPVResultVector", swaptionPriceVector));


	//! Spot Euler
	test_McLmm_for_FRA( false, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,FRAPriceVector, analytical_FRAPrice);
	PrintElement_PTR Spot_EULER_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_EULER_FRAResultVector_print", FRAPriceVector));

	//! Spot PC
	test_McLmm_for_FRA( false, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,FRAPriceVector, analytical_FRAPrice);
	PrintElement_PTR Spot_PC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_PC_FRAResultVector_print", FRAPriceVector));


	std::vector<PrintElement_PTR> elements_print;
	elements_print.push_back(nbSimulationVector_print);
	elements_print.push_back(analytical_swaptionPrice_print);
	elements_print.push_back(Euler_swaptionPriceVector_print);
	elements_print.push_back(PC_swaptionPriceVector_print);
	//elements_print.push_back(IPC_swaptionPriceVector_print);
	elements_print.push_back(Spot_EULER_swaptionPriceVector_print);
	elements_print.push_back(Spot_PC_swaptionPriceVector_print);


	Printer printer(path, elements_print);
	printer.print();
}


