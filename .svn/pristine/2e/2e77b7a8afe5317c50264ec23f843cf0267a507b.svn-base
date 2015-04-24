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

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/LmmBaseCostFunction.h>


/*! \class LmmGlobal_gCostFunction
 *
 *
 *
 */

class LmmGlobal_gCostFunction : public LmmBaseCostFunction
{
public:

	//! constructor 
	LmmGlobal_gCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
				   , LiborQuotes_ConstPTR liborQuotes_ptr
		           , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes // instrument to calibrate 
				   , GMatrixMapping_PTR gMatrixMapping_ptr
				   , Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam
				   , LmmPenalty_PTR penalty_ptr);

	virtual std::string Name() const { return "LmmGlobal_gCostFunction"; }

	// specific row for local calib
	virtual void reset_SwaptionRow(const size_t iDelegate)  { throw("reset_SwaptionRow can not be called in Global Cost Function");}	
	virtual void reset_SwaptionCell(const size_t iDelegate, const size_t jDelegate) { throw("reset_SwaptionCell can not be called in Global Cost Function");}	
	
	virtual void reset_CalibrationParams(const QuantLib::Array & params) const ;

	//const Array& param_array
	Disposable<Array> values(const Array& x) const ; 

	// Getter!
	// attention, methods returning matrix copies, do not use in a loop
	LmmPenalty_PTR get_LmmPenalty_PTR() {return pLmmPenalty_ ; }
	GMatrixMapping_PTR get_GMatrixMapping(){ return pGMatrixMapping_;}
	Shifted_HGVolatilityParam_PTR get_Shifted_HGVolatilityParam() {return buffer_Shifted_HGVolatilityParam_;}

	//! use only for print calib evolution in the case of virtual calibration test
	//! in this case, we know the "true parameters" which is used for generating market data
	void reset_TrueParam(const QuantLib::Array & true_param); 

	virtual void print(const std::string& filename) const;

	double calc_PenaltyTimeHomogeneity_L2() const ;
	double calc_PenaltyTimeHomogeneity_L1() const ;
	double calc_PenaltyTimeHomogeneity_L_INF() const ;
	
	double calc_PenaltySmoothMaturity_L2() const ;
	double calc_PenaltySmoothMaturity_L1() const ;
	double calc_PenaltySmoothMaturity_L_INF() const ;

protected :

	virtual void fully_UpdateSwaptionMdlValues() const ;
	virtual void partially_UpdateSwaptionMdlValues() const { throw("partially_UpdateSwaptionMdlValues can not be called in Global Cost Function");}
	
private :

	LmmPenalty_PTR pLmmPenalty_;			

	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato_;
	mutable GMatrixMapping_PTR  pGMatrixMapping_ ; 
	mutable Shifted_HGVolatilityParam_PTR buffer_Shifted_HGVolatilityParam_;	
	
	std::vector<double> buffer_calib_reference;// used for test
	QuantLib::Array error_calib(const QuantLib::Array & actual_param) const;

	size_t nbSwaptions_;
	size_t calc_nbPenalties() const ; 
};

typedef boost::shared_ptr<LmmGlobal_gCostFunction> LmmGlobal_gCostFunction_PTR;