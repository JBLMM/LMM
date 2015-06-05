#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>

#include <Coupon.h>



class CouponLeg
{

	std::vector<Coupon_CONSTPTR>  leg_;

public:
	// getter, constructor, destructor ... 
	CouponLeg(const std::vector<Coupon_CONSTPTR>&  leg); // const std::vector<Coupon_CONSTPTR>&  leg2);

	//getter
	const std::vector<Coupon_CONSTPTR>& getLeg()const{return leg_;}

	//setter
	void addToLeg(Coupon_CONSTPTR coupon);

	//setter: add an element
	void addLeg(Coupon_CONSTPTR c){leg_.push_back(c);}
	virtual ~CouponLeg(){}

	// end of getter, constructor, destructor...  // couponPaymentDate in [indexStart, indexEnd[  
	boost::shared_ptr<const CouponLeg> getSubCouponLeg(size_t indexStart, size_t indexEnd) const;  

};
typedef boost::shared_ptr<CouponLeg> CouponLeg_PTR;
typedef boost::shared_ptr<const CouponLeg> CouponLeg_CONSTPTR;