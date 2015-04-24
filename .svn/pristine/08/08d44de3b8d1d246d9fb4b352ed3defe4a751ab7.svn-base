#pragma once

#include <deque>

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>

#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/GMatrixMapping.h>
#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/LmmBaseCostFunction.h>

#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>


/*! \class LmmLocal_gCostFunction
 *
 *
 *
 */

class LmmLocal_gCostFunction : public LmmBaseCostFunction
{
public:

	//! constructor 
	LmmLocal_gCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
				          , LiborQuotes_ConstPTR liborQuotes_ptr
		                  , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes // instrument to calibrate 
				          , GMatrixMapping_PTR gMatrixMapping_ptr
				          , Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam);

	virtual std::string Name() const { return "LmmLocal_gCostFunction"; }

	virtual void reset_SwaptionCell(const size_t iDelegate, const size_t jDelegate) { throw("reset_SwaptionCell can not be called in Local Cost Function");}

	// specific row for local calib
	virtual void reset_SwaptionRow(const size_t iDelegate) ;
		
	virtual void reset_CalibrationParams(const QuantLib::Array & params) const ;
	
	//const Array& param_array
	Disposable<Array> values(const Array& x) const ; 
	
	// Getter!
	// attention, methods returning matrix copies, do not use in a loop
	GMatrixMapping_PTR get_GMatrixMapping(){ return pGMatrixMapping_;}
	Shifted_HGVolatilityParam_PTR get_Shifted_HGVolatilityParam() { return buffer_Shifted_HGVolatilityParam_; }
		
	//! use only for print calib evolution in the case of virtual calibration test
	//! in this case, we know the "true parameters" which is used for generating market data
	void reset_TrueParam(const QuantLib::Array & true_param); 
	
	void print(const std::string& filename) const;

protected :
	virtual void fully_UpdateSwaptionMdlValues() const ;
	virtual void partially_UpdateSwaptionMdlValues() const ;

private :

	size_t nbSwaptionOnRow_; 
	size_t specific_row_calib_; 
		
	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato_;
	mutable GMatrixMapping_PTR  pGMatrixMapping_ ; 
	mutable Shifted_HGVolatilityParam_PTR buffer_Shifted_HGVolatilityParam_;	
	
	// output de calib debugging
	std::vector<double> buffer_calib_reference;// used for test
	QuantLib::Array error_calib(const QuantLib::Array & actual_param) const;
};

typedef boost::shared_ptr<LmmLocal_gCostFunction> LmmLocal_gCostFunction_PTR;

