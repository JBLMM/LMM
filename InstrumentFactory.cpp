#include "InstrumentFactory.h"
#include <GeneticSwap.h>
#include <vector>
#include <iostream>
#include <cassert>


	GeneticSwap_PTR InstrumentFactory::createVanillaSwap(double strike, 
											size_t indexStart, 
											size_t indexEnd, 
											Tenor floatingLegTenor, 
											Tenor fixedLegTenor,
											LMMTenorStructure_PTR swapStructure,
											double nominal,
											std::vector<double>& liborsInitValue)
	{
		CouponLeg_PTR floatingLeg(new CouponLeg());
		CouponLeg_PTR fixedLeg(new CouponLeg());

	size_t floatingVsLiborTenorTypeRatio = floatingLegTenor.ratioTo(swapStructure->get_tenorType() ) ;
	size_t fixedVsLiborTenorTypeRatio    = fixedLegTenor.ratioTo( swapStructure->get_tenorType() )   ;

	assert(floatingLegTenor == swapStructure->get_tenorType() ) ;  // floatingTenor == lmmTenor!

	assert( indexEnd > indexStart ); assert( indexStart >=0 );
	assert( (indexEnd - indexStart)%floatingVsLiborTenorTypeRatio ==0  );
	assert( (indexEnd - indexStart)%fixedVsLiborTenorTypeRatio    ==0  );

	size_t nbFloatLeg =  (indexEnd - indexStart)/floatingVsLiborTenorTypeRatio;
	size_t nbFixedLeg = (indexEnd - indexStart)/fixedVsLiborTenorTypeRatio;

	std::vector<size_t> floatingLegPaymentIndexSchedule;
	std::vector<size_t> fixedLegPaymentIndexSchedule;

	for(size_t i=0; i<nbFloatLeg ; ++i)
	{
		floatingLegPaymentIndexSchedule.push_back(indexStart+(i+1)*floatingVsLiborTenorTypeRatio);
	}

	for(size_t i=0; i< nbFixedLeg ; ++i)
	{
		fixedLegPaymentIndexSchedule.push_back(indexStart+(i+1)*fixedVsLiborTenorTypeRatio);
	}

	//precalculate deltaT for fixed and float legs, moved from LmmVanillaSwapPricer (30.05.2014)
	//assert( lmmTenorStructure->get_horizon()   >= this->get_indexEnd() );// if not cannot price this swap;

	//! floatingLeg
	//const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = this->get_floatingLegPaymentIndexSchedule();


		for(size_t j=0;j<floatingLegPaymentIndexSchedule.size(); j++)
		{
			size_t i=floatingLegPaymentIndexSchedule[j];
			Rate_PTR rate(new LiborRate());
			floatingLeg->addLeg1(Coupon_PTR(new CappedFlooredCoupon(
				swapStructure->get_tenorDate(i), 
				nominal, 
				floatingLegTenor.YearFraction(), 
				liborsInitValue[i-1],  0.0,
				rate, 0.0, 0.0, 
				swapStructure->get_tenorDate(i)
				)
				)
				);
		}

		for(size_t j=0;j<fixedLegPaymentIndexSchedule.size(); j++)
		{
			size_t i=fixedLegPaymentIndexSchedule[j];
			Rate_PTR rate(new LiborRate());
			fixedLeg->addLeg1(Coupon_PTR(new CappedFlooredCoupon(
				swapStructure->get_tenorDate(i), nominal, floatingLegTenor.YearFraction(), strike, 0.0, rate, 0.0, 0.0, swapStructure->get_tenorDate(i))));
		}

		//return vanillaSwap
		GeneticSwap_PTR vanillaSwap(new GeneticSwap(floatingLeg,fixedLeg));
		return vanillaSwap;
	}

	GeneticSwap_PTR InstrumentFactory::createGeneticSwap(CouponLeg_PTR Leg1, CouponLeg_PTR Leg2)
	{
		GeneticSwap_PTR geneticSwap(new GeneticSwap(Leg1,Leg2));
		return geneticSwap;
	}
