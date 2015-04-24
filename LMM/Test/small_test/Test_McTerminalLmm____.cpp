#define BOOST_TEST_MODULE LMM_unit_test_model
#include <boost/test/included/unit_test.hpp>

#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/RNGenerator/RNGenerator.h>

#include <LMM/ModelMCLMM/Correlation.h>
#include <LMM/ModelMCLMM/HGVolatilityFunction.h>
#include <LMM/ModelMCLMM/McTerminalLmm.h>


BOOST_AUTO_TEST_SUITE(lmm_test_model_McTerminalLmm)

BOOST_AUTO_TEST_CASE(test_McTerminalLmm)  // OK: Test passed!
{
	//! LMMTenorStructure
	Tenor tenorType = Tenor::_6M;
	size_t horizonYear = 2;
	LMMTenorStructure_PTR lmmTenorStructure( new LMMTenorStructure(tenorType, horizonYear));
	lmmTenorStructure->print("test_McTerminalLmm_Tenor.csv");

	//! volatility function
	double a = -0.06;
	double b = 0.17;
	double c = 0.54;
	double d = 0.17;
	DefaultHGVolatilityParam abcdParams(a,b,c,d,lmmTenorStructure);
	
	VolatilityFunction_PTR hgVolatilityFunction (new HGVolatilityFunction(abcdParams,  lmmTenorStructure)); 
	hgVolatilityFunction->print("test_McTerminalLmm_Volatility.csv");

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
	////! Correlation 2: don't do reduction.
	//size_t nbFactor          = lmmTenorStructure.get_horizon()+1;; // need to test nbFactor  = 3, and nbFactor = 
	//size_t correlFullRank    = lmmTenorStructure.get_horizon()+1;
	//size_t correlReducedRank = nbFactor;
	//CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	//double correlConst       = 1.0; 
	//Correlation_PTR correlation(new Const_Correlation(correlFullRank,correlReducedRank,correlReductionType,correlConst));


	//! Dispersion
	Dispersion dispersion(correlation, hgVolatilityFunction);


	//McLmm
	double shiftValue = -0.01;
	std::vector<double> shifts(lmmTenorStructure->get_horizon()+1, shiftValue);

	double fwdRate = 0.02;
	std::vector<double> liborsInitValue(lmmTenorStructure->get_horizon()+1, fwdRate);

	unsigned long seed = 47;
	RNGenerator_PTR  rnGenerator(new McGenerator(seed));

	size_t nbSimulation = 1;

	McTerminalLmm mcTerminalLmm(dispersion, shifts, liborsInitValue, rnGenerator, MCSchemeType::EULER);

    mcTerminalLmm.print("test_McTerminalLmm_McTerminal.csv");  // Attention, there are two print: parent class and derived class
	
	
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
