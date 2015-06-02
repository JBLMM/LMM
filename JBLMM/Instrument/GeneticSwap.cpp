#include "JBLMM/Instrument/GeneticSwap.h"

GeneticSwap::GeneticSwap(CouponLeg_CONSTPTR leg1, CouponLeg_CONSTPTR leg2)
	:leg1_(leg1),
	 leg2_(leg2)
{
}

 GeneticSwap_PTR GeneticSwap::getSubGeneticSwap(const size_t indexStart, const size_t indexEnd) const
{
	return GeneticSwap_PTR(new GeneticSwap(getLeg1()->getSubCouponLeg(indexStart,indexEnd), 
		                                   getLeg2()->getSubCouponLeg(indexStart,indexEnd)));
}