#include "CouponLeg.h"
#include <vector>

CouponLeg::CouponLeg(std::vector<Coupon_PTR>  leg1, std::vector<Coupon_PTR>  leg2)
	{
		leg1_=leg1;
		leg2_=leg2;
	}

boost::shared_ptr<CouponLeg> CouponLeg::getCouponLeg(size_t indexStart, size_t indexEnd) const
	{
		std::vector<Coupon_PTR>  leg1;
		std::vector<Coupon_PTR>  leg2;
		for(size_t i=0; i<getLeg1().size(); i++)
		{
			Coupon_PTR coupon=getLeg1()[i];
			if(coupon->getPaymentDate()>=indexStart&&coupon->getPaymentDate()<indexEnd)
			{
				leg1.push_back(coupon->clone());
			}
		}
		for (size_t i=0; i<getLeg2().size(); i++)
		{
			Coupon_PTR coupon=getLeg1()[i];
			if(coupon->getPaymentDate()>=indexStart&&coupon->getPaymentDate()<indexEnd)
			{
				leg2.push_back(coupon->clone());
			}
		}
		return boost::shared_ptr<CouponLeg>(new CouponLeg(leg1, leg2)); // copy coupon other than use smart pointer   !!!!!!!!!!!!!!!!!!!!! YY !!!!!!!!!!!!!!!!!!
	}

