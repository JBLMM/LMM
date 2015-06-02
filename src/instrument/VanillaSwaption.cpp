#include <cassert>
#include <cmath>

#include <LMM/instrument/VanillaSwaption.h>


VanillaSwaption::VanillaSwaption(const VanillaSwap& vanillaSwap, OptionType::OptionType vanillaSwaptionType)
: vanillaSwap_(vanillaSwap)
, vanillaSwaptionType_(vanillaSwaptionType)
{
	
}

double VanillaSwaption::payoff(const double& pvVloatingLeg, const double& pvFixedLeg) const // for MC
{
	if(vanillaSwaptionType_ == OptionType::CALL)
		return std::max(pvVloatingLeg - pvFixedLeg, 0.0);
	else
		return std::max(pvFixedLeg - pvVloatingLeg, 0.0);
}

void VanillaSwaption::show()
{
	std::cout << "---------------------------------------------" << std::endl ;
	std::cout << "--- creation d'un objet VanillaSwaption ----" << std::endl ;
	vanillaSwap_.show() ;
	std::cout << "---------------------------------------------" << std::endl ;
}