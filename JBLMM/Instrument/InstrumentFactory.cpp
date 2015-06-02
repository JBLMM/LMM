#include "JBLMM/Instrument/InstrumentFactory.h"
#include <JBLMM/Instrument/GeneticSwap.h>
#include <vector>
#include <iostream>
#include <cassert>
#include <JBLMM/Element/TargetCoupon.h>


GeneticSwap_PTR InstrumentFactory::createVanillaSwap(	 double strike, 
														 LMM::Index indexStart, 
												 		 LMM::Index indexEnd, 
														 Tenor floatingLegTenor, 
														 Tenor fixedLegTenor,
														 LMMTenorStructure_PTR swapStructure,
														 double nominal)
{
	//CouponLeg_CONSTPTR floatingLeg; // (new CouponLeg());
	//CouponLeg_CONSTPTR fixedLeg;    // (new CouponLeg());

	//! check 
	size_t floatingVsLiborTenorTypeRatio = floatingLegTenor.ratioTo(swapStructure->get_tenorType()) ;
	size_t fixedVsLiborTenorTypeRatio    = fixedLegTenor.ratioTo(swapStructure->get_tenorType())   ;

	assert(floatingLegTenor == swapStructure->get_tenorType() ) ;  // floatingTenor == lmmTenor!

	assert( indexEnd > indexStart ); assert( indexStart >=0 );
	assert( (indexEnd - indexStart)%floatingVsLiborTenorTypeRatio ==0  );
	assert( (indexEnd - indexStart)%fixedVsLiborTenorTypeRatio    ==0  );

	size_t nbFloatLeg =  (indexEnd - indexStart)/floatingVsLiborTenorTypeRatio;
	size_t nbFixedLeg = (indexEnd - indexStart)/fixedVsLiborTenorTypeRatio;


	//! 
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

	std::vector<Coupon_CONSTPTR> floatingCouponVector;
	std::vector<Coupon_CONSTPTR> fixedCouponVector;
	bool   floatingLeg_ifFloored   = false;
	double floatingLeg_floorStrike = -1.0e100;
	bool   floatingLeg_ifCapped    = false;
	double floatingLeg_capStrike   = 1.0e100;
	double floatingLeg_multFactor = 1.0;
	double floatingLeg_addFactor  = 0.0;

	bool   fixedLeg_ifFloored   = false;
	double fixedLeg_floorStrike = -1.0e100;
	bool   fixedLeg_ifCapped    = false;
	double fixedLeg_capStrike   = 1.0e100;
	double fixedLeg_multFactor = 0.0;
	double fixedLeg_addFactor  = strike;

	LMM::Index  valuationDateIndex = 0 ; 
	for(size_t i=0;i<floatingLegPaymentIndexSchedule.size(); i++)
	{
		LMM::Index couponPaymentIndex=floatingLegPaymentIndexSchedule[i];
		floatingCouponVector.push_back(Coupon_CONSTPTR(new CappedFlooredCoupon(	couponPaymentIndex, 
																				nominal, 
																				floatingLegTenor.YearFraction(), 
																				floatingLeg_ifFloored,
																				floatingLeg_floorStrike,
																				floatingLeg_ifCapped,
																				floatingLeg_capStrike,
																				nullptr,
																				floatingLeg_multFactor,
																				floatingLeg_addFactor,
																				valuationDateIndex)));
			}

	for(size_t i=0;i<fixedLegPaymentIndexSchedule.size(); i++)
	{
		LMM::Index couponPaymentIndex=fixedLegPaymentIndexSchedule[i];
		fixedCouponVector.push_back(Coupon_PTR(new CappedFlooredCoupon(	couponPaymentIndex, 
																		nominal, 
																		fixedLegTenor.YearFraction(), 
																		fixedLeg_ifFloored,
																		fixedLeg_floorStrike, 
																		fixedLeg_ifCapped,
																		fixedLeg_capStrike, 
																		nullptr, 
																		fixedLeg_multFactor, 
																		fixedLeg_addFactor, 
																		valuationDateIndex)));
	}
	GeneticSwap_PTR vanillaSwap(new GeneticSwap(CouponLeg_CONSTPTR(new CouponLeg(floatingCouponVector)),
												CouponLeg_CONSTPTR(new CouponLeg(fixedCouponVector))));
	return vanillaSwap;
}

GeneticSwap_PTR InstrumentFactory::createStandardTARNSwap(	double strike, 
															LMM::Index indexStart, 
												 			LMM::Index indexEnd, 
															Tenor floatingLegTenor, 
															Tenor fixedLegTenor,
															LMMTenorStructure_PTR swapStructure,
															double nominal,
															double target)
{
	//CouponLeg_CONSTPTR floatingLeg; // (new CouponLeg());
	//CouponLeg_CONSTPTR fixedLeg;    // (new CouponLeg());

	//! check 
	size_t floatingVsLiborTenorTypeRatio = floatingLegTenor.ratioTo(swapStructure->get_tenorType()) ;
	size_t fixedVsLiborTenorTypeRatio    = fixedLegTenor.ratioTo(swapStructure->get_tenorType())   ;

	assert(floatingLegTenor == swapStructure->get_tenorType() ) ;  // floatingTenor == lmmTenor!

	assert( indexEnd > indexStart ); assert( indexStart >=0 );
	assert( (indexEnd - indexStart)%floatingVsLiborTenorTypeRatio ==0  );
	assert( (indexEnd - indexStart)%fixedVsLiborTenorTypeRatio    ==0  );

	size_t nbFloatLeg =  (indexEnd - indexStart)/floatingVsLiborTenorTypeRatio;
	size_t nbFixedLeg = (indexEnd - indexStart)/fixedVsLiborTenorTypeRatio;


	//! 
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

	std::vector<Coupon_CONSTPTR> floatingCouponVector;
	std::vector<Coupon_CONSTPTR> fixedCouponVector;
	bool   floatingLeg_ifFloored   = false;
	double floatingLeg_floorStrike = -1.0e100;
	bool   floatingLeg_ifCapped    = false;
	double floatingLeg_capStrike   = 1.0e100;
	double floatingLeg_multFactor = 1.0;
	double floatingLeg_addFactor  = 0.0;

	bool   fixedLeg_ifFloored   = false;
	double fixedLeg_floorStrike = -1.0e100;
	bool   fixedLeg_ifCapped    = false;
	double fixedLeg_capStrike   = 1.0e100;
	double fixedLeg_multFactor = 0.0;
	double fixedLeg_addFactor  = strike;

	std::string leg1CouponDependency = "";
	std::string leg2CouponDependency = "";

	LMM::Index  valuationDateIndex = 0 ; 

	for(size_t i=0;i<floatingLegPaymentIndexSchedule.size(); i++)
	{
		LMM::Index couponPaymentIndex=floatingLegPaymentIndexSchedule[i];
		floatingCouponVector.push_back(Coupon_CONSTPTR(new TargetCoupon(	couponPaymentIndex, 
																			nominal, 
																			floatingLegTenor.YearFraction(), 
																			floatingLeg_ifFloored,
																			floatingLeg_floorStrike,
																			floatingLeg_ifCapped,
																			floatingLeg_capStrike,
																			nullptr,
																			floatingLeg_multFactor,
																			floatingLeg_addFactor,
																			valuationDateIndex,
																			target,
																			leg1CouponDependency)));
		leg1CouponDependency += "l"; 
	}

	for(size_t i=0;i<fixedLegPaymentIndexSchedule.size(); i++)
	{
		LMM::Index couponPaymentIndex=fixedLegPaymentIndexSchedule[i];
		fixedCouponVector.push_back(Coupon_CONSTPTR(new TargetCoupon(	couponPaymentIndex, 
																		nominal, 
																		fixedLegTenor.YearFraction(), 
																		fixedLeg_ifFloored,
																		fixedLeg_floorStrike,
																		fixedLeg_ifCapped,
																		fixedLeg_capStrike,
																		nullptr,
																		fixedLeg_multFactor,
																		fixedLeg_addFactor,
																		valuationDateIndex,
																		target,
																		leg2CouponDependency)));
		leg2CouponDependency += "l"; 
	}
	GeneticSwap_PTR targetSwap(new GeneticSwap(	CouponLeg_CONSTPTR(new CouponLeg(floatingCouponVector)),
												CouponLeg_CONSTPTR(new CouponLeg(fixedCouponVector))));
	return targetSwap;
}
//GeneticSwap_PTR InstrumentFactory::createTARNSwap(	std::vector<TargetCoupon_CONSTPTR> leg1,
//													std::vector<TargetCoupon_CONSTPTR> leg2,
//													LMMTenorStructure_PTR swapStructure)
//{
//		//std::sort(indexGet.begin(),indexGet.end());
//		//std::sort(indexGive.begin(),indexGive.end());
//
//		for(size_t i=1;i<indexGet.size();i++){assert(indexGet[i]!=indexGet[i-1]);}
//		for(size_t i=1;i<indexGive.size();i++){assert(indexGive[i]!=indexGive[i-1]);}
//
//		assert(indexGet.size()==indexPeriodGet.size());
//		assert(indexGive.size()==indexPeriodGive.size());
//
//		size_t lmmTenorStructureEndIndex = swapStructure->get_horizon()+1;
//		assert(indexGet[indexGet.size()-1]<=lmmTenorStructureEndIndex);
//		assert(indexGive[indexGive.size()-1]<=lmmTenorStructureEndIndex);
//		assert(indexGet[0]>=1);
//		assert(indexGive[0]>=1);
//
//	//precalculate deltaT for fixed and float legs, moved from LmmVanillaSwapPricer (30.05.2014)
//	//assert( lmmTenorStructure->get_horizon()   >= this->get_indexEnd() );// if not cannot price this swap;
//
//	//! floatingLeg
//	//const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = this->get_floatingLegPaymentIndexSchedule();
//
//		CouponLeg_PTR legGet(new CouponLeg());
//		CouponLeg_PTR legGive(new CouponLeg());
//
//		for(size_t i=0;i<indexGet.size(); i++)
//		{
//			LMM::Index couponIndex= indexGet[i];
//			Rate_PTR rate(new LiborRate());
//			legGet->addLeg(Coupon_PTR(new TargetCoupon( swapStructure->get_tenorDate(couponIndex), 
//														nominal, 
//														indexPeriodGet[i], 
//														liborsInitValue[couponIndex-1],  0.0,
//														rate, 0.0, 0.0, 
//														swapStructure->get_tenorDate(couponIndex-1),
//														target
//														)));
//		}
//
//		for(size_t i=0;i<indexGive.size(); i++)
//		{
//			size_t couponIndex= indexGive[i];
//			Rate_PTR rate(new LiborRate());
//			legGive->addLeg1(Coupon_PTR(new TargetCoupon(
//				swapStructure->get_tenorDate(couponIndex), 
//				nominal, 
//				indexPeriodGive[i], 
//				liborsInitValue[couponIndex-1],  0.0,
//				rate, 0.0, 0.0, 
//				swapStructure->get_tenorDate(couponIndex-1),
//				target
//				)));
//		}
//		GeneticSwap_PTR TARNSwap(new GeneticSwap(legGet,legGive));
//		return TARNSwap;
//}	

GeneticSwap_PTR InstrumentFactory::createGeneticSwap(CouponLeg_PTR Leg1, CouponLeg_PTR Leg2)
{
	GeneticSwap_PTR geneticSwap(new GeneticSwap(Leg1,Leg2));
	return geneticSwap;
}
