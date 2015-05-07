#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>

#include <Coupon.h>



class CouponLeg: public Coupon
{
	std::vector<Coupon_PTR>  leg1_;
	std::vector<Coupon_PTR>  leg2_;
public:
	// getter, constructor, destructor ... 
	CouponLeg(){}
	CouponLeg(std::vector<Coupon_PTR>  leg1, std::vector<Coupon_PTR>  leg2)
	{
		leg1_=leg1;
		leg2_=leg2;
	}
	//getter
	std::vector<Coupon_PTR> getLeg1()const{return leg1_;}
	std::vector<Coupon_PTR> getLeg2()const{return leg2_;}
	virtual ~CouponLeg(){}
	// end of getter, constructor, destructor...
	boost::shared_ptr<CouponLeg> getCouponLeg(size_t indexStart, size_t indexEnd) const  // couponPaymentDate in [indexStart, indexEnd[  
	{
		std::vector<Coupon_PTR>  leg1;
		std::vector<Coupon_PTR>  leg2;
		for(int i=0; i<getLeg1().size(); i++)
		{
			Coupon_PTR coupon=getLeg1()[i];
			if(coupon->getPaymentDate()>=indexStart&&coupon->getPaymentDate()<indexEnd)
			{
				leg1.push_back(coupon->clone());
			}
		}
		for (int i=0; i<getLeg2().size(); i++)
		{
			Coupon_PTR coupon=getLeg1()[i];
			if(coupon->getPaymentDate()>=indexStart&&coupon->getPaymentDate()<indexEnd)
			{
				leg2.push_back(coupon->clone());
			}
		}
		return boost::shared_ptr<CouponLeg>(new CouponLeg(leg1, leg2)); // copy coupon other than use smart pointer   !!!!!!!!!!!!!!!!!!!!! YY !!!!!!!!!!!!!!!!!!
	}
};
typedef boost::shared_ptr<CouponLeg> CouponLeg_PTR;
typedef boost::shared_ptr<const CouponLeg> CouponLeg_CONSTPTR;