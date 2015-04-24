
//! MClmm price Swap, don't need to be too general !!!  

#pragma once
#include <boost/shared_ptr.hpp>
#include <LMM/LmmModel/McLmm.h>        // Model
#include <LMM/instrument/VanillaSwap.h>  // Instrument
#include <LMM/pricer/LmmVanillaSwapPricer.h>


class McLmmVanillaSwapPricer : public LmmVanillaSwapPricer// want to use the pre-calculation of
{
protected:
	McLmm_PTR mcLmm_; // model
public:
	McLmmVanillaSwapPricer(const McLmm_PTR& mcLmm)
	: LmmVanillaSwapPricer(mcLmm->get_lmm()->get_LMMTenorStructure() )
	, mcLmm_(mcLmm)
	{};

	//! Pricing at time T0=0
	//double swapRate(const VanillaSwap& vanillaSwap, size_t nbSimulation) const;
	double swapNPV (const VanillaSwap& vanillaSwap, size_t nbSimulation)  const;

	//! 
	void resetGeneratorToinitSeed(){mcLmm_->get_RNGenerator()->resetGeneratorToinitSeed();}

protected: 

	//! one simulation
	double pvFloatingLeg(LMM::Index indexValuationDate,
						 const VanillaSwap& vanillaSwap,
						 const std::vector<double>& numeraire, 
						 const matrix& libors) const ;

public:
	double swapRate(LMM::Index indexValuationDate,
					const VanillaSwap& vanillaSwap,
					const std::vector<double>& numeraire, 
					const matrix& liborMatrix) const;

	//double annuity(LMM::Index indexValuationDate,
	//			   const lmmModel& lmmModel,
	//			   const std::vector<double>& numeraire) const;

	//double pvFixedLeg(LMM::Index indexValuationDate,
	//				  const lmmModel& lmmModel,
	//				  const std::vector<double>& numeraire) const;

public:
	//! To validate the result
	//double swapNPV_Analytical(const lmmModel& lmmModel,
	//	const std::vector<double>& initLibor) const;

};


typedef boost::shared_ptr<McLmmVanillaSwapPricer> McLmmVanillaSwapPricer_PTR;