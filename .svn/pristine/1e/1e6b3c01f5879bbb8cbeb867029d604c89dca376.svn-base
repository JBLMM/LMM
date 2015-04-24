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

void test_LmmGenericSensitivity(const std::string& mkt_data_filename);

void test_LmmSensitivityCorrelationReducedRank();

void test_LmmSensitivityABCDReducedRank();

void test_Calib_gMatrix(const LmmCalibrationConfig& config , const std::string& mkt_data_filename);

void test_Stability_ABCD_vs_Correl(const std::string& mkt_data_filename);

void test_ABCDSensitivity_ABCD_vs_nbFactor_N_Alpha();

void test_calibration_Cascade_then_Global(const std::string& mkt_data_filename);

void test_LmmCalibrationSensitivity()
{
	std::vector<std::string> mkt_file_list = InputFileManager::get_VCUB_FileList();

	//test_LmmGenericSensitivity(mkt_file_list.back());// test to run one by one
	test_LmmGenericSensitivity(mkt_file_list[0]);// test to run one by one
	//test_LmmGenericSensitivity(mkt_file_list[8]);// test to run one by one
	//test_LmmGenericSensitivity(mkt_file_list[16]);// test to run one by one

	//test_LmmSensitivityABCDReducedRank();

	//test_LmmSensitivityCorrelationReducedRank();	

	//test_Stability_ABCD_vs_Correl(mkt_file_list.back() );

	//test_ABCDSensitivity_ABCD_vs_nbFactor_N_Alpha();

	//test_calibration_Cascade_then_Global(mkt_file_list[8]);
}


void test_calibration_Cascade_then_Global(const std::string& mkt_data_filename)
{
	LmmCalibrationConfig config;
	config.reset_nbYear(16);
	config.correl_ReducedRank_= 3;

	config.correl_alpha_ = 0.000000001;
	config.correl_beta_  = 0.05;

	config.vol_abcd_.a_  = 0.25673;
	config.vol_abcd_.b_  = 0.00002;
	config.vol_abcd_.c_  = 0.547;
	config.vol_abcd_.d_  = 0.13998;

	config.use_positive_constraint_=true;
	

	std::stringstream foldername ;

	std::string base_name_file = LMMPATH::get_BaseFileName(mkt_data_filename) + "\\";
	foldername<<"Cascade_Then_Global\\";

	foldername << base_name_file ;

	LMMPATH::reset_Output_SubFolder(foldername.str() );


	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_data_filename );

	GMatrixMapping_PTR pGMatrixMapping = marketData_LMM_CascadeExact_calibration(config, pLmmSwaptionMarketData, config.get_abcdArray() ,create_InitCorrelation(config));
	
	marketData_LMM_Global_gCalibration(config, pLmmSwaptionMarketData,config.get_abcdArray(),create_InitCorrelation(config),pGMatrixMapping );	
}


void test_ABCDSensitivity_ABCD_vs_nbFactor_N_Alpha()
{
	///// create a grid of nbFactor and alpha, calibrate abcd throu this grid and see the quality of calibrator

	std::vector<std::string> file_list = InputFileManager::get_VCUB_FileList();

	const std::string& mkt_data_filename = file_list.back();

	std::string base_output_folder = "Test_ABCDSensitivity\\";
	std::string base_name_file = LMMPATH::get_BaseFileName(mkt_data_filename);

	for(size_t nbFactor =3; nbFactor<33; nbFactor+=5)
	{
		for(double correl_alpha=0.0000001;correl_alpha<=0.00000011;correl_alpha+=0.03)
		{			

			LmmCalibrationConfig config;

			config.reset_nbYear(16);

			config.correl_ReducedRank_= nbFactor;

			config.correl_alpha_= 0.0000001;
			config.correl_beta_= correl_alpha;

			std::ostringstream sub_folder_name;

			sub_folder_name<<"_nbF_"<<nbFactor<<"_alp_"<<correl_alpha;

			std::string output_folder = base_output_folder + sub_folder_name.str() + "\\";

			LMMPATH::reset_Output_SubFolder(output_folder );

			LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_data_filename );

			QuantLib::Array calibrated_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);
		}
	}
}

void test_Stability_ABCD_vs_Correl(const std::string& mkt_data_filename)
{
	// loop testing if calib of abcd, then correl, then reiterate by the result is stable

	LmmCalibrationConfig config;

	config.reset_nbYear(16);
	config.correl_ReducedRank_= 30;

	config.correl_alpha_= 1. ;
	config.correl_beta_=  1. ;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_data_filename );

	size_t loop_counter=0;
	std::string base_folder = "Test_ABCD_vs_Correl\\";
	const std::string base_path = LMMPATH::get_output_path() +  base_folder;
	std::string common_result_file = base_path + "Evolution_abcd_correlation.csv";
	
	while(loop_counter<10)
	{
		{			
			std::ofstream common_result ; common_result.open(common_result_file.c_str(), std::ios::app);

			common_result<<"-------------- loop "<<loop_counter<<std::endl;
			common_result<<"a,b,c,d, ,, alpha,beta,"<<std::endl;
			common_result<<"a,b,c,d, ,, alpha,beta,"<<std::endl;
			common_result<<config.vol_abcd_.a_<<","<<config.vol_abcd_.b_<<","<<config.vol_abcd_.c_<<","<<config.vol_abcd_.d_<<", ,,";
			common_result<<config.correl_alpha_<<","<<config.correl_beta_<<","<<std::endl<<std::endl;
			common_result.close();
		}

		std::stringstream foldername ;
		foldername<<"loop_"<<loop_counter;
		std::string full_path_folder = base_folder + foldername.str() + "\\";
		LMMPATH::reset_Output_SubFolder( full_path_folder );	

		QuantLib::Array calibrated_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);
		config.vol_abcd_.a_ = calibrated_abcd[0];
		config.vol_abcd_.b_ = calibrated_abcd[1];
		config.vol_abcd_.c_ = calibrated_abcd[2];
		config.vol_abcd_.d_ = calibrated_abcd[3];

		Correlation_PTR pCorrelation = marketData_LMM_Correlation_calibration(config,pLmmSwaptionMarketData, calibrated_abcd );
		QuantLib::Array calibrated_correlation = pCorrelation->get_ArrayFrom_Correlation();
		config.correl_alpha_= calibrated_correlation[0];
		config.correl_beta_= calibrated_correlation[1];

		++loop_counter;
	}
}

void test_Calib_gMatrix(const LmmCalibrationConfig& config , const std::string& mkt_data_filename)
{
	std::string folder_data_file = "WithPositiveConstraint\\" + LMMPATH::get_BaseFileName(mkt_data_filename) + "\\";

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_data_filename);

	{
		std::string sub_folder_data_file = folder_data_file + "Cascade\\";
		LMMPATH::reset_Output_SubFolder(sub_folder_data_file );
		GMatrixMapping_PTR pGMatrixMapping = marketData_LMM_CascadeExact_calibration(config, pLmmSwaptionMarketData, config.get_abcdArray() ,create_InitCorrelation(config));
		config.print("calib_config.csv");
	}

	{
		config.use_local_calib_=false;
		std::string sub_folder_data_file = folder_data_file + "Global\\";
		LMMPATH::reset_Output_SubFolder(sub_folder_data_file );
		marketData_LMM_Global_gCalibration(config, pLmmSwaptionMarketData,config.get_abcdArray(),create_InitCorrelation(config), GMatrixMapping_PTR() );
		config.print("calib_config.csv");
	}

	{
		config.use_local_calib_=true;
		std::string sub_folder_data_file = folder_data_file + "Local\\";
		LMMPATH::reset_Output_SubFolder(sub_folder_data_file );
		marketData_LMM_Global_gCalibration(config, pLmmSwaptionMarketData,config.get_abcdArray(),create_InitCorrelation(config), GMatrixMapping_PTR() );
		config.print("calib_config.csv");
	}


}

void test_LmmGenericSensitivity(const std::string& mkt_data_filename)
{

	LmmCalibrationConfig config;
	config.reset_nbYear(16);
	config.correl_ReducedRank_= 3;

	config.correl_alpha_ = 0.000000001;
	config.correl_beta_  = 0.05;

	config.vol_abcd_.a_  = 0.136554  ;
	config.vol_abcd_.b_  = 8.45E-06  ;
	config.vol_abcd_.c_  = 0.0439938 ;
	config.vol_abcd_.d_  = 0.0269172 ;
	
	config.penalty_libor_ = 1e-6;
	config.penalty_time_homogeneity_ = 1e-4;

	config.use_positive_constraint_=true;
	
	std::stringstream foldername ;

	std::string base_name_file = LMMPATH::get_BaseFileName(mkt_data_filename) + "\\";
	foldername<<"Regularized_";

	foldername << base_name_file ;

	LMMPATH::reset_Output_SubFolder(foldername.str() );
	
	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_data_filename );

	//config.print("config_for_abcd_calibration.csv");
	//QuantLib::Array calibrated_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);

	marketData_LMM_Global_gCalibration(config, pLmmSwaptionMarketData,config.get_abcdArray(),create_InitCorrelation(config), GMatrixMapping_PTR() );

	//QuantLib::Array calibrated_abcd(4);
	//config.correl_alpha_= 1e-7 ; config.correl_beta_= 1;
	//calibrated_abcd[0]=0.223519;calibrated_abcd[1]=1.36E-05;calibrated_abcd[2]=0.455953;calibrated_abcd[3]=0.140501;
	//Correlation_PTR pCorrelation = marketData_LMM_Correlation_calibration(config,pLmmSwaptionMarketData, calibrated_abcd );

	//config.print("config_for_correlation_calibration.csv");
	//Correlation_PTR pCorrelation = marketData_LMM_Correlation_calibration(config,pLmmSwaptionMarketData, calibrated_abcd );
	//Correlation_PTR pCorrelation = create_InitCorrelation(config);
	//GMatrixMapping_PTR pGMatrixMapping = marketData_LMM_CascadeExact_calibration(config, pLmmSwaptionMarketData, config.get_abcdArray() ,pCorrelation);
	//marketData_LMM_Global_gCalibration(config, pLmmSwaptionMarketData,config.get_abcdArray(),pCorrelation, pGMatrixMapping );



	foldername.str( std::string() ) ;
}

void test_LmmSensitivityABCDReducedRank()
{
	std::vector<std::string> mkt_file_list = InputFileManager::get_VCUB_FileList();
	size_t nbFile = mkt_file_list.size();

	LmmCalibrationConfig config;
	config.reset_nbYear(16);
	//config.vol_abcd_.a_ = 0.148975	;
	//config.vol_abcd_.b_ = 0.0197503	;
	//config.vol_abcd_.c_ = 0.108518	;
	//config.vol_abcd_.d_ = 1.38E-08  ;


	std::vector<size_t> reduced_rank_list;
	reduced_rank_list.push_back(3);
	reduced_rank_list.push_back(5);
	reduced_rank_list.push_back(10);
	reduced_rank_list.push_back(15);
	reduced_rank_list.push_back(20);
	reduced_rank_list.push_back(25);
	reduced_rank_list.push_back(30);

	std::stringstream foldername ;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_file_list[0]);

	for(size_t i=0; i<reduced_rank_list.size();++i)
	{
		config.correl_ReducedRank_ = reduced_rank_list[i]  ;

		foldername <<"ABCDCalib_"<< LMMPATH::get_BaseFileName(mkt_file_list[0])<<"\\"<<"ReducedRank_"<<reduced_rank_list[i]<<"\\";
		LMMPATH::reset_Output_SubFolder(foldername.str() );

		QuantLib::Array calibrated_abcd = marketData_LMM_ABCD_calibration(config,pLmmSwaptionMarketData);

		foldername.str( std::string() ) ;
	}
}

void test_LmmSensitivityCorrelationReducedRank()
{
	std::vector<std::string> mkt_file_list = InputFileManager::get_VCUB_FileList();
	size_t nbFile = mkt_file_list.size();

	LmmCalibrationConfig config;
	config.reset_nbYear(16);
	config.vol_abcd_.a_ = 0.148975	;
	config.vol_abcd_.b_ = 0.0197503	;
	config.vol_abcd_.c_ = 0.108518	;
	config.vol_abcd_.d_ = 1.38E-08  ;


	std::vector<size_t> reduced_rank_list;
	reduced_rank_list.push_back(3);
	reduced_rank_list.push_back(5);
	reduced_rank_list.push_back(10);
	reduced_rank_list.push_back(15);
	reduced_rank_list.push_back(20);
	reduced_rank_list.push_back(25);
	reduced_rank_list.push_back(30);

	std::stringstream foldername ;

	LmmSwaptionMarketData_PTR pLmmSwaptionMarketData=get_LmmSwaptionMarketData(config, mkt_file_list[0]);

	for(size_t i=0; i<reduced_rank_list.size();++i)
	{
		config.correl_ReducedRank_ = reduced_rank_list[i]  ;

		foldername << LMMPATH::get_BaseFileName(mkt_file_list[0])<<"\\"<<"ReducedRank_"<<reduced_rank_list[i]<<"\\";
		LMMPATH::reset_Output_SubFolder(foldername.str() );

		Correlation_PTR pCorrelation = marketData_LMM_Correlation_calibration(config,pLmmSwaptionMarketData,config.get_abcdArray() );

		foldername.str( std::string() ) ;
	}
}


