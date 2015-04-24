#include <LMM/calibration/LmmCalibrationConfig.h>
#include <LMM/helper/GenericPath.h>

#include <iostream>
#include <cassert>

LmmCalibrationConfig::LmmCalibrationConfig()
	: model_nbYear_(16)
	, vol_abcd_(0.17,0.37,1.12,0.3)  //// a very "normal" curve in Rebonato.2009 p.13
	//, vol_abcd_(0.175226,0.567055,1.15307,0.220205) //// QL propose 01.Aug.2014 with column 5
	, correl_ReducedRank_(3)
	, correl_alpha_(0.2)
	, correl_beta_(0.5)
	, penalty_libor_(0.)
	, penalty_time_homogeneity_(0.)
	, test_folder_("calibration")
	, use_local_calib_(false)
	, use_positive_constraint_(false)
{
	reset_result();

	correl_FullRank_ = 2*model_nbYear_ + 1; // case fixed/float = 2 typically lmm 6MO/1YR

	assert(correl_ReducedRank_ <= correl_FullRank_);
}

void LmmCalibrationConfig::print(const std::string& filename) const 
{
	std::string path_OutPut = LMMPATH::get_output_path() + filename;

	std::ofstream outputstream;
	outputstream.open(path_OutPut.c_str());

	outputstream<<"LmmCalibrationConfig parameters holder"<<std::endl;

	outputstream<<"model_nbYear_ , "<<model_nbYear_<<std::endl; 
	outputstream<<"a,b,c,d, "<<std::endl; 
	outputstream<<vol_abcd_.a_<<","<<vol_abcd_.b_<<","<<vol_abcd_.c_<<","<<vol_abcd_.d_<<","<<std::endl; 

	outputstream<<"correl_FullRank_ , "    << correl_FullRank_    <<std::endl; 
	outputstream<<"correl_ReducedRank_ , " << correl_ReducedRank_ <<std::endl; 
	outputstream<<"correl_alpha_ , "       << correl_alpha_       <<std::endl; 
	outputstream<<"correl_beta_ , "        << correl_beta_        <<std::endl; 

	if(use_local_calib_) outputstream<<"Use Local Calibraion , " <<std::endl; 
	
	outputstream<<"Calibration Constraint ,"<<std::endl;
	outputstream<<"Positive Constraint ,,";
	if(use_positive_constraint_) outputstream<<"YES , " <<std::endl; else  outputstream<<"NON , " <<std::endl; 
	outputstream<<"Penalty Libor ,," << penalty_libor_ <<","<<std::endl; 
	outputstream<<"Penalty Time Homogeneity ,," << penalty_time_homogeneity_ <<","<<std::endl; 
	
	outputstream.close();
}

void LmmCalibrationConfig::reset_result() const 
{
	/// errors always has to be positive or null after computing
	/// initiate errors to small negative, if result keep negative, that is a problem when calculating error

	result_quote_error_l2      =-1e-9;
	result_quote_error_l1      =-1e-9;
	result_quote_error_linf    =-1e-9;

	result_pelTime_error_l2    =-1e-9;
	result_pelTime_error_l1    =-1e-9;
	result_pelTime_error_linf  =-1e-9;

	result_pelLibor_error_l2   =-1e-9;
	result_pelLibor_error_l1   =-1e-9;
	result_pelLibor_error_linf =-1e-9;
}