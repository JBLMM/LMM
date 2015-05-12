#pragma once

#include <boost/shared_ptr.hpp>

#include<Rate.h>
#include<LMM\helper\TenorType.h>

class LiborRate : public Rate1
{
	double fixingTime_; // T_{i-1}
	Tenor  duration_; // 3m, 6m 
public:
	LiborRate();
	LiborRate(double fixingTime, const Tenor duration);
	Rate_PTR clone()const;
};
typedef boost::shared_ptr<LiborRate> LiborRate_PTR;
typedef boost::shared_ptr<LiborRate> LiborRate_CONSTPTR;

