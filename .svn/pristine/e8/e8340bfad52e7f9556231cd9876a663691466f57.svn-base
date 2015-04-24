#pragma once

#include <LMM/instrument/VanillaCaplet.h>
#include <LMM/LmmModel/Lmm.h>
#include <vector>
#include <LMM/LmmModel/Shifted_HGVolatilityFunction.h>

class LmmVanillaCapletPricer  //only for Shifted_HGVolatilityFunction, not for general LMM (which may not have closed formula)
{
	Lmm_PTR lmm_; // general Lmm may not have closed formula ! 
	mutable std::vector<double> buffer_ZC0_;

	//Shifted_HGVolatilityFunction_PTR shifted_HGVolatilityFunction_;
	void initializeConstShift();

public:

	LmmVanillaCapletPricer(const Lmm_PTR& lmm);
	virtual ~LmmVanillaCapletPricer(){};

	//YY TODO: it appears in every pricer, to move it to some common place...
	void   precalculateZC0(const std::vector<double>& liborsInitValue) const; // P(T0,Ti)
	double price (const VanillaCaplet& caplet, const std::vector<double>& liborsInitValue) const;
	//double blackVol (const VanillaCaplet& caplet, const std::vector<double>& liborsInitValue) const;
	double convertPriceToBlackVol(const VanillaCaplet& caplet, const std::vector<double>& liborsInitValue, double price) const;

private:
	std::vector<double>         constShifts_;
};


typedef boost::shared_ptr<LmmVanillaCapletPricer> LmmVanillaCapletPricer_PTR;