#pragma once

#include <cassert>

#include <boost/shared_ptr.hpp>

#include <LMM/numeric/NumericalMethods.h>
#include <LMM/instrument/VanillaSwaption.h>
#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/Dispersion.h>
#include <LMM/pricer/LmmVanillaSwapPricer.h>

/*! \class LmmApproxVanillaSwaptionPricer_Piterbarg
*  Defining approx methods for swaption pricing
*  to use the precalculation.
*/

//YY: terminal prob => N+1
class LmmVanillaSwaptionApproxPricer : public LmmVanillaSwapPricer
{

public:

	virtual double volBlack(const VanillaSwaption& vanillaswaption, const std::vector<double>& liborsInitValue) const = 0;
	virtual double price   (const VanillaSwaption& vanillaswaption, const std::vector<double>& liborsInitValue)const = 0; 
	virtual void   update_VolatilityParam(VolatilityParam_PTR vol_param_ptr) = 0; //! YY Bad idea, this should be in the Model, but not the pricer. 
	//! destructor
	virtual ~LmmVanillaSwaptionApproxPricer();

};

typedef boost::shared_ptr<LmmVanillaSwaptionApproxPricer> LmmVanillaSwaptionApproxPricer_PTR;