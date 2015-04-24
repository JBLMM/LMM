#pragma once

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>

#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/GMatrixMapping.h>
#include <LMM/LmmModel/UpperTriangleVanillaSwaptionQuotes.h>
#include <LMM/LmmModel/LmmPenalty.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>

#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>


/*! \class LmmSkewCostFunction
 *
 *
 *
 */

class LmmSkewCostFunction : public CostFunction
{
public:

	//! constructor 
	LmmSkewCostFunction( LmmVanillaSwaptionApproxPricer_Rebonato_PTR lmmRobonato_ptr                // pricer  
				   , LiborQuotes_ConstPTR liborQuotes_ptr
				   , const double& quoted_strike_bump
		           , UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSkewQuotes // instrument to calibrate 
				   , Shifted_HGVolatilityParam_PTR pShifted_HGVolatilityParam );
		
	Real value(const Array & x) const ; 
	
	//const Array& param_array
	Disposable<Array> values(const Array& x) const ; 

	// Getter!
	// attention, methods returning matrix copies, do not use in a loop
	UpperTriangularDoubleMatrix get_MarketQuotes() const;
	UpperTriangularDoubleMatrix get_CurrentModelQuotes() const;
	Shifted_HGVolatilityParam_PTR get_Shifted_HGVolatilityParam(); 

	//! use only for print calib evolution in the case of virtual calibration test
	//! in this case, we know the "true parameters" which is used for generating market data
	void reset_reference_calib(const QuantLib::Array & true_param); 

private :
	//! pricing 
	void update_SwaptionMdlSkewValues() const ;

	const double quoted_strike_bump_;
	LiborQuotes_ConstPTR pLiborQuotes_;
	UpperTriangleVanillaSwaptionQuotes_ConstPTR pUpperTriangleVanillaSkewQuotes_;	

	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR pLmmVanillaSwaptionApproxPricer_Rebonato_;
	mutable UpperTriangularDoubleMatrix    mdl_swaption_skew;
	mutable Shifted_HGVolatilityParam_PTR buffer_Shifted_HGVolatilityParam_;	
	
	// output de calib debugging
	mutable unsigned int nbCalled;
	std::vector<double> buffer_calib_reference;// used for test
	bool breakForPrintOut(unsigned int nbIter) const { return (nbCalled % 5 == 0) ; }
	QuantLib::Array error_calib(const QuantLib::Array & actual_param) const;

	size_t calc_nbSwaptions() const ; 	
};

typedef boost::shared_ptr<LmmSkewCostFunction> LmmSkewCostFunction_PTR;