#include "GeneticVanillaSwap.h"
#include <Coupon.h>
#include <CappedFlooredCoupon.h>

#include <math.h>
#include <memory>
#include <iostream>


size_t GeneticVanillaSwapPricer::find(double paymentDay, std::vector<double> dateLibor)const
{
	assert(paymentDay>=0);
	for(size_t i=0; i<dateLibor.size();i++)
	{
		if(paymentDay<dateLibor[i]){return i-1;}
	}
	return dateLibor.size()-1;
}

double GeneticVanillaSwapPricer::geneticVanillaSwap_Analytical_1(std::vector<double> liborInitValue, std::vector<double> dateLibor)const 
{
	GeneticSwap_PTR gs=getGeneticVanillaSwap();
	CouponLeg_PTR floatingLeg1= gs->getLeg1();
	CouponLeg_PTR fixedLeg1= gs->getLeg2();
	std::vector<Coupon_PTR> floatingLeg=floatingLeg1->getLeg1();
	std::vector<Coupon_PTR> fixedLeg=fixedLeg1->getLeg1();

	
	size_t liborHorizon = liborInitValue.size();
	size_t floatingLegHorizon = floatingLeg.size();
	size_t fixedLegHorizon = fixedLeg.size();

	//calculate zero-coupon
	std::vector<double> ZC(liborHorizon+1);
	ZC[0] = 1.0;
	for(size_t i=1; i<ZC.size(); ++i)
	{
		ZC[i] = ZC[i-1]/(1+(dateLibor[i]-dateLibor[i-1])*liborInitValue[i-1]);
	}
	// Caculate the floating leg
	double floatingIncome=0.0;
	for(size_t i=0; i<floatingLegHorizon;i++)
	{
		Coupon_PTR cfc=floatingLeg[i];
		size_t index=find(cfc->getPaymentDate(), dateLibor);
		double tauxActualisation = ZC[index];		
		floatingIncome+=tauxActualisation*cfc->getNominal()*cfc->getPeriod()*
			std::max(cfc->getFloor(), 
			std::min(cfc->getCap(),
			cfc->getMultiFactor()*liborInitValue[index-1]
		+cfc->getAddFactor()));
	}

	// Caculate the fixed leg
	double fixedIncome=0.0;
	for(size_t i=0; i<fixedLegHorizon;i++)
	{
		Coupon_PTR cfc=fixedLeg[i];
		size_t index=find(cfc->getPaymentDate(), dateLibor);
		double tauxActualisation = ZC[index];		
		fixedIncome+=2*tauxActualisation*cfc->getNominal()*cfc->getPeriod()*std::max(cfc->getFloor(), std::min(cfc->getCap(),cfc->getMultiFactor()*liborInitValue[index-1]+cfc->getAddFactor()));
	}
	
	return floatingIncome-fixedIncome;
	
}
