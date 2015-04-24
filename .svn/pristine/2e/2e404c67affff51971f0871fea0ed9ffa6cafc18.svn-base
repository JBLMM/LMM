#pragma once

#include <LMM/instrument/VanillaFRA.h>
#include <LMM/LmmModel/Lmm.h>
#include <vector>

class LmmVanillaFRAPricer // Analytical
{
	Lmm_PTR lmm_;
	mutable std::vector<double> buffer_ZC0_;

public:

	LmmVanillaFRAPricer(const Lmm_PTR& lmm);
	virtual ~LmmVanillaFRAPricer(){};

	//YY TODO: it appears in every pricer, to move it to some common place...
	void precalculateZC0(const std::vector<double>& liborsInitValue) const; // P(T0,Ti)
	double price (const VanillaFRA& fra, const std::vector<double>& liborsInitValue) const;
};


typedef boost::shared_ptr<LmmVanillaFRAPricer> LmmVanillaFRAPricer_PTR;