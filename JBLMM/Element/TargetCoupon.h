#pragma once
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>

#include <JBLMM/Element/CappedFlooredCoupon.h>

class TargetCoupon : public CappedFlooredCoupon
{
	double target_; // K  ---- check target is the same strike
	std::string couponDependency_;  // string = sumToBeforeLast

public:
	//getter
	double getTarget()const{return target_;}
	std::string getCouponDependency()const{return couponDependency_;}
	//constructor, destructor
	TargetCoupon(	LMM::Index paymentIndex,
		            double nominal,
					double period, 
					bool   ifFloored,
					double floorStrike,
					bool   ifCapped, 
					double capStrike, 
					Rate_CONSTPTR rate,
					double multiFactor,
					double addFactor, 
					LMM::Index valuationDateIndex,
					double target,
					std::string couponDependency);  
	virtual ~TargetCoupon(){}
};
typedef boost::shared_ptr<TargetCoupon> TargetCoupon_PTR;
typedef boost::shared_ptr<const TargetCoupon> TargetCoupon_CONSTPTR;

