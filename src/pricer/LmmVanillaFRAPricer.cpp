#include <LMM/pricer/LmmVanillaFRAPricer.h>
#include <cassert>
#include <vector>


LmmVanillaFRAPricer::LmmVanillaFRAPricer(const Lmm_PTR& lmm) 
	: lmm_(lmm), 
	buffer_ZC0_(lmm_->get_horizon()+2){};

void LmmVanillaFRAPricer::precalculateZC0(const std::vector<double>& liborsInitValue) const // P(T0,Ti)
{
	const std::vector<double>& deltaT = lmm_->get_LMMTenorStructure()->get_deltaT(); // it's the same as swap floating tenor, but need to check

	buffer_ZC0_[0] = 1.0;
	for(size_t i=1; i<buffer_ZC0_.size(); ++i)
	{
		size_t indexLibor = i-1;
		// P(0,T_{i-1}) / P(0,T_i)   = 1 + (T_{i}-T_{i-1}) L(0,T_{i-1},T_i)
		buffer_ZC0_[i] = buffer_ZC0_[i-1]/(1+deltaT[indexLibor]*liborsInitValue[indexLibor]);
	}
}

double LmmVanillaFRAPricer::price(const VanillaFRA& fra, const std::vector<double>& liborsInitValue) const
{
	assert( lmm_->get_LMMTenorStructure()->get_tenorType() == fra.get_lmmTenorStructureTenorType()); // this pricer can price this FRA
	assert(liborsInitValue.size() == lmm_->get_nbLIBOR());

	precalculateZC0(liborsInitValue);

	LMM::Index fixingIndex  = fra.get_indexFixing();
	LMM::Index paymentIndex = fra.get_indexPayment();

	double fwd   = liborsInitValue[fixingIndex];	
	double df    = buffer_ZC0_[paymentIndex];
	double price = df*fra.payoff(fwd);

	return price;
}
