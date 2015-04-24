#pragma once

#include <deque>

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>

#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/GMatrixMapping.h>
#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>
#include <LMM/LmmModel/LmmPenalty.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/LmmBaseCostFunction.h>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>


class LmmCascade_gCostFunction : public LmmBaseCostFunction
{
public:

	//! constructor 
	LmmCascade_gCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
						    , LiborQuotes_ConstPTR liborQuotes_ptr
						    , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes // instrument to calibrate 
				            , GMatrixMapping_PTR gMatrixMapping_ptr
						    , Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam
						    );

	virtual std::string Name() const { return "LmmCascade_gCostFunction" ; }
		
	virtual void reset_SwaptionRow(const size_t iDelegate)  { throw("reset_SwaptionRow can not be called in Cascade Cost Function");}	

	virtual void reset_SwaptionCell(const size_t iDelegate, const size_t jDelegate) ;
		
	virtual void reset_CalibrationParams(const QuantLib::Array & params) const ;
	
	//const Array& param_array
	Disposable<Array> values(const Array& x) const ; 
	
	// Getter!
	// attention, methods returning matrix copies, do not use in a loop
	GMatrixMapping_PTR get_GMatrixMapping(){ return pGMatrixMapping_;}
	Shifted_HGVolatilityParam_PTR get_Shifted_HGVolatilityParam() { return buffer_Shifted_HGVolatilityParam_; }
	
	void print(const std::string& filename) const;

	virtual void reset_TrueParam(const QuantLib::Array & true_param) ; 

	const std::pair<size_t,size_t>& get_CurrentCell() const { return current_cell_;}

	const double get_current_diff_Value() const 
	{
		const UpperTriangularVanillaSwaptionQuotes & mkt_swaption_quote = pUpperTriangleVanillaSwaptionQuotes_->get_UpperTriangularVanillaSwaptionQuotes() ;
		size_t iExperity = current_cell_.first;
		size_t jTenor = current_cell_.second;
		return upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor) - mkt_swaption_quote(iExperity,jTenor).second ;	
	}

	const double get_current_mdl_Value() const 
	{
		size_t iExperity = current_cell_.first;
		size_t jTenor = current_cell_.second;
		return upperTriangleVanillaSwaptionMdlValues_(iExperity,jTenor);	
	}

protected :
		//! pricing 
	virtual void fully_UpdateSwaptionMdlValues() const ;
	virtual void partially_UpdateSwaptionMdlValues() const ;

private :

	std::pair<size_t,size_t> current_cell_;

	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato_;
	mutable GMatrixMapping_PTR  pGMatrixMapping_ ; 
	mutable Shifted_HGVolatilityParam_PTR buffer_Shifted_HGVolatilityParam_;	

	std::vector<double> buffer_calib_reference;// used for test
	QuantLib::Array error_calib(const QuantLib::Array & actual_param) const;
};

typedef boost::shared_ptr<LmmCascade_gCostFunction> LmmCascade_gCostFunction_PTR;