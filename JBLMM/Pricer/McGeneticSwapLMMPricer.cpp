#include "JBLMM/Pricer/McGeneticSwapLMMPricer.h"
#include <cassert>
#include <iostream>
#include <math.h>

#include <JBLMM/Element/Coupon.h>
#include <JBLMM/Element/CappedFlooredCoupon.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <JBLMM/Instrument/GeneticSwap.h>
#include <JBLMM/Element/CouponLeg.h>
#include <JBLMM/Element/LiborRate.h>
#include <JBLMM/Element/Rate.h>



//double McLmmVanillaSwapPricer::swapRate(const VanillaSwap& vanillaSwap, size_t nbSimulation) const
//{
//	throw("Error: not implemented");
//}

//size_t McGeneticSwapLMMPricer::find(double paymentDay, std::vector<double> dateLibor)const
//{
//	assert(paymentDay>=0);
//	for(size_t i=0; i<dateLibor.size();i++)
//	{
//		if(paymentDay<dateLibor[i]){return i-1;}
//	}
//	return dateLibor.size()-1;
//}

//simulation
double McGeneticSwapLMMPricer::swapNPV(const GeneticSwap_CONSTPTR geneticSwap, size_t nbSimulation)  const
{
	double result = 0.0;
	double variance = 0.0;

	LMMTenorStructure_PTR lmmTenorStructure=getMcLmm()->get_lmm()->get_LMMTenorStructure();

	LMM::Index indexValuationDate = 0;
	for(size_t itrSimulation=0; itrSimulation<nbSimulation; ++itrSimulation)
	{
		mcLmm_->simulateLMM();  // YY TODO: not efficient at all, don't need to do all the simulation ... 
		double npvFloatingLeg_Get  = pvFloatingLeg(indexValuationDate, geneticSwap->getLeg1(), mcLmm_->get_numeraire(), mcLmm_->get_liborMatrix(),lmmTenorStructure);
		double npvFixedLeg_Give    = pvFloatingLeg  (indexValuationDate, geneticSwap->getLeg2(), mcLmm_->get_numeraire(), mcLmm_->get_liborMatrix(),lmmTenorStructure);
		double npvSwap		       = npvFloatingLeg_Get - npvFixedLeg_Give;
		result				      += npvSwap;
		variance += npvSwap*npvSwap;
	}
	result   /=nbSimulation; 

	if(LMM::DEUBGLMM())
	{
		variance = (variance/nbSimulation-result*result);	
	    std::cout << LMM::NOTIF_MSG 
			      << " mcScheme=" << MCSchemeType::mcSchemeType2String(mcLmm_->get_mcSchemeType())
			      << ", SwapPriceMC=" << result
				  << ", nbSimualtion=" << nbSimulation 
			      << ",  99% confidential interval = " << 2.57*sqrt(variance / nbSimulation) << std::endl;
	}	

	return result;
}


//! for MC
double McGeneticSwapLMMPricer::pvFloatingLeg(		LMM::Index indexValuationDate,
													const CouponLeg_CONSTPTR couponLeg,
													const std::vector<double>& numeraire, 
													const matrix& liborMatrix,
													LMMTenorStructure_PTR lmmTenorStructure)const 
{
	//assert(indexValuationDate <= GeneticSwap.get_indexStart()); //YY TODO: this test too slow, esp: within MC simulation
	//assert(pLMMTenorStructure_->get_horizon() >= GeneticSwap.get_indexEnd());  // if not cannot price this swap;

	double price = 0.0;
	std::vector<Coupon_CONSTPTR> couponList=couponLeg->getLeg();
	for(size_t i=0; i<couponList.size(); i++)
	{
		Coupon_CONSTPTR coupon=couponList[i];		
		LMM::Index paymentIndex = coupon->getPaymentIndex();
		
		if(paymentIndex<=indexValuationDate)continue;

		price+=numeraire[indexValuationDate]/numeraire[paymentIndex]*calculate(coupon, liborMatrix(paymentIndex-1,indexValuationDate));
	}

	//const std::vector<LMM::Index>& floatingLegPaymentIndexSchedule = GeneticSwap.get_floatingLegPaymentIndexSchedule();
	//for(size_t itr = 0; itr < floatingLegPaymentIndexSchedule.size(); ++itr)
	//{
	//	//! At time T_{i+1}, pay: L_i(T_i)
	//	size_t floatingLegPaymentIndex = floatingLegPaymentIndexSchedule[itr]; // i+1
	//	//! YY TODO: restriction, Attention: this line suppose: MCLMM simulation use the same lmmTenor to simulate libor !!!
	//	size_t indexLibor = floatingLegPaymentIndex-1; // condition: floatingTenor = lmmTenor
	//	size_t indexT     = indexLibor;                                        // = i
	//	double libor      = liborMatrix(indexLibor, indexT);
	//	double delta_T    = GeneticSwap.get_DeltaTFloatLeg(itr);
	//	price			 += delta_T*libor*numeraire[indexValuationDate]/numeraire[floatingLegPaymentIndex];		
	//}
	return price;
}

double McGeneticSwapLMMPricer::calculate(const Coupon_CONSTPTR coupon, double liborValue)const
{
	Coupon_PTR coupon_nonConst		=	boost::const_pointer_cast<Coupon>(coupon);
	CappedFlooredCoupon_PTR cappedFlooredCoupon = boost::dynamic_pointer_cast<CappedFlooredCoupon>(coupon_nonConst);

	double nominal		=	cappedFlooredCoupon->getNominal(); 
	double period		=	cappedFlooredCoupon->getPeriod();
	double floor		=	cappedFlooredCoupon->getFloorStrike();
	double cap			=	cappedFlooredCoupon->getCapStrike();
	double multiFactor	=	cappedFlooredCoupon->getMultiFactor();
	double addFactor	=	cappedFlooredCoupon->getAddFactor();

	return nominal*period*std::max(floor, std::min(cap,multiFactor*liborValue+addFactor));
}

//double McGeneticSwapPricer::swapRate(LMM::Index indexValuationDate,
//					const GeneticSwap_PTR geneticSwap,
//					const std::vector<double>& numeraire, 
//					const matrix& liborMatrix,
//					LMMTenorStructure_PTR lmmTenorStructure) const
//{
//	double pvFloating = pvFloatingLeg(indexValuationDate, geneticSwap->getLeg1(), numeraire, liborMatrix,lmmTenorStructure);
//	double annuity = pvFixedLeg   (indexValuationDate, GeneticSwap, numeraire);
//	return pvFloating / annuity;
//}

//double MCLmmVanillaSwapPricer::annuity( LMM::Index indexValuationDate,
//										const VanillaSwap& vanillaSwap,
//									    const std::vector<double>& numeraire) const
//{
//	assert(indexValuationDate <= vanillaSwap.get_indexStart()); //YY TODO: this test too slow, esp: within MC simulation
//	assert(pLMMTenorStructure_.get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
//
//	double price = 0.0;
//	const std::vector<LMM::Index>& fixedLegPaymentIndexSchedule  = vanillaSwap.get_fixedLegPaymentIndexSchedule();
//	for(size_t itr = 0; itr < fixedLegPaymentIndexSchedule.size(); ++itr)
//	{
//		size_t fixedLegPaymentIndex = fixedLegPaymentIndexSchedule[itr]; // = i+1
//		double delta_T              = deltaTFixedLeg_[itr];
//		//std::cout << "numeraire[indexValuationDate]/numeraire[fixedLegPaymentIndex] = " << numeraire[indexValuationDate]/numeraire[fixedLegPaymentIndex] << std::endl;
//		price			           += delta_T*numeraire[indexValuationDate]/numeraire[fixedLegPaymentIndex];		
//	}
//	return price;
//}
//
//double MCLmmVanillaSwapPricer::pvFixedLeg(LMM::Index indexValuationDate,
//	                                      const VanillaSwap& vanillaSwap,
//									      const std::vector<double>& numeraire)  const
//{
//	assert(indexValuationDate <= vanillaSwap.get_indexStart()); //YY TODO: this test too slow, esp: within MC simulation
//	assert(pLMMTenorStructure_.get_horizon() >= vanillaSwap.get_indexEnd());  // if not cannot price this swap;
//
//	return vanillaSwap.get_strike()*annuity(indexValuationDate,vanillaSwap,numeraire);
//}
//

