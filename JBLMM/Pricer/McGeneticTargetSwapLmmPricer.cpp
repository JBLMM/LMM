#include <JBLMM/Pricer/McGeneticTargetSwapLmmPricer.h>

#include <JBLMM/Element/Coupon.h>
#include <JBLMM/Element/CappedFlooredCoupon.h>
#include <LMM/helper/LMMTenorStructure.h>
#include <JBLMM/Instrument/GeneticSwap.h>
#include <JBLMM/Element/CouponLeg.h>
#include <JBLMM/Element/LiborRate.h>
#include <JBLMM/Element/Rate.h>
#include <JBLMM/Element/TargetCoupon.h>


double McGeneticTargetSwapLmmPricer::swapNPV(const GeneticSwap_CONSTPTR geneticSwap, size_t nbSimulation)const
{
	double result = 0.0;
	double variance = 0.0;

	LMMTenorStructure_PTR lmmTenorStructure=getMcLmm()->get_lmm()->get_LMMTenorStructure();

	LMM::Index indexValuationDate = 0;
	for(size_t itrSimulation=0; itrSimulation<nbSimulation; ++itrSimulation)
	{
		mcLmm_->simulateLMM();  // YY TODO: not efficient at all, don't need to do all the simulation ... 
		double npvFloatingLeg_Get  = evaluateCouponLeg(indexValuationDate, geneticSwap->getLeg1(), mcLmm_->get_numeraire(), mcLmm_->get_liborMatrix(),lmmTenorStructure);
		double npvFixedLeg_Give    = evaluateCouponLeg(indexValuationDate, geneticSwap->getLeg2(), mcLmm_->get_numeraire(), mcLmm_->get_liborMatrix(),lmmTenorStructure);
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
double McGeneticTargetSwapLmmPricer::evaluateCouponLeg(	LMM::Index indexValuationDate,
														const CouponLeg_CONSTPTR couponLeg,
														const std::vector<double>& numeraire, 
														const matrix& liborMatrix,
														LMMTenorStructure_PTR lmmTenorStructure)const 
{
	double price = 0.0;
	std::vector<Coupon_CONSTPTR> couponList=couponLeg->getLeg();
	
	for(size_t i=0; i<couponList.size(); i++)
	{
		Coupon_CONSTPTR coupon=couponList[i];		
		LMM::Index paymentIndex = coupon->getPaymentIndex();

		if(paymentIndex<=indexValuationDate)continue;
		
		TargetCoupon_CONSTPTR targetCoupon = boost::dynamic_pointer_cast<const TargetCoupon>(coupon);
		if(!targetCoupon)
			throw("fail to cast targetCoupon");
		double target	=	targetCoupon->getTarget();

		if(price<target)
		{	
			Rate_CONSTPTR	rate		=	targetCoupon->getRate();
			LiborRate_CONSTPTR liborRate		=	boost::dynamic_pointer_cast<const LiborRate>(rate);
			if(!liborRate)
				throw("fail to cast liborRate");

			LMM::Index	indexLibor				=	liborRate->getFixingTime();			
			assert(indexLibor<=paymentIndex);

			double numeraire1		=	numeraire[indexValuationDate];
			double numeraire2		=	numeraire[paymentIndex];
			double numeraireLocal	=	numeraire1/numeraire2;
			double liborValue		=	liborMatrix(indexLibor, indexLibor);
			double result_calculate	=	calculate(coupon, liborValue);			
			price+=numeraire[indexValuationDate]/numeraire[paymentIndex]*calculate(coupon, liborMatrix(indexLibor,indexLibor));
		}
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


double McGeneticTargetSwapLmmPricer::calculate(Coupon_CONSTPTR coupon, double liborValue)const
{
	CappedFlooredCoupon_CONSTPTR cappedFlooredCoupon = boost::dynamic_pointer_cast<const CappedFlooredCoupon>(coupon);

	double nominal		=	cappedFlooredCoupon->getNominal(); 
	double period		=	cappedFlooredCoupon->getPeriod();
	double floor		=	cappedFlooredCoupon->getFloorStrike();
	double cap			=	cappedFlooredCoupon->getCapStrike();
	double multiFactor	=	cappedFlooredCoupon->getMultiFactor();
	double addFactor	=	cappedFlooredCoupon->getAddFactor();

	return nominal*period*std::max(floor, std::min(cap,multiFactor*liborValue+addFactor));
}




