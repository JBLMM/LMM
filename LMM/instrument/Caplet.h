#pragma once

#include <cassert>

#include <LMM/helper/Name.h>
#include <LMM/helper/TenorType.h>

class Caplet
{

public:
	Caplet(const double& strike,
		LMM::Index  indexFixing, 
		LMM::Index  indexPayement, 
		Tenor    underlyingLiborTenorType,
		Tenor    lmmTenorStructureTenorType);	

private:

	double strike_;

	LMM::Index indexFixing_;      
	LMM::Index indexPayement_;   
	Tenor underlyingLiborTenorType_;
	Tenor lmmTenorStructureTenorType_; // used to check pricer's tenor type
};
