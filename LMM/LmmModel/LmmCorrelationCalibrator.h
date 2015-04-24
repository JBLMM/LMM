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
#include <LMM/LmmModel/LmmCorrelationCostFunction.h>
#include <LMM/LmmModel/GMatrixMapping.h>

#include <LMM/LmmModel/LmmBaseCalibrator.h>
#include <LMM/LmmModel/LmmBaseCostFunction.h>
#include <LMM/LmmModel/LmmCorrelationCostFunction.h>

class LmmCorrelationCalibrator :public LmmBaseCalibrator
{
public :

	//! ctor
	LmmCorrelationCalibrator
		(
		const QuantLib::Array & init_correl_param
		, const QuantLib::Size & maxIterations       
		, const QuantLib::Real & rootEpsilon        
		, const QuantLib::Real & functionEpsilon    
		, LmmBaseCostFunction_PTR & pLmmBaseCostFunction		
		)
		: LmmBaseCalibrator(maxIterations, rootEpsilon , functionEpsilon, pLmmBaseCostFunction )
		, init_correl_param_(init_correl_param)
		, found_correl_param_(init_correl_param.size() )
	{
			for(size_t i=0;i<found_correl_param_.size();++i){found_correl_param_[i]=-1000000;} //initiate to absurb value
	}
	
	virtual std::string Name() const 
	{
		if(isVirtualCalibration_) return "VIRTUAL Correlation Calibrator";
		else  return "Correlation Calibrator";
	}
	virtual void solve();
		
	const QuantLib::Array& get_Found_CorrelParam()const { return found_correl_param_;}
	Correlation_PTR get_Found_Correlation() const 
	{
		boost::shared_ptr<LmmCorrelationCostFunction> pLmmCorrelationCostFunction = boost::static_pointer_cast<LmmCorrelationCostFunction>(pLmmBaseCostFunction_);
		return pLmmCorrelationCostFunction->get_Correlation() ;
	}

	//! print plus plus without erease
	void printPlusPlus(const std::string& base_filename) const ;

protected :

	virtual void retreive_calib_info();

private:

	const QuantLib::Array init_correl_param_;
	QuantLib::Array found_correl_param_;

	QuantLib::Integer infoStopSolver_;
	std::string printStopCriteriaLevenbergMarquardt() const;
};

