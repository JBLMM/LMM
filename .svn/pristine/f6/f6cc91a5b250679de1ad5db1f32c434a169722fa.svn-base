#define BOOST_TEST_MODULE LMM_unit_test_calibration_RebonatoVolatilityCostFunction
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <cassert>
#include <string.h>

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/ATMSwaptionStructuredData.h>

#include <LMM/ModelMCLMM/Lmm.h>
#include <LMM/ModelMCLMM/Correlation.h>
#include <LMM/ModelMCLMM/HGVolatilityFunction.h>
#include <LMM/pricer/LmmApproxVanillaSwaptionPricer.h>


#include <LMM/calibration/RebonatoVolatilityCostFunction.h>

BOOST_AUTO_TEST_SUITE(lmm_test_calibration_RebonatoVolatilityCostFunction)

	BOOST_AUTO_TEST_CASE(test_constructors)
{
	Tenor tenorlmm = Tenor::_6M;
	Tenor tenorfixedleg = Tenor::_6M;
	Tenor tenorfloatleg = Tenor::_6M;
	size_t nbYear = 3;

	LMMTenorStructure_PTR pLmmTenorStructure( new LMMTenorStructure(tenorlmm, nbYear) );


	// =================   CREATION OF LMM_PTR   =============================
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	double d = 0.2;

	VolatilityParam_PTR abcdParams_ptr(new DefaultHGVolatilityParam(a,b,c,d,pLmmTenorStructure) );
	
	VolatilityFunction_PTR hgVolatilityFunction (new HGVolatilityFunction(dynamic_cast<const HGVolatilityParam&>( *abcdParams_ptr) ,  pLmmTenorStructure)); 
	hgVolatilityFunction->print("test_pricer_LMMApproxVanillaSwaptionPricer_Volatility.csv");

	//! Correlation 
	size_t nbFactor       = 3; // need to test nbFactor  = 3, and nbFactor = 
	size_t correlFullRank = pLmmTenorStructure->get_horizon()+1;
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
	std::vector<double> shifts(pLmmTenorStructure->get_horizon()+1, shiftValue);

	double fwdRate = 0.04;
	std::vector<double> liborsInitValue(pLmmTenorStructure->get_horizon()+1, fwdRate);

	Lmm_PTR lmm_ptr(new Lmm(dispersion, shifts) );
	// =================  END  CREATION OF LMM_PTR   =============================




	ATMSwaptionStructuredData_PTR pATMSwaptionStructuredData(new ATMSwaptionStructuredData( pLmmTenorStructure ) ) ;
	std::string path_FileIn = LMM::get_runtime_datapath() + "testATMSwaption_Mtk_info.csv";
	ATMSwaptionMarketData atmSwaptionMarketData;
	atmSwaptionMarketData.readDataFromFile( path_FileIn );
	pATMSwaptionStructuredData->parseFromMarketData(atmSwaptionMarketData , tenorfixedleg, tenorfloatleg);


	//preparing for deprecated constructor of 	RebonatoVolatilityCostFunction
	LmmApproxVanillaSwaptionPricer approximation(lmm_ptr);					 
	std::vector<double> libor_shifts(pATMSwaptionStructuredData->get_nbLIBOR() );
	
	
	RebonatoVolatilityCostFunction::matrix_ weights;          //ctntodo activate for weights
	RebonatoVolatilityCostFunction::matrix_ weights_maturity;
	RebonatoVolatilityCostFunction::matrix_ weights_tenor;
	RebonatoVolatilityCostFunction::matrix_ weights_maturity_2;
	RebonatoVolatilityCostFunction::matrix_ weights_tenor_2;


	RebonatoVolatilityCostFunction swpmApproxCostFunction
		(abcdParams_ptr, pATMSwaptionStructuredData,
		approximation,			
		libor_shifts,
		weights,
		weights_maturity,
		weights_tenor,
		weights_maturity_2,
		weights_tenor_2
		);

	const size_t nbVolParams = 4;

	
	QuantLib::Array x(nbVolParams);
	x[0]=0 ; x[1]=0 ; x[2]=0 ; x[3]=0 ;//
	//abcdParams_ptr->reset_FromArray(x);                // reset the volatility parameters
	//lmm_ptr->reset_VolatilityParam(*abcdParams_ptr);   // reset the computation covariance to the new parameters

	QuantLib::Array vector_value1 =  swpmApproxCostFunction.values(x);
	QuantLib::Real  value1        =  swpmApproxCostFunction.value(x);


	x[0]=1 ; x[1]=1 ; x[2]=1 ; x[3]=1 ;//
	//abcdParams_ptr->reset_FromArray(x);                // reset the volatility parameters
	//lmm_ptr->reset_VolatilityParam(*abcdParams_ptr);   // reset the computation covariance to the new parameters

	QuantLib::Real  value2        =  swpmApproxCostFunction.value(x);
	QuantLib::Array vector_value2 =  swpmApproxCostFunction.values(x);

	// value1 has to be different than value2 after reset the volatility param
	// todo maybe find the test more rigouruous
	BOOST_CHECK( value1 != value2 );
}

BOOST_AUTO_TEST_SUITE_END()