#pragma once

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

#include <LMM/LmmModel/LmmABCDCostFunction.h>
#include <LMM/LmmModel/LmmBaseCalibrator.h>
#include <LMM/LmmModel/LmmBaseCostFunction.h>

class LmmABCDCalibrator : public LmmBaseCalibrator
{
public :

	//! ctor
	LmmABCDCalibrator
		(
		const QuantLib::Array & init_abcd
		, const QuantLib::Size & maxIterations       
		, const QuantLib::Real & rootEpsilon        
		, const QuantLib::Real & functionEpsilon    
		, LmmBaseCostFunction_PTR & pLmmBaseCostFunction		
		)
		: LmmBaseCalibrator(maxIterations, rootEpsilon , functionEpsilon, pLmmBaseCostFunction )
		, init_abcd_(init_abcd)
		, found_abcd_(init_abcd.size() )		
	{
		assert(init_abcd.size() == 4);
		for(size_t i=0;i<4;++i){found_abcd_[i]=-1000000;} //initiate to absurb value
	}

	virtual std::string Name() const 
	{
		if(isVirtualCalibration_) return "VIRTUAL ABCD Calibrator";
		else  return "ABCD Calibrator";
	}

	void solve();

	void reset_d_constraint(const double& mean_d, const double & margin /* % allowable around d*/) ;

	//! getter
	void add_ConstraintCell(const std::pair<size_t,size_t>& cell)
	{
		boost::shared_ptr<LmmABCDCostFunction> pLmmABCDCostFunction = boost::static_pointer_cast<LmmABCDCostFunction>(pLmmBaseCostFunction_);
		pLmmABCDCostFunction->add_ConstraintCell(cell);
	}

	const std::vector<std::pair<size_t,size_t> >& get_ConstraintCell() const 
	{
		boost::shared_ptr<LmmABCDCostFunction> pLmmABCDCostFunction = boost::static_pointer_cast<LmmABCDCostFunction>(pLmmBaseCostFunction_);
		return pLmmABCDCostFunction->get_ConstraintCell();		
	}

	Shifted_HGVolatilityParam_PTR get_Shifted_HGVolatilityParam() 
	{
		boost::shared_ptr<LmmABCDCostFunction> pLmmABCDCostFunction = boost::static_pointer_cast<LmmABCDCostFunction>(pLmmBaseCostFunction_);
		return pLmmABCDCostFunction->get_Shifted_HGVolatilityParam() ;
	}

	//! method retreiving information from QuantLib::EndCriteria
	const QuantLib::Array& get_Found_ABCD()const { return found_abcd_;}

	//! print plus plus without erease
	void printPlusPlus(const std::string& base_filename) const ;

protected :

	virtual void retreive_calib_info();

private:

	const QuantLib::Array init_abcd_;
	QuantLib::Array found_abcd_;

	QuantLib::Integer infoStopSolver_;
};