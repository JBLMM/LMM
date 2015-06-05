#include "VanillaSwapRate.h"
#include <boost/shared_ptr.hpp>

VanillaSwapRate::VanillaSwapRate(const VanillaSwap& vanillaSwap)
	:
	vanillaSwap_(vanillaSwap)
{
}

Rate_PTR VanillaSwapRate::clone()const
{
	return Rate_PTR(new VanillaSwapRate(*this));
}

//Rate_PTR Rate::clone_Rate_PTR()const{return Rate_PTR(new Rate(*this));}


