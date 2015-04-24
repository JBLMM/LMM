#include <LMM/Test/Tests.h>
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
#include <ql/termstructures/volatility/abcdcalibration.hpp>
#include <ql/math/optimization/endcriteria.hpp>
#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/problem.hpp>
#include <ql/math/optimization/simplex.hpp>
#include <ql/math/optimization/bfgs.hpp> 
#include <ql/math/optimization/conjugategradient.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>
// ---- include for QuantLib calibration -------

#include <LMM/helper/GenericPath.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/numeric/NumericalMethods.h>
#include <LMM/RNGenerator/McGenerator.h>
#include <LMM/helper/Noise.h>
#include <LMM/helper/InputFileManager.h>
#include <LMM/helper/LmmGnuplotPrinterMatrix.h>

#include <LMM/calibration/ATMSwaptionMarketData.h>
#include <LMM/calibration/SwaptionMarketDataContainer.h>
#include <LMM/calibration/SwaptionMarketDataManager.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/LmmABCDCostFunction.h>
#include <LMM/LmmModel/LmmABCDCalibrator.h>
#include <LMM/LmmModel/LmmSkewCostFunction.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/LmmSwaptionMarketData.h>

#include <LMM/LmmModel/LmmShiftCalibrator.h>

#include <LMM/LmmModel/LmmGlobal_gCalibrator.h>
#include <LMM/LmmModel/LmmGlobal_gCostFunction.h>
#include <LMM/LmmModel/LmmLocal_gCalibrator.h>
#include <LMM/LmmModel/LmmLocal_gCostFunction.h>
#include <LMM/LmmModel/LmmCascade_gCalibrator.h>
#include <LMM/LmmModel/LmmCascade_gCostFunction.h>


void test_calib_gMatrix_OneFile(const std::string& mkt_data_file);

void test_calib_gMatrixNegative_allData();

void test_LmmCalibrationMarketData()
{

	std::vector<std::string> mkt_file_list = InputFileManager::get_VCUB_FileList();
	test_calib_gMatrix_OneFile(mkt_file_list[0]);

	//////series of tests all calibator, with all data files, and allowing negative values of g .
	//test_calib_gMatrixNegative_allData();
}

void test_calib_gMatrix_OneFile(const std::string& mkt_data_file)
{
	std::string folder_name;// = "TotalCalib\\" ;  config.use_positive_constraint_=true;
	std::string base_name_file = LMMPATH::get_BaseFileName(mkt_data_file) + "\\";
	folder_name+=base_name_file;
	LMMPATH::reset_Output_SubFolder(folder_name );

	LmmCalibrationConfig config;
	config.model_nbYear_=16;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_data_file);

	config.correl_ReducedRank_= 30 ; config.correl_alpha_ = 0.000000001 ; config.correl_beta_  = 0.05;
	QuantLib::Array found_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);

	//Correlation_PTR found_correlation_ptr = marketData_LMM_Correlation_calibration(config,pLmmSwaptionMarketData,found_abcd);

	config.correl_ReducedRank_= 3 ;

	//marketData_LMM_CascadeExact_calibration(config, pLmmSwaptionMarketData, found_abcd , create_InitCorrelation(config) );
	//config.use_local_calib_=true;
	//marketData_LMM_Local_gCalibration(config, pLmmSwaptionMarketData, found_abcd , create_InitCorrelation(config), GMatrixMapping_PTR() );
	//config.use_local_calib_=false;
	config.penalty_time_homogeneity_ = 1e-4 ; config.penalty_libor_ = 1e-6 ; config.use_positive_constraint_= true;
	marketData_LMM_Global_gCalibration(config, pLmmSwaptionMarketData, found_abcd , create_InitCorrelation(config), GMatrixMapping_PTR() );
}


void test_calib_gMatrixNegative_allData()
{
	std::vector<std::string> mkt_file_list = InputFileManager::get_VCUB_FileList();
	//size_t nbFile = 2;
	size_t nbFile = mkt_file_list.size();

	LmmCalibrationConfig config;
	config.model_nbYear_=16;

	//std::cout<<"------------------------------------------------------  HORIZON "<<nbYear<<" YR "<<std::endl;//ctndebug_calib_todelete
	for(size_t iFile=0 ; iFile< nbFile ; ++iFile)
	{
		std::string folder_name;// = "TotalCalib\\" ;  config.use_positive_constraint_=true;
		std::string base_name_file = LMMPATH::get_BaseFileName(mkt_file_list[iFile]) + "\\";
		folder_name+=base_name_file;
		LMMPATH::reset_Output_SubFolder(folder_name );

		LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_file_list[iFile]);

		config.correl_ReducedRank_= 30 ; config.correl_alpha_ = 0.000000001 ; config.correl_beta_  = 0.05;
		QuantLib::Array found_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);

		//Correlation_PTR found_correlation_ptr = marketData_LMM_Correlation_calibration(config,pLmmSwaptionMarketData,found_abcd);

		config.correl_ReducedRank_= 3 ;

		//marketData_LMM_CascadeExact_calibration(config, pLmmSwaptionMarketData, found_abcd , create_InitCorrelation(config) );
		//config.use_local_calib_=true;
		//marketData_LMM_Local_gCalibration(config, pLmmSwaptionMarketData, found_abcd , create_InitCorrelation(config), GMatrixMapping_PTR() );
		//config.use_local_calib_=false;
		//config.penalty_time_homogeneity_ = 1e-4 ; config.penalty_libor_ = 1e-6 ; config.use_positive_constraint_=false;
		marketData_LMM_Global_gCalibration(config, pLmmSwaptionMarketData, found_abcd , create_InitCorrelation(config), GMatrixMapping_PTR() );

	}
}