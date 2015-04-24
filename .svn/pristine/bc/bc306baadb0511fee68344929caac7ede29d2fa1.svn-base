#pragma once

#include <boost/shared_ptr.hpp>

#include <ql/math/optimization/costfunction.hpp>
#include <ql/math/optimization/endcriteria.hpp>
#include <ql/math/optimization/constraint.hpp>
#include <ql/math/optimization/problem.hpp>
#include <ql/math/optimization/simplex.hpp>
#include <ql/math/optimization/bfgs.hpp> 
#include <ql/math/optimization/conjugategradient.hpp>
#include <ql/math/optimization/levenbergmarquardt.hpp>

#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>

#include <LMM/LmmModel/LmmBaseCostFunction.h>

class LmmBaseCalibrator
{
public :

	//! ctor
	LmmBaseCalibrator
		( const QuantLib::Size & maxIterations       
		, const QuantLib::Real & rootEpsilon        
		, const QuantLib::Real & functionEpsilon    
		, LmmBaseCostFunction_PTR & pLmmBaseCostFunction			
		);

	virtual std::string Name() const =0 ;

	bool isVirtualCalibration() const { return isVirtualCalibration_; }

	virtual void solve() = 0 ;

	virtual void printPlusPlus(const std::string& base_filename) const = 0 ;

	//! activate the positive constraint to be sure that the cellulle calibration do not have negative solution
	void activate_PositiveConstraint() ;

	//! method retreiving information from QuantLib::EndCriteria
	QuantLib::Size maxIterations()                const {return stopCriteria_.maxIterations() ;}
	QuantLib::Size maxStationaryStateIterations() const {return stopCriteria_.maxStationaryStateIterations() ;}
	QuantLib::Real rootEpsilon()                  const {return stopCriteria_.rootEpsilon() ;}
	QuantLib::Real functionEpsilon()              const {return stopCriteria_.functionEpsilon() ;}
	QuantLib::Real gradientNormEpsilon()          const {return stopCriteria_.gradientNormEpsilon() ;}
	const QuantLib::EndCriteria::Type & get_EndConvergenceType() const { return endConvergenceType_ ;}
		
	const double& get_QuoteError_L2() const { return quote_error_l2_; }
	const double& get_QuoteError_L1() const { return quote_error_l1_; }
	const double& get_QuoteError_LInf() const { return quote_error_lInf_; }


	const std::string& get_BaseGeneral_Result_Info() const { return base_general_result_info_;}

protected :

	bool isVirtualCalibration_;

	// post calibration calculated errors
	void retreive_calib_global_error() ;
	virtual void retreive_calib_info() = 0 ;

	std::string base_general_result_info_;

	//! minimizator
	QuantLib::EndCriteria stopCriteria_;   // ( maxIterations ,minStatIterations ,rootEpsilon ,functionEpsilon ,gradientNormEpsilon );
	QuantLib::EndCriteria::Type endConvergenceType_;

	bool use_positive_constraint_;
	boost::shared_ptr<Constraint> pConstraint_;
	LmmBaseCostFunction_PTR pLmmBaseCostFunction_ ;

	//! storage information after minimization
	size_t total_number_called_;
	double total_minimization_time_;
	
	// stored relative error for post calibration
	double max_quote_rel_error_;
	std::pair<size_t,size_t> max_quote_rel_error_position_;
	UpperTriangularDoubleMatrix rel_quote_error_matrix_;// error in (%)
	
	//// stored error for post calibration
	double max_quote_abs_error_;
	std::pair<size_t,size_t> max_quote_abs_error_position_;
	UpperTriangularDoubleMatrix abs_quote_error_matrix_;// absolute error 

	double quote_error_l2_, quote_error_l1_, quote_error_lInf_;

	void printAnnexeStopCriteriaLevenbergMarquardt( std::ofstream & stream ) const ;
};

typedef boost::shared_ptr<LmmBaseCalibrator> LmmBaseCalibrator_PTR;