#include <iostream>

#include <cassert>
#include <LMM/instrument/VanillaFRA.h>

VanillaFRA::VanillaFRA(LMM::Index  indexFixing, 
					   LMM::Index  indexPayement, 
					   Tenor	   underlyingLiborTenorType,
					   LMMTenorStructure_PTR lmmTenorStructure)
					   : indexFixing_(indexFixing)
					   , indexPayement_(indexPayement)
					   , underlyingLiborTenorType_(underlyingLiborTenorType)
					   , lmmTenorStructure_(lmmTenorStructure)
{
	assert( underlyingLiborTenorType_ == lmmTenorStructure_->get_tenorType());
	assert( indexFixing +1 == indexPayement); // vanillaFRA
}


double VanillaFRA::payoff(double liborRate) const // for MC
{
	double deltaT =  lmmTenorStructure_->get_deltaT(indexFixing_);
	return deltaT*liborRate;
}