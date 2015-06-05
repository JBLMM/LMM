#pragma once

#include <vector>

#include <GeneticSwap.h>
#include <CouponLeg.h>
#include <CappedFlooredCoupon.h>
#include <LiborRate.h>

#include <LMM/helper/LMMTenorStructure.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/Name.h>

class InstrumentFactory
{
public:
	static GeneticSwap_PTR createVanillaSwap(double strike, 
											LMM::Index indexStart, 
											LMM::Index indexEnd, 
											Tenor floatingLegTenor, 
											Tenor fixedLegTenor,
											LMMTenorStructure_PTR swapStructure,
											double nominal);

	//static GeneticSwap_PTR createTARNSwap(	double target,
	//										std::vector<LMM::Index> indexGet, 
	//										std::vector<LMM::Index> indexGive, 
	//										std::vector<double> indexPeriodGet, 
	//										std::vector<double> indexPeriodGive, 
	//										LMMTenorStructure_PTR swapStructure,
	//										double nominal);

	static GeneticSwap_PTR createGeneticSwap(CouponLeg_PTR Leg1, CouponLeg_PTR Leg2);

};

