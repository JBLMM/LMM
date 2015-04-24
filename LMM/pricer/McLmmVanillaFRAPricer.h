
//! MClmm price Swap, don't need to be too general !!!  
#pragma once
#include <boost/shared_ptr.hpp>

#include <LMM/LmmModel/McLmm.h>        // Model
#include <LMM/instrument/VanillaFRA.h>  // Instrument



class McLmmVanillaFRAPricer 
{
private:
	McLmm_PTR mcLmm_;
	//mutable std::vector<double> buffer_ZC0_;
public:
	McLmmVanillaFRAPricer(const McLmm_PTR& mcLmm)
		: mcLmm_(mcLmm){};

	//! Pricing at time T0 = 0
	double price (const VanillaFRA& vanillaCaplet, size_t nbSimulation)  const;

	//!YY: every where, maybe move it to higher level
	void resetGeneratorToinitSeed(){mcLmm_->get_RNGenerator()->resetGeneratorToinitSeed();}
};

typedef boost::shared_ptr<McLmmVanillaFRAPricer> McLmmVanillaFRAPricer_PTR;