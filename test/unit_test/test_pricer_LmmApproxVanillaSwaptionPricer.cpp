#define BOOST_TEST_MODULE LMM_unit_test_pricer_LmmApproxVanillaSwaptionPricer
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <cassert>
#include <string.h>

#include <LMM/helper/Name.h>
#include <LMM/helper/GenericPath.h>
#include <LMM/helper/Printer.h>
#include <LMM/RNGenerator/McGenerator.h>

#include <LMM/helper/TenorType.h>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/ModelMCLMM/Lmm.h>
#include <LMM/ModelMCLMM/Correlation.h>
#include <LMM/ModelMCLMM/HGVolatilityFunction.h>

#include <LMM/pricer/LmmApproxVanillaSwaptionPricer.h>

BOOST_AUTO_TEST_SUITE(lmm_test_pricer_LmmApproxVanillaSwaptionPricer)



BOOST_AUTO_TEST_CASE(test_constructors)
{
	//! LMMTenorStructure
	Tenor simuTenor = Tenor::_6M;
	size_t nbYear = 10;
	LMMTenorStructure_PTR lmmTenorStructure(new LMMTenorStructure(simuTenor, nbYear));
	lmmTenorStructure->print("test_pricer_LMMApproxVanillaSwaptionPricer_SimuTenor.csv");

	//! volatility function
	//double a = -0.06;
	//double b = 0.17;
	//double c = 0.54;
	//double d = 0.17;

	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	double d = 0.2;

	DefaultHGVolatilityParam abcdParams(a,b,c,d, lmmTenorStructure);
	
	VolatilityFunction_PTR hgVolatilityFunction (new HGVolatilityFunction(abcdParams,  lmmTenorStructure)); 
	hgVolatilityFunction->print("test_pricer_LMMApproxVanillaSwaptionPricer_Volatility.csv");

	//! Correlation 
	size_t nbFactor       = 15; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = lmmTenorStructure->get_horizon()+1;
	size_t correlReducedRank = nbFactor;
	CorrelationReductionType::CorrelationReductionType correlReductionType = CorrelationReductionType::PCA;
	double correlAlpha = 0.0;
	double correlBeta  = 0.1;
	Correlation_PTR correlation(new XY_beta_Correlation(correlFullRank,correlReducedRank, correlReductionType,correlAlpha,correlBeta));
	correlation->calculate(); // for print.
	correlation->print("test_pricer_LMMApproxVanillaSwaptionPricer_Correlation.csv");

	//! Dispersion
	Dispersion dispersion(correlation, hgVolatilityFunction);

	//Lmm
	double shiftValue = 0.0; // -0.01;
	std::vector<double> shifts(lmmTenorStructure->get_horizon()+1, shiftValue);

	double fwdRate = 0.04;
	std::vector<double> liborsInitValue(lmmTenorStructure->get_horizon()+1, fwdRate);

	Lmm_PTR lmm(new Lmm(dispersion, shifts) );
	//std::cout<<*lmm<<std::endl<<std::endl<<std::endl;
	
	// create Vanilla Swap
	double strike = 0.04;
	LMM::Index indexStart = 12; // 12
	LMM::Index indexEnd   = 18; // 28;
	Tenor floatingLegTenorType = Tenor::_6M;
	Tenor fixedLegTenorType    = Tenor::_1YR;

	LMM::VanillaSwap vanillaSwap(strike, indexStart, indexEnd, floatingLegTenorType, fixedLegTenorType, lmmTenorStructure);
	std::cout<< vanillaSwap <<std::endl;

	// create Swaption
	LMM::Index indexMaturity = indexStart;
	VanillaSwaption vanillaSwaption(vanillaSwap,OptionType::CALL) ;

	LmmApproxVanillaSwaptionPricer approxSwaptionPricer(lmm);
	
	double swapRate = approxSwaptionPricer.swapRate_Analytical(vanillaSwap, liborsInitValue); // swapRate at time 0

	double approxVolBlack = approxSwaptionPricer.volBlack(vanillaSwaption, liborsInitValue);

	std::cout<<"Test LMM Approx Vanilla Swaption Pricer "<<std::endl
		     <<" Vol Black : "<<approxVolBlack<<std::endl
			 <<" Analitical Swap_rate : "<<swapRate<<std::endl;

	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
