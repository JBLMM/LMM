#include "JBLMM/Element/CouponLeg.h"
#include <vector>

CouponLeg::CouponLeg(const  std::vector<Coupon_CONSTPTR>&  leg)
	:	leg_(leg)
{
}

CouponLeg_CONSTPTR CouponLeg::getSubCouponLeg(size_t indexStart, size_t indexEnd) const  //Question: why boost::shared_ptr<const CouponLeg&>?  //deep copy a sub-sequence of CouponLeg
	{
		std::vector<Coupon_CONSTPTR>  leg;
		for(size_t i=0; i<getLeg().size(); i++)
		{
			Coupon_CONSTPTR coupon=getLeg()[i];
			if(coupon->getPaymentIndex() >= indexStart&&coupon->getPaymentIndex()<indexEnd)
			{
				leg.push_back(coupon->clone());  // ! deep copy
			}
		}
		return CouponLeg_CONSTPTR(new CouponLeg(leg)); // copy coupon other than use smart pointer   !!!!!!!!!!!!!!!!!!!!! YY !!!!!!!!!!!!!!!!!!
}

void CouponLeg::addToLeg(Coupon_CONSTPTR coupon)
{
	leg_.push_back(coupon);
}