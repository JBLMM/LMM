#include "GeneticSwaption.h"

GeneticSwaption::GeneticSwaption(LMM::Index maturity, GeneticSwap_CONSTPTR geneticSwap)
	:
	maturity_(maturity),
	geneticSwap_(geneticSwap_)
{
}

bool GeneticSwaption::check()const
{
	LMM::Index leg1StartIndex=getGeneticSwap()->getLeg1()->getLeg()[0]->getPaymentIndex();
	LMM::Index leg2StartIndex=getGeneticSwap()->getLeg2()->getLeg()[0]->getPaymentIndex();
	return (getMaturity()<=leg1StartIndex&&getMaturity()<=leg2StartIndex);
}
