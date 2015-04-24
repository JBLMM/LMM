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

class LmmGlobal_gCalibrator : public LmmBaseCalibrator
{
public :

	//! ctor
	LmmGlobal_gCalibrator
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
	{
		
	}

	virtual std::string Name() const 
	{
		if(isVirtualCalibration_) return "VIRTUAL Global gCalibrator";
		else  return "Global gCalibrator";
	}

	void set_VirtualTestTrueParam(const UpperTriangularDoubleMatrix& true_gDelegate)
	{
		assert(true_gDelegate.size1() == true_gDelegate_matrix_.size1() && true_gDelegate.size2() == true_gDelegate_matrix_.size2() );//parameters is the gDelegate matrix
		isVirtualCalibration_ = true;
		true_gDelegate_matrix_ = true_gDelegate;
		max_param_rel_error_ = 0;
	}

	virtual void solve();

	double get_PenaltyTimeHomogeneity_L2() const ;
	double get_PenaltyTimeHomogeneity_L1() const ;
	double get_PenaltyTimeHomogeneity_L_INF() const ;
	
	double get_PenaltySmoothMaturity_L2() const ;
	double get_PenaltySmoothMaturity_L1() const ;
	double get_PenaltySmoothMaturity_L_INF() const ;


	//! print plus plus without erease
	void printPlusPlus(const std::string& base_filename) const ;
		
	const std::vector<std::pair<size_t,size_t> > & get_BadSwaption_Indices() const { return bad_swaptions_indices_;}

protected :

	virtual void retreive_calib_info();

private:

	const GMatrixMapping gMatrixMappingInitiate_;	

	std::vector<std::pair<size_t,size_t> > bad_swaptions_indices_;//stored indices of very small or negative calibrated gDelegate

	double max_param_rel_error_;
	UpperTriangularDoubleMatrix true_gDelegate_matrix_;// known true parameter in virtual test
	UpperTriangularDoubleMatrix calibrated_gDelegate_matrix_;
	UpperTriangularDoubleMatrix rel_error_gDelegate_matrix_; // error in (%)

	QuantLib::Integer infoStopSolver_;
	std::string printStopCriteriaLevenbergMarquardt() const;
};

typedef boost::shared_ptr<LmmGlobal_gCalibrator> LmmGlobal_gCalibrator_PTR;

