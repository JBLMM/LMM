#include <LMM/LmmModel/LmmShiftCalibrator.h>

#include <LMM/helper/GenericPath.h>

#include <fstream>
#include <cassert>
#include <string.h>
#include <ctime>


void LmmShiftCalibrator::solve()
{
	QuantLib::PositiveConstraint constraint ;

	QuantLib::Problem optimizationProblem_(costFunction_, constraint, init_shift_param_); 

	QuantLib::LevenbergMarquardt minimizationSolver_(functionEpsilon(), rootEpsilon(), 1e-16);

	clock_t start_minimizer = std::clock();

	endConvergenceType_ = minimizationSolver_.minimize( optimizationProblem_, stopCriteria_ );	

	clock_t end_minimizer = std::clock();

	minimization_time_ = double(end_minimizer - start_minimizer) / CLOCKS_PER_SEC;
	infoStopSolver_ = minimizationSolver_.getInfo();	
	nbFunctionEvaluation_ = optimizationProblem_.functionEvaluation() ;  
	nbGradiantEvaluation_ = optimizationProblem_.gradientEvaluation() ;	
}

void LmmShiftCalibrator::printPlusPlus(const std::string& base_filename) const
{
	std::string path_OutPut = LMMPATH::get_output_path() + base_filename;

	std::ofstream final_result ;

	final_result.open(path_OutPut.c_str(), std::ios::app);

	UpperTriangularDoubleMatrix mkt_quotes_matrix = dynamic_cast<LmmSkewCostFunction&>(costFunction_).get_MarketQuotes();
	UpperTriangularDoubleMatrix mdl_quotes_matrix = dynamic_cast<LmmSkewCostFunction&>(costFunction_).get_CurrentModelQuotes();

	assert(mkt_quotes_matrix.size1() == mdl_quotes_matrix.size1() );
	assert(mkt_quotes_matrix.size2() == mdl_quotes_matrix.size2() );

	final_result<<std::endl<<std::endl<<"==================== LmmShiftCalibrator Results  ==================="<<std::endl;
	final_result<<"Convergence,"<<endConvergenceType_<<std::endl
		<<nbFunctionEvaluation_ <<", Iterations,"<<std::endl
		<<nbGradiantEvaluation_ <<", Grad Calc,"<<std::endl
		<< minimization_time_ <<" s, Time,"<<std::endl;

	final_result<<std::endl;
	printStopCriteriaLevenbergMarquardt(final_result);

	final_result<<std::endl<<"==================== Vector Results  ==================="<<std::endl;
	final_result<<"(Expir;Tenor) , , MKT Skew, MDL  Skew, , Error (%)"<<std::endl;

	double rel_linf_diff=0;//relative error
	for(size_t iExpirity = 1; iExpirity<mkt_quotes_matrix.size1()-1; ++iExpirity) // row
	{
		size_t nb_jSwaption = mkt_quotes_matrix.size2()- iExpirity;
		for(size_t jTenor = 1;jTenor< nb_jSwaption; ++jTenor)
		{
			//relative error
			const double rel_diff =100* (mkt_quotes_matrix(iExpirity,jTenor) - mdl_quotes_matrix(iExpirity,jTenor) ) / std::abs(mkt_quotes_matrix(iExpirity,jTenor) );

			final_result<<"("<<iExpirity<<";"<<jTenor<<"), ,";
			final_result<<mkt_quotes_matrix(iExpirity,jTenor) <<","
				<<mdl_quotes_matrix(iExpirity,jTenor) <<", ,"
				<<rel_diff                            <<",";

			if(std::abs(rel_diff)>30)
			{
				final_result<<", BAD BAD,"<<std::endl;
			}
			else if(std::abs(rel_diff)>10)
			{
				final_result<<", BAD,"<<std::endl;
			}
			else
			{
				final_result<<","<<std::endl;
			}

			if( rel_linf_diff < std::abs(rel_diff) ) rel_linf_diff = std::abs(rel_diff);
		}
	}
	final_result<<std::endl
		<<", , , ,Max Error(%) ,"<< rel_linf_diff        <<","<<std::endl<<std::endl;

	final_result<<std::endl<<"==================== Matrix Results  ==================="<<std::endl;
	UpperTriangularDoubleMatrix diff_matrix = mkt_quotes_matrix - mdl_quotes_matrix;

	final_result<<std::endl<<"## diff MKT-MDL Skew, "<<std::endl;
	for(size_t i=0;i<diff_matrix.size1();++i)
	{
		for(size_t j=0;j<diff_matrix.size1()-i;++j)
		{
			final_result<<diff_matrix(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"## Market  Skew, "<<std::endl;
	for(size_t i=0;i<mkt_quotes_matrix.size1();++i)
	{
		for(size_t j=0;j<mkt_quotes_matrix.size1()-i;++j)
		{
			final_result<<mkt_quotes_matrix(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl<<"## Model  Skew, "<<std::endl;
	for(size_t i=0;i<mdl_quotes_matrix.size1();++i)
	{
		for(size_t j=0;j<mdl_quotes_matrix.size1()-i;++j)
		{
			final_result<<mdl_quotes_matrix(i,j)<<",";
		}
		final_result<<std::endl;
	}

	final_result<<std::endl;
	final_result.close();
}


void LmmShiftCalibrator::printStopCriteriaLevenbergMarquardt( std::ofstream & stream ) const 
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

	stream << LevenbergMarquardtSolverInfo <<"," <<std::endl;
}