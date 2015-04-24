#include <iostream>

#include <LMM/instrument/Caplet.h>

Caplet::Caplet(const double& strike,
			   LMM::Index  indexFixing, 
			   LMM::Index  indexPayement, 
			   Tenor    underlyingLiborTenorType,
			   Tenor    lmmTenorStructureTenorType)
			   : strike_(strike)
			   , indexFixing_(indexFixing)
			   , indexPayement_(indexPayement)
			   , underlyingLiborTenorType_(underlyingLiborTenorType)
			   , lmmTenorStructureTenorType_(lmmTenorStructureTenorType)
{
	assert( underlyingLiborTenorType_ == lmmTenorStructureTenorType_);
}
