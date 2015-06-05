#include <LMM/Test/Tests.h>

#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/RNGenerator/RNGenerator.h>

#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/McTerminalLmm.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>

#include <McGeneticSwapLMMPricer.h>
#include <InstrumentFactory.h>
#include <GeneticVanillaSwapPricer.h>


/*
void Test_McGeneticSwapLMMPricer()
{
	//! LMMTenorStructure
	double strike=0.06;
	size_t indexStart=8;
	size_t indexEnd=18;

	Tenor floatingTenor=Tenor::_6M;
	Tenor fixedTenor=Tenor::_12M;

	Tenor tenorType = Tenor::_6M;
	size_t horizonYear = 10;
	LMMTenorStructure_PTR lmmTenorStructure( new LMMTenorStructure(tenorType, horizonYear));
	//lmmTenorStructure->print("test_McTerminalLmm_Tenor.csv"); // it will not work any more, because I delete the "<<" operator

	//! volatility function
	double a = -0.06;
	double b = 0.17;
	double c = 0.54;
	double d = 0.17;
	Shifted_HGVolatilityParam::ABCDParameter abcdParam (a,b,c,d);

	double g_constParam = 1.0;
	double shift_constParam = -0.01;

	ConstShifted_HGVolatilityParam_PTR hgParam( new ConstShifted_HGVolatilityParam(lmmTenorStructure,abcdParam,g_constParam,shift_constParam));
	


	//! Correlation 1
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = lmmTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.1;
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.
	correlation->print("test_McTerminalLmm_Correlation.csv");

	//McLmm
	//double shiftValue = -0.01;
	//std::vector<double> shifts(lmmTenorStructure->get_horizon()+1, shiftValue);

	double fwdRate = 0.02;
	std::vector<double> liborsInitValue(lmmTenorStructure->get_horizon()+1, fwdRate);


	ConstShifted_HGVolatilityFunction_PTR hgVolatilityFunction (new ConstShifted_HGVolatilityFunction(lmmTenorStructure, correlation, hgParam)); 
	hgVolatilityFunction->print("test_McTerminalLmm_Volatility.csv");

	////! Correlation 2: don't do reduction.
	//size_t nbFactor          = lmmTenorStructure.get_horizon()+1;; // need to test nbFactor  = 3, and nbFactor = 
	//size_t correlFullRank    = lmmTenorStructure.get_horizon()+1;
	//size_t correlReducedRank = nbFactor;
	//CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	//double correlConst       = 1.0; 
	//Correlation_PTR correlation(new Const_Correlation(correlFullRank,correlReducedRank,correlReductionType,correlConst));


	//! Dispersion
	Dispersion dispersion(hgVolatilityFunction);



	unsigned long seed = 47;
	RNGenerator_PTR  rnGenerator(new McGenerator(seed));

	size_t nbSimulation = 1;

	Lmm_PTR shiftedLmm (new Lmm(dispersion));

	//build a McGeneticSwapLMMPricer
	McGeneticSwapLMMPricer_PTR mcGeneticSwapLMMPricer(new McGeneticSwapLMMPricer(McLmm_PTR(new McTerminalLmm(shiftedLmm, liborsInitValue, rnGenerator, MCSchemeType::EULER))));

	GeneticSwap_PTR vanillaSwap_Genetic=InstrumentFactory::createVanillaSwap(
			strike,indexStart,indexEnd,floatingTenor,fixedTenor,lmmTenorStructure,1.0, liborsInitValue);

	LMMTenorStructure_PTR lmmTenorStructure1=mcGeneticSwapLMMPricer->getMcLmm()->get_lmm()->get_LMMTenorStructure();
	double price=mcGeneticSwapLMMPricer->swapNPV(vanillaSwap_Genetic, 1000, lmmTenorStructure1);

	std::cout<< "price: "<<price<< endl;

	GeneticVanillaSwapPricer_PTR geneticVanillaSwapPricer(new GeneticVanillaSwapPricer(vanillaSwap_Genetic));
	double geneticPrice=geneticVanillaSwapPricer->geneticVanillaSwap_Analytical_1(liborsInitValue, lmmTenorStructure->get_tenorDate());
	cout << "GeneticSwapTest: "<<geneticPrice << endl;

	McTerminalLmm mcTerminalLmm(shiftedLmm, liborsInitValue, rnGenerator, MCSchemeType::EULER);
    mcTerminalLmm.print("test_McTerminalLmm_McTerminal.csv");  // Attention, there are two print: parent class and derived class
}
*/