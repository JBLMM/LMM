#include <LMM/Test/Tests.h>

#include <iostream> 
#include <fstream> 
#include <string> 

#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/RNGenerator/RNGenerator.h>

#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/McTerminalLmm.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>

#include <JBLMM/Pricer/McGeneticSwapLMMPricer.h>
#include <JBLMM/Instrument/InstrumentFactory.h>
#include <JBLMM/Pricer/GeneticVanillaSwapPricer.h>
#include <JBLMM/Pricer/McGeneticTargetSwapLmmPricer.h>


void Test_McGeneticTargetSwapLMMPricing()
{
	//! LMMTenorStructure
	double	strike			=	0.02;
	LMM::Index	indexStart	=	0;
	LMM::Index	indexEnd	=	4;
	Tenor	floatingTenor	=	Tenor::_6M;
	Tenor	fixedTenor		=	Tenor::_12M;
	Tenor	tenorType		=	Tenor::_6M;
	size_t	horizonYear		=	2;
	LMMTenorStructure_PTR lmmTenorStructure( new LMMTenorStructure(tenorType, horizonYear));
	size_t	nbSimulation	=	1;
	double	nominal			=	1.0;
	double	target			=	0.02;

	double	fwdRate		=	0.02;
	std::vector<double> liborsInitValue(lmmTenorStructure->get_horizon()+1, fwdRate);


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

	unsigned long seed = 93;
	RNGenerator_PTR  rnGenerator(new McGenerator(seed));

	Lmm_PTR shiftedLmm (new Lmm(dispersion));

	//build a McGeneticSwapLMMPricer
	McGeneticSwapLMMPricer_PTR mcGeneticTargetSwapLMMPricer(new McGeneticTargetSwapLmmPricer(McLmm_PTR(new McTerminalLmm(shiftedLmm, liborsInitValue, rnGenerator, MCSchemeType::EULER))));
	//build the vanillaSwap in the way of GeneticSwap
	GeneticSwap_CONSTPTR targetSwap_Genetic=InstrumentFactory::createStandardTARNSwap(
			strike, indexStart, indexEnd, floatingTenor, fixedTenor, lmmTenorStructure, nominal, target);

	//use Monte Carlo Method
	double	MonteCarloTargetPrice		=	mcGeneticTargetSwapLMMPricer->swapNPV(targetSwap_Genetic, nbSimulation);

	cout	<<	"MonteCarloTargetPrice: "	<<	MonteCarloTargetPrice	<<	endl;
	//cout	<<	"Difference between MonteCarloPrice and OrdinaryGeneticVanillaSwapPrice: "		<<	MonteCarloTargetPrice-OrdinaryGeneticVanillaSwapPrice	<<	endl;	


	McTerminalLmm mcTerminalLmm(shiftedLmm, liborsInitValue, rnGenerator, MCSchemeType::EULER);
    mcTerminalLmm.print("test_McTerminalLmm_McTerminal.csv");  // Attention, there are two print: parent class and derived class
}
