#pragma once

#include <boost/shared_ptr.hpp>

#include<Rate.h>
#include<LMM\helper\TenorType.h>
#include<LMM\instrument\VanillaSwap.h>


class VanillaSwapRate : public Rate1
{
	const VanillaSwap vanillaSwap_;
public:
	//getter
	const VanillaSwap& getVanillaSwap()const{return vanillaSwap_;}

	//constructeur
	VanillaSwapRate(const VanillaSwap& vanillaSwap);

	//clone
	Rate_PTR clone()const;
};
typedef boost::shared_ptr<VanillaSwapRate> VanillaSwapRate_PTR;
typedef boost::shared_ptr<VanillaSwapRate> VanillaSwapRate_CONSTPTR;

