#include "LiborRate.h"
#include <LMM/helper/TenorType.h>

LiborRate::LiborRate(double fixingTime, const Tenor duration):duration_(duration){fixingTime_=fixingTime;}
LiborRate::LiborRate():duration_(Tenor::_1M){};
Rate_PTR LiborRate::clone()const{return Rate_PTR(new LiborRate(*this));}



