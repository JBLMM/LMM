#include <LMM/LmmModel/LmmABCDCalibrator.h>

#include <LMM/helper/GenericPath.h>
#include <LMM/LmmModel/LmmABCDCostFunction.h>
#include <ql/termstructures/volatility/abcd.hpp>

#include <fstream>
#include <cassert>
#include <string.h>
#include <ctime>

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
///  TEST TO CREATE A SPECIFIC ABCD CONSTRAINT  //////

// constraint retaining d around a value predefined


//! %Constraint imposing i-th argument to be in [low_i,high_i] for all i
class ABCDConstraint : public QuantLib::Constraint 
{
private:
	class Impl: public QuantLib::Constraint::Impl 
	{
	public:
		Impl(const double& mean_d, const double& margin /*percentage*/ ) 
		: low_ (4 , 0.)
		, high_(4 , std::numeric_limits< double >::max() ) 
		{
			assert(mean_d>0.)  ;	assert(margin>0. && margin<1.);
			// see abcd condition at
			// https://github.com/lballabio/quantlib/blob/master/QuantLib/ql/termstructures/volatility/abcd.hpp

			const double d_pp = mean_d * ( 1. + margin);
			const double d_mm = mean_d * ( 1. - margin);

			low_[0] = -d_mm ; // a+d > 0
			low_[1] = std::numeric_limits< double >::lowest() ; // b can be negative
			low_[2] = 0. ;    // c>0	
			low_[3] = d_mm ;
			high_[3] = d_pp;

			for (Size i = 0; i < 4; i++) 
			{
				assert( low_[i] < high_[i] ) ; 
				std::cout<< i << "  Low_"<<low_[i] <<"    high_"<<high_[i]<<std::endl;//ctndebug to comment
			}
		}

		bool test(const Array& params) const 
		{
			QL_ENSURE(params.size()==4, " Number of abcd parameters has to be 4")

				for (Size i = 0; i < params.size(); i++) 
				{
					if ((params[i] < low_[i]) || (params[i] > high_[i])) return false;
				}

				if (params[0] + params[3] <0.) return false ; // a+d==0

				return true;
		}

		QuantLib::Array upperBound(const Array&) const { return high_; }
		QuantLib::Array lowerBound(const Array&) const { return low_ ; }
	private:
		QuantLib::Array low_, high_;
	};

public:
	ABCDConstraint(const double& mean_d, const double& margin /*percentage*/ )
	: QuantLib::Constraint(boost::shared_ptr <QuantLib::Constraint::Impl>( new ABCDConstraint::Impl(mean_d, margin)) ) {}
};


void LmmABCDCalibrator::solve()
{
	QuantLib::LevenbergMarquardt minimizationSolver(functionEpsilon(), rootEpsilon(), 1e-16);

	QuantLib::Problem optimizationProblem(*pLmmBaseCostFunction_.get(),  *pConstraint_.get(), init_abcd_);  

	clock_t start_minimizer = std::clock();

	endConvergenceType_ = minimizationSolver.minimize( optimizationProblem, stopCriteria_ );	

	clock_t end_minimizer = std::clock();

	found_abcd_ = optimizationProblem.currentValue();

	total_number_called_ = pLmmBaseCostFunction_->get_nbCalled();
	total_minimization_time_ = double(end_minimizer - start_minimizer) / CLOCKS_PER_SEC;
	infoStopSolver_ = minimizationSolver.getInfo();	
	
	retreive_calib_info();
}

void LmmABCDCalibrator::retreive_calib_info()
{
	retreive_calib_global_error();
	
	std::stringstream general_info;

	general_info<<"=============== ABCD Init =============="<<std::endl;
	general_info<<"a,b,c,d,,"<<std::endl;
	general_info<<init_abcd_[0]<<","<<init_abcd_[1]<<","<<init_abcd_[2]<<","<<init_abcd_[3]<<",,"<<std::endl;
	general_info<<"=============== ABCD Found =============="<<std::endl;
	general_info<<"a,b,c,d,,"<<std::endl;
	general_info<<found_abcd_[0]<<","<<found_abcd_[1]<<","<<found_abcd_[2]<<","<<found_abcd_[3]<<",,"<<std::endl;

	
	LMMTenorStructure_PTR pLMMTenorStructure = pLmmBaseCostFunction_->get_LMMTenorStructure_PTR() ;
	
	size_t N = pLMMTenorStructure->get_horizon();
	general_info<<"tau,";
	for(size_t i=0;i<N;++i){ general_info<<pLMMTenorStructure->get_tenorDate(i) <<","; } general_info<<std::endl;

	QuantLib::AbcdFunction  abcdFunction(found_abcd_[0],found_abcd_[1],found_abcd_[2],found_abcd_[3]); 
	general_info<<"Func(tau),";
	for(size_t i=0;i<N;++i)
	{
		const double tau = pLMMTenorStructure->get_tenorDate(i) ;
		double abcd_function = abcdFunction(tau); 
		general_info<< abcd_function <<","; 
	} general_info<<std::endl<<std::endl;

	general_info<<"Used Swaptions,,";
	if( get_ConstraintCell().empty() )
	{
		general_info<<"Whole Swaption Matrix,";
	}
	else
	{
		assert( get_ConstraintCell().size() >3 );
		for(size_t iCell=0;iCell<get_ConstraintCell().size();++iCell)
		{
			general_info<<"("<<get_ConstraintCell()[iCell].first<<";"<<get_ConstraintCell()[iCell].second<<"),";
		}
	}

	general_info<<std::endl;
	
	base_general_result_info_ += general_info.str();
}

void LmmABCDCalibrator::printPlusPlus(const std::string& base_filename) const
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

	final_result<<std::endl;
	final_result.close();
}

void LmmABCDCalibrator::reset_d_constraint(const double& meand, const double & margin)
{
	use_positive_constraint_= false ;
	pConstraint_.reset(new ABCDConstraint(meand,margin) );
}