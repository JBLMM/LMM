#include <iostream>

#include <LMM/instrument/VanillaCaplet.h>

VanillaCaplet::VanillaCaplet(
	           OptionType::OptionType vanillaCapletType, 
			   double strike,
			   LMM::Index  indexFixing, 
			   LMM::Index  indexPayement, 
			   Tenor    underlyingLiborTenorType,
			   LMMTenorStructure_PTR lmmTenorStructure)
			   : vanillaCapletType_(vanillaCapletType)
			   , strike_(strike)
			   , indexFixing_(indexFixing)
			   , indexPayement_(indexPayement)
			   , underlyingLiborTenorType_(underlyingLiborTenorType)
			   , lmmTenorStructure_(lmmTenorStructure)
{
	assert( underlyingLiborTenorType_ == lmmTenorStructure_->get_tenorType());
	assert( indexFixing +1 == indexPayement); // vanillaCaplet
}


double VanillaCaplet::payoff(double liborRate) const
{
	double deltaT =  lmmTenorStructure_->get_deltaT(indexFixing_);

	if(vanillaCapletType_ == OptionType::CALL)
		return deltaT*std::max(liborRate - strike_, 0.0);
	else
		return deltaT*std::max(strike_ - liborRate, 0.0);
}
