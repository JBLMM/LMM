#include "JBLMM/Element/Rate.h"
#include <boost/shared_ptr.hpp>

Rate_PTR Rate1::clone()const
{
	return Rate_PTR(new Rate1(*this));
}



