#pragma once

#include <GeneticSwap.h>
#include <CouponLeg.h>

#include <LMM/helper/TenorType.h>

class InstrumentFactory
{
	static GeneticSwap_PTR createVanillaSwap(size_t indexStart, size_t indexEnd, Tenor floatingLegTenor, Tenor fixedLegTenor){}
	static GeneticSwap_PTR createGeneticSwap(CouponLeg Leg1, CouponLeg Leg2){}
};

