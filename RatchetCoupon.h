#pragma once

#include<CappedFlooredCoupon.h>

#include <boost/shared_ptr.hpp>



class RatchetCoupon : public CappedFlooredCoupon
{
	double leverage_;
	double DependentCoupon_;  //-1
};
typedef boost::shared_ptr<RatchetCoupon> RatchetCoupon_PTR;
typedef boost::shared_ptr<const RatchetCoupon> RatchetCoupon_CONSTPTR;


