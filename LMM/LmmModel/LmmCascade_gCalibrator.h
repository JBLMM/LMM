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
#include <LMM/LmmModel/GMatrixMapping.h>

#include <LMM/LmmModel/LmmBaseCalibrator.h>
#include <LMM/LmmModel/LmmBaseCostFunction.h>

class LmmCascade_gCalibrator : public LmmBaseCalibrator
{
public :

	//! ctor
	LmmCascade_gCalibrator
		(
		  const GMatrixMapping & gMatrixMappingInitiate
		, const QuantLib::Size & maxIterations       
		, const QuantLib::Real & rootEpsilon        
		, const QuantLib::Real & functionEpsilon    
		, LmmBaseCostFunction_PTR & pLmmBaseCostFunction		
		)
		: LmmBaseCalibrator(maxIterations, rootEpsilon , functionEpsilon, pLmmBaseCostFunction )
		, gMatrixMappingInitiate_(gMatrixMappingInitiate)
		, max_param_rel_error_(1000000000)
		, calibrated_gDelegate_matrix_( gMatrixMappingInitiate_.get_gDelegate_Ref().size1(), gMatrixMappingInitiate_.get_gDelegate_Ref().size2() )
		, true_gDelegate_matrix_(  gMatrixMappingInitiate_.get_gDelegate_Ref().size1(), gMatrixMappingInitiate_.get_gDelegate_Ref().size2() )
		, rel_error_gDelegate_matrix_( gMatrixMappingInitiate_.get_gDelegate_Ref().size1(), gMatrixMappingInitiate_.get_gDelegate_Ref().size2() )
		, infoStopSolverMatrix_( gMatrixMappingInitiate_.get_gDelegate_Ref().size1(), gMatrixMappingInitiate_.get_gDelegate_Ref().size2() )
	{
		
	}

	virtual std::string Name() const 
	{
		if(isVirtualCalibration_) return "VIRTUAL Cascade gCalibrator";
		else  return "Cascade gCalibrator";
	}

	void set_VirtualTestTrueParam(const UpperTriangularDoubleMatrix& true_gDelegate)
	{
		assert(true_gDelegate.size1() == true_gDelegate_matrix_.size1() && true_gDelegate.size2() == true_gDelegate_matrix_.size2() );//parameters is the gDelegate matrix
		isVirtualCalibration_ = true;
		true_gDelegate_matrix_ = true_gDelegate;
		max_param_rel_error_ = 0;
	}

	virtual void solve();

	//! print plus plus without erease
	void printPlusPlus(const std::string& base_filename) const ;
		
	const std::vector<std::pair<size_t,size_t> > & get_BadSwaption_Indices() const { return bad_swaptions_indices_;}

protected :

	virtual void retreive_calib_info();

private:

	const GMatrixMapping gMatrixMappingInitiate_;	

	std::vector<std::pair<size_t,size_t> > bad_swaptions_indices_;//stored indices of very small or negative calibrated gDelegate
	std::vector< double > bad_swaption_prices_;
	std::vector< double > bad_swaption_errors_;
	std::vector< double > bad_gDelegate_found_;

	double max_param_rel_error_;
	UpperTriangularDoubleMatrix true_gDelegate_matrix_;// known true parameter in virtual test
	UpperTriangularDoubleMatrix calibrated_gDelegate_matrix_;
	UpperTriangularDoubleMatrix rel_error_gDelegate_matrix_; // error in (%)

	boost::numeric::ublas::matrix<QuantLib::Integer> infoStopSolverMatrix_;	

		// call this just for debugging in a specific case iExperity, jTenor
	void test_ctndebug_costFunction(size_t iExperity, size_t  jTenor, const double minX, const double maxX, size_t nbSeg) const;
};

typedef boost::shared_ptr<LmmCascade_gCalibrator> LmmCascade_gCalibrator_PTR;

