#include <LMM/LmmModel/LmmCorrelationCalibrator.h>

#include <LMM/helper/GenericPath.h>
#include <ql/termstructures/volatility/abcd.hpp>

#include <fstream>
#include <cassert>
#include <string.h>
#include <ctime>


void LmmCorrelationCalibrator::solve()
{
	QuantLib::LevenbergMarquardt minimizationSolver(functionEpsilon(), rootEpsilon(), 1e-16);

	this->activate_PositiveConstraint(); // alpha, beta of correlation has to be positives.

	QuantLib::Problem optimizationProblem(*pLmmBaseCostFunction_.get(),  *pConstraint_.get(), init_correl_param_);  

	clock_t start_minimizer = std::clock();

	endConvergenceType_ = minimizationSolver.minimize( optimizationProblem, stopCriteria_ );	

	clock_t end_minimizer = std::clock();

	found_correl_param_ = optimizationProblem.currentValue();

	total_number_called_ = pLmmBaseCostFunction_->get_nbCalled();
	total_minimization_time_ = double(end_minimizer - start_minimizer) / CLOCKS_PER_SEC;
	infoStopSolver_ = minimizationSolver.getInfo();	
	
	retreive_calib_info();
}

void LmmCorrelationCalibrator::retreive_calib_info()  // YY TODO: change name "retrieve calib info"
{
	retreive_calib_global_error();

	/// print general result info
	std::stringstream general_info;
	general_info<<"Solver Stop "<<printStopCriteriaLevenbergMarquardt()<<std::endl;

	general_info<<"----- Initial Correlation Parameters  ------ "<<std::endl;
	for(size_t i=0;i<init_correl_param_.size();++i)
	{
		general_info<<init_correl_param_[i]<<",";
	}
	general_info<<std::endl;

	general_info<<"----- Found Correlation Parameters  ------ "<<std::endl;
	for(size_t i=0;i<found_correl_param_.size();++i)
	{
		general_info<<found_correl_param_[i]<<",";
	}
	general_info<<std::endl;

	boost::shared_ptr<LmmCorrelationCostFunction> pLmmCorrelationCostFunction = boost::static_pointer_cast<LmmCorrelationCostFunction>(pLmmBaseCostFunction_);
	
	general_info << "fullRank_,"<<pLmmCorrelationCostFunction->get_Correlation()->get_fullRank() <<","<<std::endl;
	general_info << "reducedRank_,"<<pLmmCorrelationCostFunction->get_Correlation()->get_reducedRank()<<","<<std::endl;
	general_info << "reductionType_,"<<pLmmCorrelationCostFunction->get_Correlation()->nameCorrelationReductionType()<<","<<std::endl;
	general_info <<std::endl<<std::endl;

	base_general_result_info_ += general_info.str();
}

void LmmCorrelationCalibrator::printPlusPlus(const std::string& base_filename) const
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

	final_result<<std::endl;
	final_result.close();
}

std::string LmmCorrelationCalibrator::printStopCriteriaLevenbergMarquardt() const 
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