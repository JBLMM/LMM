#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/Test/Test_CalibrationConfig.h>

#include <iostream>
#include <cassert>
#include <string.h>
#include <cmath>
#include <fstream>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <ctime>
#include <cmath>

// ---- include for QuantLib calibration -------
#include <ql/math/optimization/endcriteria.hpp>
#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/problem.hpp>
#include <ql/math/optimization/simplex.hpp>
#include <ql/math/optimization/bfgs.hpp> 
#include <ql/math/optimization/conjugategradient.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
// ---- include for QuantLib calibration -------

#include <LMM/LmmModel/LmmCorrelationCostFunction.h>
#include <LMM/LmmModel/LmmCorrelationCalibrator.h>

#include <LMM/helper/InputFileManager.h>


void test_LmmVirtualCalibration_XY_beta_Correlation(const Test_CalibrationConfig& config);

void test_MarketDataLmmCorrelationCalibration();

void test_LmmVirtualCorrelationCalibration()
{
	//Test_CalibrationConfig config;

	//config.g_diag_noise=0;
	//config.perturbe_noise=0.5;
	//config.nbYear=16;

	//config.re_init();

	//test_LmmVirtualCalibration_XY_beta_Correlation(config);

	test_MarketDataLmmCorrelationCalibration();
}

void test_MarketDataLmmCorrelationCalibration()
{
	std::vector<std::string> mkt_file_list = InputFileManager::get_VCUB_FileList();
	size_t nbFile = mkt_file_list.size();

	LmmCalibrationConfig config;
	config.model_nbYear_=16;

	for(size_t iFile=0;iFile<nbFile;++iFile)
	{
		LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_file_list[iFile]);
		QuantLib::Array found_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);
		Correlation_PTR pCorrelation = marketData_LMM_Correlation_calibration(config,pLmmSwaptionMarketData,found_abcd);
	}
}



void test_LmmVirtualCalibration_XY_beta_Correlation(const Test_CalibrationConfig& config)
{
	const QuantLib::Array xy_correlation_init    = config.pCorrelation->get_ArrayFrom_Correlation() ;
	config.pCorrelation->print("true_Correlation.csv");

	const QuantLib::Array xy_correlation_perturb = config.get_Perturbed_Array(xy_correlation_init)  ;

	config.pCorrelation->reset_FromArray(xy_correlation_perturb);

	config.pLmmVanillaSwaptionApproxPricer_Rebonato->update_Correlation();
	config.pCorrelation->print("perturbed_Correlation.csv");


	LmmBaseCostFunction_PTR pLmmCorrelationCostFunction(new LmmCorrelationCostFunction
		(
		config.pLmmVanillaSwaptionApproxPricer_Rebonato,
		config.pLiborQuotes,
		config.pUpperTriangleVanillaSwaptionQuotes,
		config.pCorrelation
		) );

	{
		////// Correlation calibrator
		QuantLib::Size maxIterations =1000;
		QuantLib::Size minStatIterations =100;
		QuantLib::Real rootEpsilon =1e-2;
		QuantLib::Real functionEpsilon =1e-2;				

		LmmCorrelationCalibrator lmmCorrelationCalibrator
			(
			xy_correlation_init
			,maxIterations
			,rootEpsilon
			,functionEpsilon
			, pLmmCorrelationCostFunction
			);

		lmmCorrelationCalibrator.solve();

		lmmCorrelationCalibrator.printPlusPlus("correlation_calibration_result.csv");

		config.pCorrelation->print("calibrated_Correlation.csv");
	}

	std::cout<<"test_LmmVirtualCalibration_XY_beta_Correlation() called "<<std::endl;
}