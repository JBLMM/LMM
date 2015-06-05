#pragma once

#include <iostream>
#include <cassert>
#include <string>
#include <cassert>

#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <ql/termstructures/volatility/abcd.hpp>

#include <LMM/helper/TypeConverter.h>
#include <LMM/RNGenerator/RNGenerator.h>
#include <LMM/helper/LMMTenorStructure.h>

#include <LMM/LmmModel/Lmm.h>
#include <LMM/LmmModel/McLmm.h>   //Model
#include <LMM/LmmModel/McTerminalLmm.h>  

#include <JBLMM/Instrument/GeneticSwap.h>   //instrument
#include <JBLMM/Element/Coupon.h>


class McGeneticSwapLMMPricer// want to use the pre-calculation of
{

protected:
	McLmm_PTR mcLmm_; // model

public:

	McGeneticSwapLMMPricer(const McLmm_PTR mcLmm)
		:mcLmm_(mcLmm)
	{};

	//gettor
	McLmm_PTR getMcLmm()const{return mcLmm_;}

	//! Pricing at time T0=0
	//double swapRate(const VanillaSwap& vanillaSwap, size_t nbSimulation) const;
	virtual double swapNPV (const GeneticSwap_CONSTPTR geneticSwap, size_t nbSimulation)const;

	//! 
	void resetGeneratorToinitSeed(){mcLmm_->get_RNGenerator()->resetGeneratorToinitSeed();}

protected: 

	//! one simulation
	virtual double evaluateCouponLeg(	 LMM::Index indexValuationDate,
										 const CouponLeg_CONSTPTR couponLeg,
										 const std::vector<double>& numeraire, 
										 const matrix& liborMatrix,
										 LMMTenorStructure_CONSTPTR lmmTenorStructure) const;

	virtual double calculate(Coupon_CONSTPTR coupon, double liborValue)const;
};


typedef boost::shared_ptr<McGeneticSwapLMMPricer> McGeneticSwapLMMPricer_PTR;
typedef boost::shared_ptr<const McGeneticSwapLMMPricer> McGeneticSwapLMMPricer_CONSTPTR;
