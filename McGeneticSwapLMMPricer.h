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

#include <GeneticSwap.h>   //instrument
#include <Coupon.h>


/*

class McGeneticSwapLMMPricer// want to use the pre-calculation of
{
protected:
	McLmm_PTR mcLmm_; // model
public:
	McGeneticSwapLMMPricer(const McLmm_PTR mcLmm):
	mcLmm_(mcLmm)
	{};
	//gettor
	McLmm_PTR getMcLmm()const{return mcLmm_;}
	//! Pricing at time T0=0
	//double swapRate(const VanillaSwap& vanillaSwap, size_t nbSimulation) const;
	double swapNPV (const GeneticSwap_PTR geneticSwap, size_t nbSimulation,LMMTenorStructure_PTR lmmTenorStructure)  const;

	//! 
	void resetGeneratorToinitSeed(){mcLmm_->get_RNGenerator()->resetGeneratorToinitSeed();}

	// size_t find(double paymentDay, std::vector<double> dateLibor)const;
	
	double calculate(Coupon_CONSTPTR coupon, double liborValue)const;

protected: 

	//! one simulation
	double pvFloatingLeg(LMM::Index indexValuationDate,
						 const CouponLeg_PTR couponLeg,
						 const std::vector<double>& numeraire, 
						 const matrix& liborMatrix,
						 LMMTenorStructure_PTR lmmTenorStructure) const ;

public:
	double swapRate(LMM::Index indexValuationDate,
					const GeneticSwap_PTR geneticSwap,
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


typedef boost::shared_ptr<McGeneticSwapLMMPricer> McGeneticSwapLMMPricer_PTR;

*/