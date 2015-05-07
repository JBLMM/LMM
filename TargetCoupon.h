#pragma once
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>

#include<CappedFlooredCoupon.h>

class TargetCoupon : public CappedFlooredCoupon
{
	double target_; // K  ---- check target is the same strike
	std::string couponePendency_;  // string = sumToBeforeLast
};
typedef boost::shared_ptr<TargetCoupon> TargetCoupon_PTR;
typedef boost::shared_ptr<const TargetCoupon> TargetCoupon_CONSTPTR;

