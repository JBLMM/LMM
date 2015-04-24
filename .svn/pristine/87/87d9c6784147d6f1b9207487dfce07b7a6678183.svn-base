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
#include <LMM/LmmModel/LmmSkewCostFunction.h>

class LmmShiftCalibrator
{
public :

	//! ctor
	LmmShiftCalibrator
		( const QuantLib::Array & init_shift_param
		, const QuantLib::Size & maxIterations       
		, const QuantLib::Real & rootEpsilon        
		, const QuantLib::Real & functionEpsilon    
		, QuantLib::CostFunction & costFunction			
		)
		: init_shift_param_(init_shift_param)
		, stopCriteria_(maxIterations, 50 , rootEpsilon, functionEpsilon, 0.)
		, costFunction_(costFunction)
		, minimization_time_(-1000)
	{}
	
	void solve();

	//! method retreiving information from QuantLib::EndCriteria
	QuantLib::Size maxIterations()                const {return stopCriteria_.maxIterations() ;}
	QuantLib::Size maxStationaryStateIterations() const {return stopCriteria_.maxStationaryStateIterations() ;}
	QuantLib::Real rootEpsilon()                  const {return stopCriteria_.rootEpsilon() ;}
	QuantLib::Real functionEpsilon()              const {return stopCriteria_.functionEpsilon() ;}
	QuantLib::Real gradientNormEpsilon()          const {return stopCriteria_.gradientNormEpsilon() ;}

	const QuantLib::EndCriteria::Type & get_EndConvergenceType() const { return endConvergenceType_ ;}

	//! print plus plus without erease
	void printPlusPlus(const std::string& base_filename) const ;

private:
	
	QuantLib::Array init_shift_param_;

	//! minimizator
	QuantLib::EndCriteria stopCriteria_;               // ( maxIterations ,minStatIterations ,rootEpsilon ,functionEpsilon ,gradientNormEpsilon );
	
	QuantLib::CostFunction & costFunction_ ;
	
	QuantLib::EndCriteria::Type endConvergenceType_;

	//! storage information after minimization
	double minimization_time_;
	QuantLib::Integer nbFunctionEvaluation_;
	QuantLib::Integer nbGradiantEvaluation_;
	QuantLib::Integer infoStopSolver_;
		
	void printStopCriteriaLevenbergMarquardt( std::ofstream & stream ) const ;
};

