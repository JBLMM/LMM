#include "LiborRate.h"
#include <LMM/helper/TenorType.h>

LiborRate::LiborRate(double fixingTime, const Tenor duration)
	:fixingTime_(fixingTime),
	 duration_(duration)
{}

//LiborRate::LiborRate():duration_(Tenor::_1M){};
Rate_PTR LiborRate::clone()const
{
	return Rate_PTR(new LiborRate(*this));
}



