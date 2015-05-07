#include <LMM/LmmModel/LmmGlobal_gCalibrator.h>

#include <LMM/LmmModel/LmmGlobal_gCostFunction.h>

#include <LMM/helper/GenericPath.h>

#include <fstream>
#include <cassert>
#include <string.h>
#include <ctime>


void LmmGlobal_gCalibrator::solve()
{
	QuantLib::LevenbergMarquardt minimizationSolver(functionEpsilon(), rootEpsilon(), 1e-16);

	QuantLib::Array start_point = gMatrixMappingInitiate_.get_DelegateArray();

	QuantLib::Problem optimizationProblem(*pLmmBaseCostFunction_.get(),  *pConstraint_.get(), start_point);  

	clock_t start_minimizer = std::clock();

	endConvergenceType_ = minimizationSolver.minimize( optimizationProblem, stopCriteria_ );	

	clock_t end_minimizer = std::clock();

	total_number_called_ = pLmmBaseCostFunction_->get_nbCalled();
	total_minimization_time_ = double(end_minimizer - start_minimizer) / CLOCKS_PER_SEC;
	infoStopSolver_ = minimizationSolver.getInfo();	
	
	retreive_calib_info();
}

void LmmGlobal_gCalibrator::retreive_calib_info()
{
	retreive_calib_global_error();

	boost::shared_ptr<LmmGlobal_gCostFunction> pLmmGlobal_gCostFunction = boost::static_pointer_cast<LmmGlobal_gCostFunction>(pLmmBaseCostFunction_);

	calibrated_gDelegate_matrix_ = pLmmGlobal_gCostFunction->get_GMatrixMapping()->get_gDelegate_Ref();

	const size_t swaption_matrix_size = calibrated_gDelegate_matrix_.size1();

	size_t nb_errors_counter=0;
	for(size_t iExperity=1;iExperity<swaption_matrix_size;++iExperity)
	{
		const size_t maxTenorBound =  swaption_matrix_size - iExperity;

		for(size_t jTenor=1;jTenor<maxTenorBound;++jTenor)
		{
			// bad calibrated param
			if(calibrated_gDelegate_matrix_(iExperity,jTenor) <1e-4
				//&& calibrated_gDelegate_matrix_(iExperity,jTenor) > -5000 // case truncated, values are setted  
				)
			{
				std::pair<size_t,size_t> bad_param_indices(iExperity,jTenor);
				bad_swaptions_indices_.push_back(bad_param_indices);
			}

			// Compute param error
			if(isVirtualCalibration_)
			{				
				const double relative_param_error = 100*(calibrated_gDelegate_matrix_(iExperity,jTenor) - true_gDelegate_matrix_(iExperity,jTenor))/true_gDelegate_matrix_(iExperity,jTenor);
				rel_error_gDelegate_matrix_(iExperity,jTenor) = relative_param_error;
				if(std::abs(relative_param_error) > max_param_rel_error_ ) max_param_rel_error_ = std::abs(relative_param_error);
			}
		}
	}

	// ---------------- PRINT GENERAL RESULT INFO OF CALBRATION  ---------------------------
	std::stringstream general_info;

	Shifted_HGVolatilityParam::ABCDParameter abcdParam = pLmmGlobal_gCostFunction->get_Shifted_HGVolatilityParam()->get_ABCD();
	general_info<<"a,b,c,d"<<std::endl;		
	general_info<<abcdParam.a_<<","<<abcdParam.b_<<","<<abcdParam.c_<<","<<abcdParam.d_<<","<<std::endl;
	
	// ------ print BAD SWAPTIONS
	if(	!bad_swaptions_indices_.empty() )
	{
		general_info<<"BAD Calibrated ,,";
		for(size_t i=0;i<bad_swaptions_indices_.size();++i)
		{
			general_info<<"("<<bad_swaptions_indices_[i].first<<";"<<bad_swaptions_indices_[i].second<<")  ,";
		}
		general_info<<std::endl;

		general_info<<"BAD calibrated Value ,,";
		for(size_t i=0;i<bad_swaptions_indices_.size();++i)
		{
			general_info<< calibrated_gDelegate_matrix_(bad_swaptions_indices_[i].first,bad_swaptions_indices_[i].second)<<" ,";
		}
	}
	general_info<<std::endl;

	general_info<<"Solver Stop "<<printStopCriteriaLevenbergMarquardt()<<std::endl;

	LmmPenalty_PTR pLmmPenalty = pLmmGlobal_gCostFunction->get_LmmPenalty_PTR() ;
	general_info<<"Calibrator Constraint ,"<<std::endl;
	general_info<<"Positivity ,"; if(use_positive_constraint_) general_info<<"YES ,"<<std::endl; else  general_info<<"NON ,"<<std::endl;
	
	// ------ print PENALTY INFORMATIONS
	general_info<<"Time_Homo Coeff    ," << pLmmPenalty->get_coeff_time_homogene()<<",,,";
	if(pLmmPenalty->get_coeff_time_homogene() > 0)
	{
		general_info<<"L2 Time_Homo    ," << get_PenaltyTimeHomogeneity_L2() <<",,";
		general_info<<"L1 Time_Homo    ," << get_PenaltyTimeHomogeneity_L1() <<",,";
		general_info<<"LINF Time_Homo  ," << get_PenaltyTimeHomogeneity_L_INF() <<",,"<<std::endl;
	}
	general_info<<std::endl;
	general_info<<"Libor_Smooth Coeff ," << pLmmPenalty->get_coeff_smooth_matutiry()<<",,,";
	if(pLmmPenalty->get_coeff_smooth_matutiry() > 0)
	{
		general_info<<"L2 Libor_Smooth   ," << get_PenaltySmoothMaturity_L2() <<",,";
		general_info<<"L1 Libor_Smooth   ," << get_PenaltySmoothMaturity_L1() <<",,";
		general_info<<"LINF Libor_Smooth ," << get_PenaltySmoothMaturity_L_INF() <<",,"<<std::endl;
	}
	general_info<<std::endl;

	if(isVirtualCalibration_)
	{
		general_info	<<max_param_rel_error_ <<", Max Param Error(%),"<<std::endl;
	}

	base_general_result_info_ += general_info.str();
}

void LmmGlobal_gCalibrator::printPlusPlus(const std::string& base_filename) const
{
	std::string path_OutPut = LMMPATH::get_output_path() + base_filename;

	std::ofstream final_result ;

	final_result.open(path_OutPut.c_str(), std::ios::app);

	final_result<<"==============  General Results  ==============="<<std::endl;

	final_result<<base_general_result_info_;

	final_result<<std::endl;
	
	final_result<<std::endl<<"==================== Quotation ERROR Matrix Results  ==================="<<std::endl;

	final_result<<std::endl<<"## absolute error , "<<std::endl;
	for(size_t i=1;i<abs_quote_error_matrix_.size1();++i)
	{
		for(size_t j=1;j<abs_quote_error_matrix_.size1()-i;++j)
		{
			final_result<<abs_quote_error_matrix_(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"## rel error (%), "<<std::endl;
	for(size_t i=1;i<rel_quote_error_matrix_.size1();++i)
	{
		for(size_t j=1;j<rel_quote_error_matrix_.size1()-i;++j)
		{
			final_result<<rel_quote_error_matrix_(i,j)<<",";
		}
		final_result<<std::endl;
	}

	///////  PRINT model quote and market quotes
	UpperTriangularDoubleMatrix mkt_black_vol_matrix = pLmmBaseCostFunction_->get_MarketQuoteValuesMatrix();
	UpperTriangularDoubleMatrix mdl_black_vol_matrix = pLmmBaseCostFunction_->get_ModelQuoteValuesMatrix();

	final_result<<std::endl<<"## Model Black Vol, "<<std::endl;
	for(size_t i=1;i<mdl_black_vol_matrix.size1();++i)
	{
		for(size_t j=1;j<mdl_black_vol_matrix.size1()-i;++j)
		{
			final_result<<mdl_black_vol_matrix(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"## Market Black Vol, "<<std::endl;
	for(size_t i=1;i<mkt_black_vol_matrix.size1();++i)
	{
		for(size_t j=1;j<mkt_black_vol_matrix.size1()-i;++j)
		{
			final_result<<mkt_black_vol_matrix(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"==================== Quotation ERROR Vector Results  ==================="<<std::endl;
	final_result<<"(Expir;Tenor) , , MKT Quotes, MDL Quotes, , Abs Error, Rel Error (%)"<<std::endl;


	for(size_t iExpirity = 1; iExpirity<mkt_black_vol_matrix.size1()-1; ++iExpirity) // row
	{
		size_t nb_jSwaption = mkt_black_vol_matrix.size2()- iExpirity;
		for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
		{
			final_result<<"("<<iExpirity<<";"<<jTenor<<"), ,";
			final_result<<mkt_black_vol_matrix(iExpirity,jTenor) <<","
				<<mdl_black_vol_matrix(iExpirity,jTenor) <<", ,"
				<<abs_quote_error_matrix_(iExpirity,jTenor) <<","
				<<rel_quote_error_matrix_(iExpirity,jTenor) <<",";

			if(std::abs(rel_quote_error_matrix_(iExpirity,jTenor))>30)
			{
				final_result<<", BAD BAD,"<<std::endl;
			}
			else if(std::abs(rel_quote_error_matrix_(iExpirity,jTenor))>10)
			{
				final_result<<", BAD,"<<std::endl;
			}
			else
			{
				final_result<<","<<std::endl;
			}			
		}
	}
	final_result<<std::endl
		<<", , , , Max Error ,"<< max_quote_abs_error_ <<","<< max_quote_rel_error_        <<","<<std::endl<<std::endl;

	if(isVirtualCalibration_)
	{
		final_result<<std::endl<<"==================== Param ERROR Vector Results for Virtual Test gDelegate ==================="<<std::endl;
		final_result<<"(Expir;Tenor) , , TrueParam, Calibrated Param, Error (%)"<<std::endl;


		for(size_t iExpirity = 1; iExpirity<mkt_black_vol_matrix.size1()-1; ++iExpirity) // row
		{
			size_t nb_jSwaption = mkt_black_vol_matrix.size2()- iExpirity;
			for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
			{
				final_result<<"("<<iExpirity<<";"<<jTenor<<"), ,";
				final_result<<true_gDelegate_matrix_(iExpirity,jTenor) <<","
					<<calibrated_gDelegate_matrix_(iExpirity,jTenor) <<", ,"
					<<rel_error_gDelegate_matrix_(iExpirity,jTenor) <<",";

				if(std::abs(rel_error_gDelegate_matrix_(iExpirity,jTenor))>15)
				{
					final_result<<", BAD BAD,"<<std::endl;
				}
				else if(std::abs(rel_error_gDelegate_matrix_(iExpirity,jTenor))>5)
				{
					final_result<<", BAD,"<<std::endl;
				}
				else
				{
					final_result<<","<<std::endl;
				}			
			}
		}
		final_result<<std::endl
			<<", , , ,Max Error(%) ,"<< max_param_rel_error_        <<","<<std::endl<<std::endl;
	}

	final_result.close();
}

double LmmGlobal_gCalibrator::get_PenaltyTimeHomogeneity_L2() const 
{
	boost::shared_ptr<LmmGlobal_gCostFunction> pLmmGlobal_gCostFunction = boost::static_pointer_cast<LmmGlobal_gCostFunction>(pLmmBaseCostFunction_);
	return pLmmGlobal_gCostFunction->calc_PenaltyTimeHomogeneity_L2() ;
}
double LmmGlobal_gCalibrator::get_PenaltyTimeHomogeneity_L1() const 
{
	boost::shared_ptr<LmmGlobal_gCostFunction> pLmmGlobal_gCostFunction = boost::static_pointer_cast<LmmGlobal_gCostFunction>(pLmmBaseCostFunction_);
	return pLmmGlobal_gCostFunction->calc_PenaltyTimeHomogeneity_L1() ;
}
double LmmGlobal_gCalibrator::get_PenaltyTimeHomogeneity_L_INF() const 
{
	boost::shared_ptr<LmmGlobal_gCostFunction> pLmmGlobal_gCostFunction = boost::static_pointer_cast<LmmGlobal_gCostFunction>(pLmmBaseCostFunction_);
	return pLmmGlobal_gCostFunction->calc_PenaltyTimeHomogeneity_L_INF() ;
}
double LmmGlobal_gCalibrator::get_PenaltySmoothMaturity_L2() const 
{
	boost::shared_ptr<LmmGlobal_gCostFunction> pLmmGlobal_gCostFunction = boost::static_pointer_cast<LmmGlobal_gCostFunction>(pLmmBaseCostFunction_);
	return pLmmGlobal_gCostFunction->calc_PenaltySmoothMaturity_L2() ;
}
double LmmGlobal_gCalibrator::get_PenaltySmoothMaturity_L1() const 
{
	boost::shared_ptr<LmmGlobal_gCostFunction> pLmmGlobal_gCostFunction = boost::static_pointer_cast<LmmGlobal_gCostFunction>(pLmmBaseCostFunction_);
	return pLmmGlobal_gCostFunction->calc_PenaltySmoothMaturity_L1() ;
}
double LmmGlobal_gCalibrator::get_PenaltySmoothMaturity_L_INF() const 
{
	boost::shared_ptr<LmmGlobal_gCostFunction> pLmmGlobal_gCostFunction = boost::static_pointer_cast<LmmGlobal_gCostFunction>(pLmmBaseCostFunction_);
	return pLmmGlobal_gCostFunction->calc_PenaltySmoothMaturity_L_INF() ;
}

std::string LmmGlobal_gCalibrator::printStopCriteriaLevenbergMarquardt() const 
{
	//http://www.math.utah.edu/software/minpack/minpack/lmdif.html see at the end

	std::string LevenbergMarquardtSolverInfo;

	if(infoStopSolver_ == 0)
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 0.Improper input parameters" ;
	else if(infoStopSolver_ == 1)
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 1.Both actual and predicted relative reductions in the sum of squares are at most FTOL" ;
	else if(infoStopSolver_ == 2)
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 2.Relative error between two consecutive iterates is at most XTOL" ;
	else if(infoStopSolver_ == 3)
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 3.Conditions for INFO = 1 and INFO = 2 both hold." ;
	else if(infoStopSolver_ == 4)
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 4.The cosine of the angle between FVEC and any column of the Jacobian is at most GTOL in absolute value" ;
	else if(infoStopSolver_ == 5)
		std::cout<<" LevenbergMarquardtSolver 5.Number of calls to FCN has reached or exceeded MAXFEV" ;
	else if(infoStopSolver_ == 5)
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 6.FTOL is too small. No further reduction in the sum of squares is possible" ;
	else if(infoStopSolver_ == 7)
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 7.XTOL is too small. No further improvement in the approximate solution X is possible." ;
	else if(infoStopSolver_ == 8)
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver 8.GTOL is too small. FVEC is orthogonal to the	columns of the Jacobian to machine precision" ;
	else
		LevenbergMarquardtSolverInfo = " LevenbergMarquardtSolver NOT DETERMINE INFO" ;

	return LevenbergMarquardtSolverInfo ;
}

