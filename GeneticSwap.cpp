#include "GeneticSwap.h"


boost::shared_ptr<GeneticSwap> GeneticSwap::getSubGeneticSwap(size_t indexStart, size_t indexEnd) const
{
	return boost::shared_ptr<GeneticSwap>(new GeneticSwap(leg1_->getCouponLeg(indexStart,indexEnd), 
		                                   leg2_->getCouponLeg(indexStart,indexEnd)));
}