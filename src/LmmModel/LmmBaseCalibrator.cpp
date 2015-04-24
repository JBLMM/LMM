#include <LMM/LmmModel/LmmBaseCalibrator.h>

#include <LMM/helper/GenericPath.h>
#include <LMM/LmmModel/LmmBaseCostFunction.h>

#include <fstream>
#include <cassert>
#include <string.h>
#include <ctime>


LmmBaseCalibrator::LmmBaseCalibrator
	( const QuantLib::Size & maxIterations       
	, const QuantLib::Real & rootEpsilon        
	, const QuantLib::Real & functionEpsilon    
	, LmmBaseCostFunction_PTR & pLmmBaseCostFunction			
	)		
	: isVirtualCalibration_(false)
	, use_positive_constraint_(false)
	, pConstraint_(new QuantLib::NoConstraint() ) // default constraint is a no constraint
	, stopCriteria_(maxIterations, 100 , rootEpsilon, functionEpsilon, 0.)
	, pLmmBaseCostFunction_(pLmmBaseCostFunction)
	, total_minimization_time_(-1000)
	, max_quote_rel_error_(0)
	, rel_quote_error_matrix_( pLmmBaseCostFunction->size1(), pLmmBaseCostFunction->size2() )
	, max_quote_abs_error_(0)
	, abs_quote_error_matrix_( pLmmBaseCostFunction->size1(), pLmmBaseCostFunction->size2() )
	, quote_error_l2_(0.), quote_error_l1_(0.), quote_error_lInf_(0.)

{

}


void LmmBaseCalibrator::activate_PositiveConstraint()
{
	use_positive_constraint_ = true;
	pConstraint_.reset( new QuantLib::PositiveConstraint() );
}


void LmmBaseCalibrator::retreive_calib_global_error() 
{
	pLmmBaseCostFunction_->fully_UpdateSwaptionMdlValues();

	const UpperTriangularDoubleMatrix mkt_quotes_matrix = pLmmBaseCostFunction_->get_MarketQuoteValuesMatrix();
	const UpperTriangularDoubleMatrix mdl_quotes_matrix = pLmmBaseCostFunction_->get_ModelQuoteValuesMatrix();

	assert(mkt_quotes_matrix.size1() == mkt_quotes_matrix.size2() );
	assert(mkt_quotes_matrix.size1() == mdl_quotes_matrix.size1() );
	assert(mkt_quotes_matrix.size2() == mdl_quotes_matrix.size2() );

	const size_t swaption_matrix_size = mkt_quotes_matrix.size1();

	size_t nb_errors_counter=0;
	for(size_t iExperity=1;iExperity<swaption_matrix_size;++iExperity)
	{
		const size_t maxTenorBound =  swaption_matrix_size - iExperity;

		for(size_t jTenor=1;jTenor<maxTenorBound;++jTenor)
		{
			// Compute quote error			
			const double absolute_quote_error = mdl_quotes_matrix(iExperity,jTenor) - mkt_quotes_matrix(iExperity,jTenor);
			abs_quote_error_matrix_(iExperity,jTenor) = absolute_quote_error;
			const double relative_quote_error = 100*(absolute_quote_error)/mkt_quotes_matrix(iExperity,jTenor);
			rel_quote_error_matrix_(iExperity,jTenor) = relative_quote_error;

			quote_error_l2_ += (abs_quote_error_matrix_(iExperity,jTenor) * abs_quote_error_matrix_(iExperity,jTenor)  );
			quote_error_l1_ += std::abs(abs_quote_error_matrix_(iExperity,jTenor) );
			++nb_errors_counter;

			if(std::abs(relative_quote_error) > max_quote_rel_error_ )
			{
				max_quote_rel_error_ = std::abs(relative_quote_error);
				max_quote_rel_error_position_.first=iExperity;
				max_quote_rel_error_position_.second=jTenor;
			}

			if(std::abs(absolute_quote_error) > max_quote_abs_error_ )
			{
				max_quote_abs_error_ = std::abs(absolute_quote_error);
				max_quote_abs_error_position_.first=iExperity;
				max_quote_abs_error_position_.second=jTenor;
				quote_error_lInf_ = max_quote_abs_error_;
			}
		}
	}

	quote_error_l2_ /= (double) nb_errors_counter ; quote_error_l2_ = std::sqrt(quote_error_l2_) ; 
	quote_error_l1_ /= (double) nb_errors_counter ;

	std::stringstream general_info;

	general_info << this->Name() << ",, DataFile ,," << pLmmBaseCostFunction_->get_UpperTriangleVanillaSwaptionQuotes()->get_Data_FileName() <<std::endl;;

	if(use_positive_constraint_) 
		general_info<<"Use Positive Constraint "<<std::endl; 
	else	
		general_info<<"NOT Positive Constraint, "<<std::endl; 

		// ------ print mininmizator end information
	general_info<<"Convergence,"<<endConvergenceType_<<std::endl
				<< total_number_called_ <<" ,called ,"<<std::endl
		        << total_minimization_time_ <<" s, Time,"<<std::endl;

	general_info<<"L2 Global Error ,"<<quote_error_l2_<<","<<std::endl;
	general_info<<"L1 Global Error ,"<<quote_error_l1_<<","<<std::endl;
	general_info<<"LInf Global Error ,"<<quote_error_lInf_<<","<<std::endl;

	general_info<< max_quote_rel_error_ <<", Max Quote Relative Error(%)," << "("<<max_quote_rel_error_position_.first<< ";"<<max_quote_rel_error_position_.second<<")" <<  std::endl
		        << max_quote_abs_error_ <<", Max Quote Absolute Error," << "("<<max_quote_abs_error_position_.first<< ";"<<max_quote_abs_error_position_.second<<")" <<  std::endl;

	base_general_result_info_.clear();
	base_general_result_info_=general_info.str();
}



void LmmBaseCalibrator::printAnnexeStopCriteriaLevenbergMarquardt( std::ofstream & stream ) const
{
	//http://www.math.utah.edu/software/minpack/minpack/lmdif.html see at the end
	stream <<std::endl;
	stream << " LevenbergMarquardt stop criteria " <<std::endl;
	stream << " See http://www.math.utah.edu/software/minpack/minpack/lmdif.html "<<std::endl;
	stream << " 0. Improper input parameters" <<std::endl;
	stream << " 1. Both actual and predicted relative reductions in the sum of squares are at most FTOL" <<std::endl;
	stream << " 2. Relative error between two consecutive iterates is at most XTOL" <<std::endl;
	stream << " 3. Conditions for INFO = 1 and INFO = 2 both hold." <<std::endl;
	stream << " 4. The cosine of the angle between FVEC and any column of the Jacobian is at most GTOL in absolute value" <<std::endl;
	stream << " 5. Number of calls to FCN has reached or exceeded MAXFEV" <<std::endl;
	stream << " 6. FTOL is too small. No further reduction in the sum of squares is possible" <<std::endl;
	stream << " 7. XTOL is too small. No further improvement in the approximate solution X is possible." <<std::endl;
	stream << " 8. GTOL is too small. FVEC is orthogonal to the	columns of the Jacobian to machine precision" <<std::endl;
	stream << " 9. NOT DETERMINE INFO" <<std::endl;
}