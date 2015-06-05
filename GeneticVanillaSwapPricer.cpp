#include "GeneticVanillaSwapPricer.h"
#include <Coupon.h>
#include <CappedFlooredCoupon.h>

#include <math.h>
#include <memory>
#include <iostream>


//size_t GeneticVanillaSwapPricer::find(double paymentDay, std::vector<double> dateLibor)const
//{
//	assert(paymentDay>=0);
//	for(size_t i=0; i<dateLibor.size();i++)
//	{
//		if(paymentDay<dateLibor[i]){return i-1;}
//	}
//	return dateLibor.size()-1;
//}

double GeneticVanillaSwapPricer::geneticVanillaSwap_Analytical(GeneticSwap_CONSTPTR geneticVanillaSwap, const std::vector<double>& liborsInitValue)const 
{
	std::vector<Coupon_CONSTPTR> floatingLeg	=	geneticVanillaSwap->getLeg1()->getLeg();
	std::vector<Coupon_CONSTPTR> fixedLeg		=	geneticVanillaSwap->getLeg2()->getLeg();

	LMM::Index liborHorizon			=	liborsInitValue.size();
	LMM::Index floatingLegHorizon	=	floatingLeg.size();
	LMM::Index fixedLegHorizon		=	fixedLeg.size();


	//cast from Coupon_CONSTPTR to CappedFlooredCoupon_PTR
	Coupon_PTR c		=	boost::const_pointer_cast<Coupon>(floatingLeg[0]);
	CappedFlooredCoupon_PTR cfc = boost::dynamic_pointer_cast<CappedFlooredCoupon>(c);

	//get deltaT from the first floatingcoupon 
	const double deltaT				=	cfc->getPeriod();

	//calculate zero-coupon
	std::vector<double> ZC(liborHorizon+1);
	ZC[0] = 1.0;
	for(size_t i=1; i<ZC.size(); ++i)
	{
		ZC[i] = ZC[i-1]/(1+deltaT*liborsInitValue[i-1]);
	}

	// Caculate the floating leg
	double floatingIncome = 0.0;
	for(size_t i=0; i<floatingLegHorizon;i++)
	{
		Coupon_CONSTPTR coupon	= floatingLeg[i];

		LMM::Index paymentIndex = coupon->getPaymentIndex();

		Coupon_PTR coupon_nonConst		=	boost::const_pointer_cast<Coupon>(coupon);
		CappedFlooredCoupon_PTR cappedFlooredCoupon = boost::dynamic_pointer_cast<CappedFlooredCoupon>(coupon_nonConst);

		if(!cappedFlooredCoupon)
			throw("fail to cast coupon to CappedFlooredCoupon"); 


		double tauxActualisation = ZC[paymentIndex];	

		floatingIncome	+=	tauxActualisation*
							cappedFlooredCoupon->getNominal()*
							cappedFlooredCoupon->getPeriod()*
							std::max(	cappedFlooredCoupon->getFloorStrike(), 
										std::min(	cappedFlooredCoupon->getCapStrike(),
													cappedFlooredCoupon->getMultiFactor()*liborsInitValue[paymentIndex-1]
													+cappedFlooredCoupon->getAddFactor()));
	}

	// Caculate the fixed leg
	double fixedIncome=0.0;
	for(size_t i=0; i<fixedLegHorizon;i++)
	{
		Coupon_CONSTPTR coupon	=	fixedLeg[i];

		// cast to CappedFlooredCoupon_PTR
		Coupon_PTR coupon_nonConst		=	boost::const_pointer_cast<Coupon>(coupon);
		CappedFlooredCoupon_PTR cappedFlooredCoupon = boost::dynamic_pointer_cast<CappedFlooredCoupon>(coupon_nonConst);

		LMM::Index paymentIndex = cappedFlooredCoupon->getPaymentIndex();

		double tauxActualisation = ZC[paymentIndex];

		fixedIncome	+=	tauxActualisation
						*cappedFlooredCoupon->getNominal()
						*cappedFlooredCoupon->getPeriod()*
						std::max(	cappedFlooredCoupon->getFloorStrike(), 
									std::min(	cappedFlooredCoupon->getCapStrike(),
																		cappedFlooredCoupon->getMultiFactor()
																		*liborsInitValue[paymentIndex-1]
																		+cappedFlooredCoupon->getAddFactor()));
	}
	
	return floatingIncome-fixedIncome;

}
