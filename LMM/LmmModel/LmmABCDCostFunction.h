#pragma once

#include <ql/types.hpp>
#include <ql/math/array.hpp>


#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/GMatrixMapping.h>
#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>
#include <LMM/LmmModel/LmmPenalty.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>
#include <LMM/LmmModel/LmmBaseCostFunction.h>

/*! \class LmmABCDCostFunction
 *
 *
 *
 */

class LmmABCDCostFunction : public LmmBaseCostFunction
{
public:

	//! constructor 
	LmmABCDCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
				       , LiborQuotes_ConstPTR liborQuotes_ptr
		               , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSwaptionQuotes // instrument to calibrate 
				       , Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam
				       );

	virtual std::string Name() const { return "LmmABCDCostFunction"; }

	virtual void reset_SwaptionRow(const size_t iDelegate)  { throw("reset_SwaptionRow can not be called in LmmABCDCostFunction");}	
	virtual void reset_SwaptionCell(const size_t iDelegate, const size_t jDelegate) { throw("reset_SwaptionCell can not be called in LmmABCDCostFunction");}	

	virtual void reset_CalibrationParams(const QuantLib::Array & params) const ;
	
	//const Array& param_array
	virtual Disposable<Array> values(const Array& x) const ; 

	void add_ConstraintCell(const std::pair<size_t,size_t>& cell) // YY TODO: change name index_swaptions_to_calibrate
	{
		assert(cell.first < pUpperTriangleVanillaSwaptionQuotes_->size1() - 1 && cell.second < pUpperTriangleVanillaSwaptionQuotes_->size2() - 1 );
		assert(cell.first + cell.second < pUpperTriangleVanillaSwaptionQuotes_->size1() );
		calib_constraint.push_back(cell);
		std::sort( calib_constraint.begin(), calib_constraint.end() );
	}

	// Getter! 
	Shifted_HGVolatilityParam_PTR get_Shifted_HGVolatilityParam() {return buffer_Shifted_HGVolatilityParam_;}
	LmmVanillaSwaptionApproxPricer_Rebonato_PTR get_LmmVanillaSwaptionApproxPricer_Rebonato() const { return pLmmVanillaSwaptionApproxPricer_Rebonato_;}

	const std::vector<std::pair<size_t,size_t> >& get_ConstraintCell() const { return calib_constraint; }

	//! use only for print calib evolution in the case of virtual calibration test
	//! in this case, we know the "true parameters" which is used for generating market data
	void reset_TrueParam(const QuantLib::Array & true_param); 

	virtual void print(const std::string& filename) const ;

protected :

	virtual void fully_UpdateSwaptionMdlValues() const     ;
	virtual void partially_UpdateSwaptionMdlValues() const ;

private :

	// If these cell vector is empty, use the whole swaption quote
	// If not empty, use only these cell from the VCUB to calibrate
	// If user choose to use these selected cell, he has to add at least 4 cells.
	std::vector<std::pair<size_t,size_t> > calib_constraint;
	
	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato_;
	mutable Shifted_HGVolatilityParam_PTR buffer_Shifted_HGVolatilityParam_;	

	std::vector<double> buffer_calib_reference;// used for test
	bool breakForPrintOut(unsigned int nbIter) const { return (nbCalled % 5 == 0) ; }
	QuantLib::Array error_calib(const QuantLib::Array & actual_param) const;

	size_t calc_nbSwaptions() const ; 
};

typedef boost::shared_ptr<LmmABCDCostFunction> LmmABCDCostFunction_PTR;