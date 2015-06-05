#include "JBLMM/Element/LiborRate.h"
#include <LMM/helper/TenorType.h>

LiborRate::LiborRate(double fixingTime, const Tenor duration)
	:
		fixingTime_(fixingTime),
		duration_(duration)
{
}


Rate_PTR LiborRate::clone()const
{
	return Rate_PTR(new LiborRate(*this));
}

ConstRate::ConstRate(const double constRateValue)
	:
	constRateValue_(constRateValue)
{
}

Rate_PTR ConstRate::clone()const
{
	return Rate_PTR(new ConstRate(*this));
}





