#pragma once

#include <deque>

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>

#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/Correlation.h>
#include <LMM/LmmModel/GMatrixMapping.h>
#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>
#include <LMM/LmmModel/LmmPenalty.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>

#include <LMM/LmmModel/LmmBaseCostFunction.h>

/*! \class LmmCorrelationCostFunction
 *
 *
 *
 */

class LmmCorrelationCostFunction : public LmmBaseCostFunction
{
public:

	//! constructor 
	LmmCorrelationCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
				              , LiborQuotes_ConstPTR liborQuotes_ptr
		                      , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes // instrument to calibrate 
				              , Correlation_PTR pCorrelation 
				              );
	
	virtual std::string Name() const { return "LmmCorrelationCostFunction" ; }
		
	virtual void reset_SwaptionRow(const size_t iDelegate)  { throw("reset_SwaptionRow can not be called in Correlation Cost Function");}	

	virtual void reset_SwaptionCell(const size_t iDelegate, const size_t jDelegate) { throw("reset_SwaptionCell can not be called in Correlation Cost Function");}

	virtual void reset_CalibrationParams(const QuantLib::Array & params) const ;
	
	//const Array& param_array
	Disposable<Array> values(const Array& x) const ; 

	// Getter!
	Correlation_PTR get_Correlation() const { return pCorrelation_ ;}
	
	//! use only for print calib evolution in the case of virtual calibration test
	//! in this case, we know the "true parameters" which is used for generating market data
	void reset_TrueParam(const QuantLib::Array & true_param); 

	void print(const std::string& filename) const;

protected :
			//! pricing 
	virtual void fully_UpdateSwaptionMdlValues() const ;
	virtual void partially_UpdateSwaptionMdlValues() const { throw("partially_UpdateSwaptionMdlValues can not be called in Correlation Cost Function");}

private :


	mutable Correlation_PTR pCorrelation_ ;
	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato_;
	
	size_t calc_nbSwaptions() const ;

	// output de calib debugging
	std::vector<double> buffer_calib_reference;// used for test
	QuantLib::Array error_calib(const QuantLib::Array & actual_param) const;		
};

typedef boost::shared_ptr<LmmCorrelationCostFunction> LmmCorrelationCostFunction_PTR;
