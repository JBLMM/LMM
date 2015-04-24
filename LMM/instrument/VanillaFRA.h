#pragma once

#include <LMM/helper/Name.h>
#include <LMM/helper/TenorType.h>
#include <LMM/helper/LMMTenorStructure.h>

class VanillaFRA
{

public:
	VanillaFRA(LMM::Index  indexFixing, 
		LMM::Index  indexPayement, 
		Tenor       underlyingLiborTenorType,
		LMMTenorStructure_PTR lmmTenorStructure);	

private:
	LMM::Index indexFixing_;      
	LMM::Index indexPayement_;   
	Tenor underlyingLiborTenorType_;
	LMMTenorStructure_PTR lmmTenorStructure_; // used to check pricer's tenor type

public:
	LMM::Index get_indexFixing () const {return indexFixing_;}
	LMM::Index get_indexPayment() const {return indexPayement_;}
	Tenor  get_lmmTenorStructureTenorType() const {return lmmTenorStructure_->get_tenorType();}
	LMMTenorStructure_PTR get_lmmTenorStructure() const {return lmmTenorStructure_;}

	double VanillaFRA::payoff(double fwd) const; // for MC
};


typedef boost::shared_ptr<VanillaFRA> VanillaFRA_PTR;