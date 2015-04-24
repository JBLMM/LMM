#pragma once
#include <LMM/LmmModel/ConstShifted_HGVolatilityFunction.h>

// LmmCalibrationConfig play the role of parameters holders for the whole model
//
// 
struct LmmCalibrationConfig 
{	
public:
	
	LmmCalibrationConfig();	
	
	Shifted_HGVolatilityParam::ABCDParameter vol_abcd_;

	size_t model_nbYear_; 

	size_t correl_FullRank_;
	size_t correl_ReducedRank_;

	double correl_alpha_;
	double correl_beta_;
	
	double penalty_libor_;
	double penalty_time_homogeneity_;

	std::string test_folder_;

	mutable bool use_local_calib_;
	bool use_positive_constraint_;

	virtual void print(const std::string& filename) const ;	

	void reset_nbYear(size_t nbYear)
	{
		model_nbYear_=nbYear;
		correl_FullRank_ = 2*model_nbYear_+1;
	}

	QuantLib::Array get_abcdArray() const
	{
		QuantLib::Array abcd(4);
		abcd[0]=vol_abcd_.a_;
		abcd[1]=vol_abcd_.b_;
		abcd[2]=vol_abcd_.c_;
		abcd[3]=vol_abcd_.d_;
		return abcd;
	}

	// storing result
	void reset_result() const ;

	mutable double result_quote_error_l2;
	mutable double result_quote_error_l1;
	mutable double result_quote_error_linf;

	mutable double result_pelTime_error_l2;
	mutable double result_pelTime_error_l1;
	mutable double result_pelTime_error_linf;

	mutable double result_pelLibor_error_l2;
	mutable double result_pelLibor_error_l1;
	mutable double result_pelLibor_error_linf;
};

