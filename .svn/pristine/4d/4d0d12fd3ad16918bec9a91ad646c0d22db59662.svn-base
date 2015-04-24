#include <cassert>

#include <LMM/pricer/McLmmVanillaSwaptionPricer.h>

double McLmmVanillaSwaptionPricer::price(const VanillaSwaption& vanillaSwaption, size_t nbSimulation)  const
{
	clock_t start_pricing = std::clock();
	assert( mcLmm_->get_lmm()->get_LMMTenorStructure()->get_tenorType() == vanillaSwaption.get_lmmTenorStructureTenorType()); // this pricer can price this swaption

	VanillaSwap vanillaSwap = vanillaSwaption.getUnderlyingSwap();

	double result   = 0.0;
	double variance = 0.0;

	LMM::Index indexValuationdate = 0;
	LMM::Index indexMaturity = vanillaSwaption.get_indexMaturity();

	for(size_t itrSimulation=0; itrSimulation<nbSimulation; ++itrSimulation)
	{
		mcLmm_->simulateLMM();  // YY TODO: not efficient at all, don't need to do all the simulation ... 
		const matrix& liborMatrix  = mcLmm_->get_liborMatrix();
		const std::vector<double>& numeraire = mcLmm_->get_numeraire();
		double npvFloatingLeg      = pvFloatingLeg(indexMaturity, vanillaSwap, numeraire, liborMatrix);
		double npvFixedLeg         = pvFixedLeg   (indexMaturity, vanillaSwap, numeraire);
		double payoffAtMaturity    = vanillaSwaption.payoff(npvFloatingLeg,npvFixedLeg);
		double value               = payoffAtMaturity * numeraire[indexValuationdate]/numeraire[indexMaturity];
		result					  += value;

		variance += value*value;
	}
	result /= nbSimulation; 

	clock_t end_pricing = std::clock();
	double pricing_duration = double(end_pricing - start_pricing) / CLOCKS_PER_SEC;

	if(LMM::DEUBGLMM())
	{
		variance = (variance/nbSimulation-result*result);	
		std::cout << LMM::NOTIF_MSG 
			      << " mcScheme = " << MCSchemeType::mcSchemeType2String(mcLmm_->get_mcSchemeType())
			      << ", nbSimualtion = " << nbSimulation
				  << ", Pricing Time = " << pricing_duration
				  << ", SwaptionPriceMC = " << result
			      << ",  99% confidential interval = " << 2.57*std::sqrt(variance / nbSimulation) << std::endl;
	}

	return result;
}
