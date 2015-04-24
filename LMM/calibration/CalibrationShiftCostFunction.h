#pragma once

#include <ql/types.hpp>
#include <ql/math/array.hpp>
#include <ql/math/optimization/costfunction.hpp>

#include <LMM/LmmModel/VolatilityFunction.h>
#include <LMM/pricer/LmmVanillaSwaptionApproxPricer_Rebonato.h>
#include <LMM/calibration/SwaptionMarketDataContainer.h>



class CalibrationShiftCostFunction: public CostFunction
{
public:

	typedef std::vector<double>               vector_;
	typedef std::vector<std::vector<double> > matrix_;
	typedef TestHGVolatilityParam::VectorOfMatrixIndices VectorOfMatrixIndices ;
	typedef SwaptionMarketDataContainer::SwaptionVector    SwaptionVector      ;
	
	CalibrationShiftCostFunction
		(
		Shifted_HGVolatilityParam_PTR pVolatilityParam,
		SwaptionMarketDataContainer_PTR pSkewMarketDataContainer,
		LmmVanillaSwaptionApproxPricer_Rebonato_PTR approxiPricer_ptr					 
		);

	~CalibrationShiftCostFunction();

	void reset_CalibReference(const QuantLib::Array& calib_reference);

	unsigned int get_nbCalled() const;
	
	Real value(const Array & x) const; 
	Disposable<Array> values(const Array& x) const; 
	
private:
	
	//! a possible calib reference can be know when running test, it help to see the evolution of error
	mutable std::vector<double> calib_reference_;

	//! optimizer's iteration counter, increment when value is called
	mutable unsigned int nbCalled;
	
	//! pointer to a vol param in order to hold the actual value of the volatility parameters
	mutable Shifted_HGVolatilityParam_PTR pHGVolatilityParamBuffer_;

	SwaptionMarketDataContainer_PTR pSkewMarketDataContainer_;

	mutable LmmVanillaSwaptionApproxPricer_Rebonato_PTR approxiPricer_ptr_; // Class implementing Rebonato's formula 
	
	bool breakForPrintOut(unsigned int nbIter) const ;

	QuantLib::Array computeCalibError(const Array& x)const;
};
