#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>

#include <Coupon.h>



class CouponLeg
{
	std::vector<Coupon_PTR>  leg1_;
	std::vector<Coupon_PTR>  leg2_;
public:
	// getter, constructor, destructor ... 
	CouponLeg(){}
	CouponLeg(std::vector<Coupon_PTR>  leg1, std::vector<Coupon_PTR>  leg2);

	//getter
	std::vector<Coupon_PTR> getLeg1()const{return leg1_;}
	std::vector<Coupon_PTR> getLeg2()const{return leg2_;}

	//setter: add an element
	void addLeg1(Coupon_PTR c){leg1_.push_back(c);}
	void addLeg2(Coupon_PTR c){leg2_.push_back(c);}
	virtual ~CouponLeg(){}
	// end of getter, constructor, destructor...
	boost::shared_ptr<CouponLeg> getCouponLeg(size_t indexStart, size_t indexEnd) const;  // couponPaymentDate in [indexStart, indexEnd[  

};
typedef boost::shared_ptr<CouponLeg> CouponLeg_PTR;
typedef boost::shared_ptr<const CouponLeg> CouponLeg_CONSTPTR;