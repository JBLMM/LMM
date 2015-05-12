#pragma once

#include <vector>

#include <GeneticSwap.h>
#include <CouponLeg.h>
#include <CappedFlooredCoupon.h>
#include <LiborRate.h>

#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/helper/TenorType.h>

class InstrumentFactory
{
public:
	static GeneticSwap_PTR createVanillaSwap(double strike, 
											size_t indexStart, 
											size_t indexEnd, 
											Tenor floatingLegTenor, 
											Tenor fixedLegTenor,
											LMMTenorStructure_PTR swapStructure,
											double nominal,
											std::vector<double>& liborsInitValue);

	static GeneticSwap_PTR createGeneticSwap(CouponLeg_PTR Leg1, CouponLeg_PTR Leg2);

};

