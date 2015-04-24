#include <iostream>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/LmmModel/BetaShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/McTerminalLmm.h>
#include <LMM/LmmModel/McSpotLmm.h>

#include <LMM/instrument/VanillaCaplet.h>
#include <LMM/pricer/McLmmVanillaCapletPricer.h>
#include <LMM/pricer/LmmVanillaCapletPricer.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>


#include <LMM/Test/Test_createLmmModel.h>
//OK 

//----------------------------------------------------------------------------------------------
//
//                                       Test_Caplet
//
//----------------------------------------------------------------------------------------------
VanillaCaplet_PTR get_VanillaCaplet_PTR()	
{
	double      strike        = 0.04; 
	LMM::Index  indexFixing   = 18; 
	LMM::Index  indexPayement = indexFixing + 1;
	Tenor       underlyingLiborTenorType = Tenor::_6M;

	size_t horizonYear = 15;
	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(underlyingLiborTenorType, horizonYear));

	return VanillaCaplet_PTR(new VanillaCaplet(OptionType::CALL, strike, indexFixing, indexPayement, underlyingLiborTenorType, lmmTenorStructure));
}

//----------------------------------------------------------------------------------------------
//
//                                       Test_VanillaCapletPricer
//
//----------------------------------------------------------------------------------------------

//Lmm_PTR get_lmm_for_Caplet(std::vector<double>& liborsInitValue_op)
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
//	Shifted_HGVolatilityParam::ABCDParameter abcdParam(a,b,c,d);
//
//	double g_constParam = 1.0;
//	double shift_constParam = 0.0;
//	ConstShifted_HGVolatilityParam_PTR hgParam( new ConstShifted_HGVolatilityParam(lmmTenorStructure, abcdParam, g_constParam, shift_constParam));
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
//
//	//McLmm
//	//double shiftValue = 0.0; // -0.01;
//	//std::vector<double> shifts(lmmTenorStructure->get_horizon()+1, shiftValue);
//
//	double fwdRate = 0.04;
//	std::vector<double> liborsInitValue(lmmTenorStructure->get_horizon()+1, fwdRate);
//	liborsInitValue_op = liborsInitValue;
//
//	ConstShifted_HGVolatilityFunction_PTR hgVolatilityFunction (new ConstShifted_HGVolatilityFunction(lmmTenorStructure, correlation, hgParam)); 
//
//	//correlation->print("test_VanillaSwaptionPricer_Correlation.csv");
//
//	//! Dispersion
//	Dispersion dispersion(hgVolatilityFunction);
//
//
//	Lmm_PTR lmm (new Lmm(dispersion));
//	return lmm;
//}


//McLmm_PTR get_McLmm_for_Caplet(bool TerminalOrSpotProb,
//							   MCSchemeType::MCSchemeType mcSchemeType, 
//							   Lmm_PTR lmm,
//							   const std::vector<double>& liborsInitValue)
//{
//	unsigned long seed = 47;
//	RNGenerator_PTR  rnGenerator(new McGenerator(seed));
//
//	if(TerminalOrSpotProb)
//	{
//		McLmm_PTR mclmm(new McTerminalLmm (lmm, liborsInitValue, rnGenerator, mcSchemeType));
//		return mclmm;
//	}
//	else
//	{
//		McLmm_PTR mclmm(new McSpotLmm (lmm, liborsInitValue, rnGenerator, mcSchemeType));
//		return mclmm;
//	}
//
//}

void test_McLmm_for_Caplet( bool terminalOrSpotProb, 
						MCSchemeType::MCSchemeType mcSchemeType,
						size_t minNbSimulation, 
						size_t maxNbSimulation,
						double bnSimualtionPowBase,
						//! output
						std::vector<size_t>& nbSimulationVector,  // output
						std::vector<double>& capletResultVector, // output
						double& analytical_capletPrice)              // output
{
	VanillaCaplet_PTR vanillaCaplet = get_VanillaCaplet_PTR();


	Tenor  tenorType          = Tenor::_6M;
	size_t horizonYear        = 15; 
	double liborsInitValueFwd = 0.04;
	double shift              = 0.0;
	TEST::TEST_ShiftedLmmAndMcLmm testHelper =TEST::TEST_ShiftedLmmAndMcLmm(tenorType, 
																			horizonYear,						   
																			liborsInitValueFwd,
																			terminalOrSpotProb,
																			mcSchemeType,
																			shift);
	Lmm_PTR   lmm = testHelper.get_lmm();
	McLmm_PTR mclmm = testHelper.get_mcLmm();
	std::vector<double> liborsInitValue = testHelper.get_liborsInitValues();


	//std::vector<double> liborsInitValue;
	//Lmm_PTR lmm = get_lmm_for_Caplet(liborsInitValue); // init also the libor. 
	//McLmm_PTR mclmm_ptr = get_McLmm_for_Caplet(TerminalOrSpotProb, mcSchemeType,lmm, liborsInitValue);

	McLmmVanillaCapletPricer mcLmmVanillaCapletPricer(mclmm);

	//! analytical swaptionApprox price
	LmmVanillaCapletPricer lmmVanillaCapletPricer(lmm); // YY: maybe a problem of polymorphisms for smart_pointer ??? 
	analytical_capletPrice = lmmVanillaCapletPricer.price(*vanillaCaplet, liborsInitValue);
	std::cout << "analyticalCapletPrice = "<< analytical_capletPrice <<  std::endl;
	double analytical_impliedVol = lmmVanillaCapletPricer.convertPriceToBlackVol(*vanillaCaplet,liborsInitValue,analytical_capletPrice);
	std::cout << "analytical_impliedVol = "<< analytical_impliedVol <<  std::endl;

	//! MC FRA price, TODO: seems a bad thing, these vectors.
	nbSimulationVector.clear();
	capletResultVector.clear();

	for(size_t i=0;; ++i)
	{
		size_t nbSimulation = (size_t)std::pow(bnSimualtionPowBase,(int)i)*minNbSimulation;
		if(nbSimulation>maxNbSimulation)
			break;
		double mcPrice = mcLmmVanillaCapletPricer.price(*vanillaCaplet, nbSimulation);

		std::cout << "mc_price = "<< mcPrice <<  std::endl;
		double mc_impliedVol = lmmVanillaCapletPricer.convertPriceToBlackVol(*vanillaCaplet,liborsInitValue,mcPrice);
		std::cout << "mc_analytical_impliedVol = "<< mc_impliedVol <<  std::endl;


		nbSimulationVector.push_back(nbSimulation);
		capletResultVector.push_back(mcPrice);
		mcLmmVanillaCapletPricer.resetGeneratorToinitSeed(); // for the next generation begin with the same seed as this one...
	}
}



void test_VanillaCapletPricer()
{
	size_t minNbSimulation = 100;
	size_t maxNbSimulation = 300000;
	double bnSimualtionPowBase = 2;

	std::vector<size_t> nbSimulationVector;
	std::vector<double> capletPriceVector;
	double analytical_capletPrice=0.0;

	std::string fileName = "test_VanillaCapletPricer.csv"; //+ MCSchemeType::mcSchemeType2String(mcSchemeType) +".csv";
	std::string path = LMMPATH::get_output_path() + fileName;

	//! Terminal Euler
	test_McLmm_for_Caplet( true, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector, capletPriceVector, analytical_capletPrice);
	std::vector<double> analytical_capletPriceConstVector(nbSimulationVector.size(), analytical_capletPrice);

	PrintElement_PTR nbSimulationVector_print        = PrintElement_PTR(new VectorPrintElement<std::vector<size_t> >("nbSimulationVector", nbSimulationVector));
	PrintElement_PTR analytical_swaptionPrice_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("analytical_capletPriceConstVector", analytical_capletPriceConstVector));
	PrintElement_PTR Euler_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Euler_capletResultVector", capletPriceVector));

	//! Terminal PC
	test_McLmm_for_Caplet( true, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,capletPriceVector, analytical_capletPrice);
	PrintElement_PTR PC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("capletPriceVector", capletPriceVector));

	////! Terminal IPC
	//test_McLmm2( true, MCSchemeType::IPC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
	//	nbSimulationVector,swaptionPriceVector, analytical_swaptionPrice);
	//PrintElement_PTR IPC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("IPC_swapNPVResultVector", swaptionPriceVector));

	//! Spot Euler
	test_McLmm_for_Caplet( false, MCSchemeType::EULER,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,capletPriceVector, analytical_capletPrice);
	PrintElement_PTR Spot_EULER_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_EULER_capletResultVector_print", capletPriceVector));

	//! Spot PC
	test_McLmm_for_Caplet( false, MCSchemeType::PC,minNbSimulation,maxNbSimulation,bnSimualtionPowBase,
		nbSimulationVector,capletPriceVector, analytical_capletPrice);
	PrintElement_PTR Spot_PC_swaptionPriceVector_print  = PrintElement_PTR(new VectorPrintElement<std::vector<double> >("Spot_EULER_capletResultVector_print", capletPriceVector));


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


