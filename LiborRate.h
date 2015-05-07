#pragma once

#include <boost/shared_ptr.hpp>

#include<Rate.h>
#include<LMM\helper\TenorType.h>

class LiborRate : public Rate1
{
	double fixingTime_; // T_{i-1}
	Tenor  duration_; // 3m, 6m 
	Rate_PTR clone(){return Rate_PTR(new boost::shared_ptr<LiborRate>(*this));}
};
typedef boost::shared_ptr<LiborRate> LiborRate_PTR;
typedef boost::shared_ptr<LiborRate> LiborRate_CONSTPTR;

