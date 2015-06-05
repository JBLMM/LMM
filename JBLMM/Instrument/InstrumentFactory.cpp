#include "JBLMM/Instrument/InstrumentFactory.h"
#include <JBLMM/Instrument/GeneticSwap.h>
#include <vector>
#include <iostream>
#include <cassert>
#include <JBLMM/Element/TargetCoupon.h>


GeneticSwap_PTR InstrumentFactory::createVanillaSwap(	 double strike, 
														 LMM::Index indexStart, 
												 		 LMM::Index indexEnd, 
														 Tenor leg1Tenor,						//
														 Tenor leg2Tenor,						
														 LMMTenorStructure_PTR swapStructure,
														 double nominal)
{
	//CouponLeg_CONSTPTR floatingLeg; // (new CouponLeg());
	//CouponLeg_CONSTPTR fixedLeg;    // (new CouponLeg());

	//! check 
	size_t leg1VsLiborTenorTypeRatio	=	leg1Tenor.ratioTo(swapStructure->get_tenorType());		//lmmTenorStructure is a index base.
	size_t leg2VsLiborTenorTypeRatio    =	leg2Tenor.ratioTo(swapStructure->get_tenorType());

	//assert(leg1Tenor == swapStructure->get_tenorType() ) ;		//floatingTenor == lmmTenor!

	assert( indexEnd > indexStart ); assert( indexStart >=0 );		
	assert( (indexEnd - indexStart)%leg1VsLiborTenorTypeRatio ==0  );	//TODO: to see if necessary or not
	assert( (indexEnd - indexStart)%leg2VsLiborTenorTypeRatio ==0  );	

	size_t nbFloatLeg = (indexEnd - indexStart)/leg1VsLiborTenorTypeRatio;
	size_t nbFixedLeg =	(indexEnd - indexStart)/leg2VsLiborTenorTypeRatio;


	//to get all the payment index
	std::vector<size_t> floatingLegPaymentIndexSchedule;
	std::vector<size_t> fixedLegPaymentIndexSchedule;

	for(size_t i=0; i<nbFloatLeg ; ++i)
	{
		floatingLegPaymentIndexSchedule.push_back(indexStart+(i+1)*leg1VsLiborTenorTypeRatio);
	}

	for(size_t i=0; i< nbFixedLeg ; ++i)
	{
		fixedLegPaymentIndexSchedule.push_back(indexStart+(i+1)*leg2VsLiborTenorTypeRatio);
	}


	//Construction of couponLegs.
	std::vector<Coupon_CONSTPTR> floatingCouponVector;
	std::vector<Coupon_CONSTPTR> fixedCouponVector;

	bool   floatingLeg_ifFloored	= false;
	double floatingLeg_floorStrike	= -1.0e100;
	bool   floatingLeg_ifCapped		= false;
	double floatingLeg_capStrike	= 1.0e100;
	double floatingLeg_multFactor	= 1.0;
	double floatingLeg_addFactor	= 0.0;

	bool   fixedLeg_ifFloored		= false;
	double fixedLeg_floorStrike		= -1.0e100;
	bool   fixedLeg_ifCapped		= false;
	double fixedLeg_capStrike		= 1.0e100;
	double fixedLeg_multFactor		= 1.0;
	double fixedLeg_addFactor		= 0.0;

	LMM::Index  valuationDateIndex = 0; 
	for(size_t i=0;i<floatingLegPaymentIndexSchedule.size(); i++)	//
	{
		LMM::Index liborFixingindex = floatingLegPaymentIndexSchedule[i]-leg1VsLiborTenorTypeRatio; // TODO: verifier !!! 
		Rate_PTR liborRate(new LiborRate(liborFixingindex, leg1Tenor));
		LMM::Index couponPaymentIndex=floatingLegPaymentIndexSchedule[i];
		floatingCouponVector.push_back(Coupon_CONSTPTR(new CappedFlooredCoupon(	couponPaymentIndex, 
																				nominal, 
																				leg1Tenor.YearFraction(), 
																				floatingLeg_ifFloored,
																				floatingLeg_floorStrike,
																				floatingLeg_ifCapped,
																				floatingLeg_capStrike,
																				liborRate,
																				floatingLeg_multFactor,
																				floatingLeg_addFactor,
																				valuationDateIndex)));
	}


	for(size_t i=0;i<fixedLegPaymentIndexSchedule.size(); i++)
	{
		Rate_PTR constRate(new ConstRate(strike));
		LMM::Index couponPaymentIndex=fixedLegPaymentIndexSchedule[i];
		fixedCouponVector.push_back(Coupon_PTR(new CappedFlooredCoupon(	couponPaymentIndex, 
																		nominal, 
																		leg2Tenor.YearFraction(), 
																		fixedLeg_ifFloored,
																		fixedLeg_floorStrike, 
																		fixedLeg_ifCapped,
																		fixedLeg_capStrike, 
																		constRate, 
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
															Tenor leg1Tenor,		//
															Tenor leg2Tenor,		
															LMMTenorStructure_PTR swapStructure,
															double nominal,
															double target)
{
	//CouponLeg_CONSTPTR floatingLeg; // (new CouponLeg());
	//CouponLeg_CONSTPTR fixedLeg;    // (new CouponLeg());

	//! check 
	size_t leg1VsLiborTenorTypeRatio = leg1Tenor.ratioTo(swapStructure->get_tenorType()) ;
	size_t leg2VsLiborTenorTypeRatio    = leg2Tenor.ratioTo(swapStructure->get_tenorType())   ;

	//assert(floatingLegTenor == swapStructure->get_tenorType() ) ;  // floatingTenor == lmmTenor not necessary?

	assert( indexEnd > indexStart ); assert( indexStart >=0 );
	assert( (indexEnd - indexStart)%leg1VsLiborTenorTypeRatio ==0  );
	assert( (indexEnd - indexStart)%leg2VsLiborTenorTypeRatio ==0  );


	//!get all the paymentIndex
	std::vector<size_t> leg1PaymentIndexSchedule;
	std::vector<size_t> leg2PaymentIndexSchedule;

	//indexCoupon = indexLibor
	size_t nbLeg1 = (indexEnd - indexStart)/leg1VsLiborTenorTypeRatio;
	size_t nbLeg2 =	(indexEnd - indexStart)/leg2VsLiborTenorTypeRatio;

	for(size_t i=0; i<nbLeg1 ; ++i)
	{
		leg1PaymentIndexSchedule.push_back(indexStart+(i+1)*leg1VsLiborTenorTypeRatio);
	}
	for(size_t i=0; i< nbLeg2; ++i)
	{
		leg2PaymentIndexSchedule.push_back(indexStart+(i+1)*leg2VsLiborTenorTypeRatio);
	}

	//build couponLeg
	std::vector<Coupon_CONSTPTR> leg1CouponVector;
	std::vector<Coupon_CONSTPTR> leg2CouponVector;
	bool   leg1_ifFloored	= false;
	double leg1_floorStrike	= -1.0e100;
	bool   leg1_ifCapped		= false;
	double leg1_capStrike	= 1.0e100;
	double leg1_multFactor	= 1.0;
	double leg1_addFactor	= 0.0;

	bool   leg2_ifFloored   = false;
	double leg2_floorStrike = -1.0e100;
	bool   leg2_ifCapped    = false;
	double leg2_capStrike   = 1.0e100;
	double leg2_multFactor	= 1.0;
	double leg2_addFactor	= 0.0;

	std::string leg1CouponDependency = "";
	std::string leg2CouponDependency = "";

	LMM::Index  valuationDateIndex = 0 ; 

	for(size_t i=0;i<leg1PaymentIndexSchedule.size(); i++)
	{
		
		LMM::Index couponPaymentIndex	=	leg1PaymentIndexSchedule[i];
		LMM::Index indexLibor			=	couponPaymentIndex-leg1VsLiborTenorTypeRatio;
		Rate_PTR liborRate(new LiborRate(indexLibor,leg1Tenor));
		leg1CouponVector.push_back(Coupon_CONSTPTR(new TargetCoupon(	couponPaymentIndex, 
																		nominal, 
																		leg1Tenor.YearFraction(), 
																		leg1_ifFloored,
																		leg1_floorStrike,
																		leg1_ifCapped,
																		leg1_capStrike,
																		liborRate,
																		leg1_multFactor,
																		leg1_addFactor,
																		valuationDateIndex,
																		target,
																		leg1CouponDependency)));
		leg1CouponDependency += "l"; 
	}


	for(size_t i=0;i<leg2PaymentIndexSchedule.size(); i++)
	{
		Rate_PTR constRate(new ConstRate(strike));
		LMM::Index couponPaymentIndex=leg2PaymentIndexSchedule[i];
		leg2CouponVector.push_back(Coupon_CONSTPTR(new TargetCoupon(	couponPaymentIndex, 
																		nominal, 
																		leg2Tenor.YearFraction(), 
																		leg2_ifFloored,
																		leg2_floorStrike,
																		leg2_ifCapped,
																		leg2_capStrike,
																		constRate,
																		leg2_multFactor,
																		leg2_addFactor,
																		valuationDateIndex,
																		target,
																		leg2CouponDependency)));
		leg2CouponDependency += "l"; 
	}
	GeneticSwap_PTR targetSwap(new GeneticSwap(	CouponLeg_CONSTPTR(new CouponLeg(leg1CouponVector)),
												CouponLeg_CONSTPTR(new CouponLeg(leg2CouponVector))));
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
