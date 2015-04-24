#define BOOST_TEST_MODULE LMM_unit_test_model_Dispersion
#include <boost/test/included/unit_test.hpp>

#include <ql/math/matrixutilities/symmetricschurdecomposition.hpp>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/ModelMCLMM/Correlation.h>
#include <LMM/ModelMCLMM/HGVolatilityFunction.h>
#include <LMM/ModelMCLMM/Dispersion.h>

BOOST_AUTO_TEST_SUITE(lmm_test_model_Dispersion)


BOOST_AUTO_TEST_CASE(test_Dispersion_constructor)
{
	Tenor tenorType = Tenor::_6M;
	const double horizonYear = 15;
	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(tenorType, horizonYear));
	
	//! volatility function
	double a = -0.06;
	double b = 0.17;
	double c = 0.54;
	double d = 0.17;
	DefaultHGVolatilityParam abcdParams(a,b,c,d,lmmTenorStructure);
	
	VolatilityFunction_PTR hgVolatilityFunction (new HGVolatilityFunction(abcdParams,  lmmTenorStructure)); 
	hgVolatilityFunction->print("unitTest_ModelMCLMM_Dispersion_VolatilityFunction.csv");

	//! Correlation 1
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = lmmTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.1;
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.
	correlation->print("unitTest_ModelMCLMM_Dispersion_Correlation.csv");

	//! Dispersion
	Dispersion dispersion(correlation, hgVolatilityFunction);

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_Dispersion_resetVolatilityParam)
{
	Tenor tenorType = Tenor::_6M;
	const double horizonYear = 15;
	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(tenorType, horizonYear));
	
	//! Correlation 1
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = lmmTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.1;
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.
	correlation->print("unitTest_ModelMCLMM_Dispersion_Correlation.csv");

	//! volatility function
	double a=0.01,b=0.01,c=0.01,d=0.01;
	DefaultHGVolatilityParam abcdParams1(a,b,c,d,lmmTenorStructure);
	VolatilityFunction* hgVolatilityFunction1 = new HGVolatilityFunction(abcdParams1,  lmmTenorStructure); 
	VolatilityFunction_PTR p1VolatilityFunction(hgVolatilityFunction1);
	p1VolatilityFunction->print("unitTest_ModelMCLMM_Dispersion_VolatilityFunction1.csv");
	
	//! Dispersion
	Dispersion dispersion(correlation, p1VolatilityFunction);

	double aa=0.0,bb=0.0,cc=0.0,dd=0.0;
	DefaultHGVolatilityParam abcdParams2(aa,bb,cc,dd,lmmTenorStructure);
	
	dispersion.reset_VolatilityParam(abcdParams2);

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()

